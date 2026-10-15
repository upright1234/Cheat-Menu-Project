#pragma once
// Diagnostic log for testing the menu through the loader, where there is no console or debugger.
// Compiled in only with /DMENU_DEBUG_LOG=1; otherwise MENU_LOG(...) / MENU_LOG_FRAME(...) are no-ops and
// normal builds are unchanged.
//
// Appends to C:\Users\Public\Documents\menu_log.txt (or %TEMP%\menu_log.txt if that is not writable) and reopens
// the file for every line, so nothing is lost if the process dies right after. It records:
//   - the DLL being loaded (static initializer, before DllMain) with the host process and image range
//   - each step of InitMyMenu, the first frames, and the first RenderMyMenu calls with a few Settings fields
//   - crashes with the menu's code involved: exception, address as menu+RVA, and menu+RVA values found on the
//     stack. Look the RVAs up in the linker map (/MAP) of the same build.
// Kernel32 functions are resolved at runtime so the import table stays identical to a normal build.
#ifndef MENU_DEBUG_LOG
#define MENU_DEBUG_LOG 0
#endif

#if MENU_DEBUG_LOG
#include <Windows.h>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstring>

#include "../CheatSettings/Config.h"
#include "../Imgui/imgui.h"
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "../Imgui/imgui_internal.h"

extern "C" IMAGE_DOS_HEADER __ImageBase;

namespace menulog {

struct Kernel32 {
    decltype(&::CreateFileA) CreateFileA;
    decltype(&::WriteFile) WriteFile;
    decltype(&::CloseHandle) CloseHandle;
    decltype(&::GetTempPathA) GetTempPathA;
    decltype(&::GetLocalTime) GetLocalTime;
    decltype(&::GetCurrentProcessId) GetCurrentProcessId;
    decltype(&::GetCurrentThreadId) GetCurrentThreadId;
    decltype(&::GetModuleFileNameA) GetModuleFileNameA;
    decltype(&::GetModuleHandleExA) GetModuleHandleExA;
    decltype(&::VirtualQuery) VirtualQuery;
    decltype(&::AddVectoredExceptionHandler) AddVectoredExceptionHandler;
};

inline Kernel32 k32{};
inline bool g_ready = false;
inline char g_path[MAX_PATH] = "";
inline uintptr_t g_base = 0, g_end = 0;
inline volatile LONG g_frames = 0;
inline volatile LONG g_menuDraws = 0; // RenderMyMenu calls; main.cpp only calls it while *ShowMenu is true
inline volatile LONG g_inHandler = 0;
inline volatile LONG g_reported = 0;

inline bool Resolve() {
    if (g_ready)
        return true;
    const HMODULE k = LoadLibraryA("kernel32.dll");
    if (!k)
        return false;
#define MENULOG_GET(fn) k32.fn = reinterpret_cast<decltype(k32.fn)>(GetProcAddress(k, #fn))
    MENULOG_GET(CreateFileA);
    MENULOG_GET(WriteFile);
    MENULOG_GET(CloseHandle);
    MENULOG_GET(GetTempPathA);
    MENULOG_GET(GetLocalTime);
    MENULOG_GET(GetCurrentProcessId);
    MENULOG_GET(GetCurrentThreadId);
    MENULOG_GET(GetModuleFileNameA);
    MENULOG_GET(GetModuleHandleExA);
    MENULOG_GET(VirtualQuery);
    MENULOG_GET(AddVectoredExceptionHandler);
#undef MENULOG_GET
    g_ready = k32.CreateFileA && k32.WriteFile && k32.CloseHandle && k32.GetLocalTime &&
        k32.GetCurrentProcessId && k32.GetCurrentThreadId;
    return g_ready;
}

inline HANDLE OpenForAppend(const char* path) {
    return k32.CreateFileA(path, FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL, nullptr);
}

inline void PickPath() {
    static const char kPublic[] = "C:\\Users\\Public\\Documents\\menu_log.txt";
    if (const HANDLE h = OpenForAppend(kPublic); h != INVALID_HANDLE_VALUE) {
        k32.CloseHandle(h);
        strcpy_s(g_path, kPublic);
        return;
    }
    char dir[MAX_PATH];
    const DWORD n = k32.GetTempPathA ? k32.GetTempPathA(MAX_PATH, dir) : 0;
    if (n > 0 && n < MAX_PATH - 16)
        snprintf(g_path, sizeof g_path, "%smenu_log.txt", dir);
}

inline void Write(const char* fmt, ...) {
    if (!Resolve())
        return;
    if (!g_path[0])
        PickPath();
    if (!g_path[0])
        return;

    char line[1024];
    SYSTEMTIME t;
    k32.GetLocalTime(&t);
    int n = snprintf(line, sizeof line, "%02u:%02u:%02u.%03u [pid %lu tid %lu] ", t.wHour, t.wMinute, t.wSecond,
        t.wMilliseconds, k32.GetCurrentProcessId(), k32.GetCurrentThreadId());
    if (n < 0)
        n = 0;
    va_list ap;
    va_start(ap, fmt);
    const int m = vsnprintf(line + n, sizeof line - n - 2, fmt, ap);
    va_end(ap);
    const int avail = (int)sizeof line - n - 3;
    n += m < 0 ? 0 : (m < avail ? m : avail);
    line[n++] = '\r';
    line[n++] = '\n';

    const HANDLE h = OpenForAppend(g_path);
    if (h == INVALID_HANDLE_VALUE)
        return;
    DWORD written;
    k32.WriteFile(h, line, (DWORD)n, &written, nullptr);
    k32.CloseHandle(h);
}

inline const char* GameName() {
#if IsApex
    return "Apex";
#elif IsArc
    return "Arc";
#elif IsFortnite
    return "Fortnite";
#elif IsBO6
    return "BO6";
#elif IsBO7Int
    return "BO7Int";
#elif IsBO7Ext
    return "BO7Ext";
#elif IsMarvel
    return "Marvel";
#elif IsMW19
    return "MW19";
#elif IsMW3
    return "MW3";
#elif IsR6
    return "R6";
#elif IsRust
    return "Rust";
#elif IsValorant
    return "Valorant";
#elif IsPUBG
    return "PUBG";
#elif IsWarDogs
    return "WarDogs";
#else
    return "none";
#endif
}

inline bool InImage(uintptr_t a) { return a >= g_base && a < g_end; }

// Image range from the allocation that holds this module. The loader may wipe the PE headers, so don't parse them.
inline void FindImageRange() {
    g_base = reinterpret_cast<uintptr_t>(&__ImageBase);
    g_end = g_base;
    MEMORY_BASIC_INFORMATION mbi{};
    if (k32.VirtualQuery && k32.VirtualQuery(&__ImageBase, &mbi, sizeof mbi)) {
        void* const alloc = mbi.AllocationBase;
        for (uintptr_t p = g_base;
             k32.VirtualQuery(reinterpret_cast<void*>(p), &mbi, sizeof mbi) && mbi.AllocationBase == alloc && mbi.State != MEM_FREE;
             p = reinterpret_cast<uintptr_t>(mbi.BaseAddress) + mbi.RegionSize)
            g_end = reinterpret_cast<uintptr_t>(mbi.BaseAddress) + mbi.RegionSize;
    }
    if (g_end - g_base < 0x10000)
        g_end = g_base + 0x200000; // separate allocations per section: assume 2 MB
}

inline void DescribeAddress(uintptr_t a, char* buf, size_t size) {
    if (InImage(a)) {
        snprintf(buf, size, "menu+0x%llX", (unsigned long long)(a - g_base));
        return;
    }
    HMODULE mod = nullptr;
    char name[MAX_PATH];
    if (k32.GetModuleHandleExA && k32.GetModuleFileNameA &&
        k32.GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            reinterpret_cast<LPCSTR>(a), &mod) &&
        k32.GetModuleFileNameA(mod, name, sizeof name)) {
        const char* file = strrchr(name, '\\');
        snprintf(buf, size, "%s+0x%llX", file ? file + 1 : name, (unsigned long long)(a - reinterpret_cast<uintptr_t>(mod)));
        return;
    }
    snprintf(buf, size, "0x%llX (no module)", (unsigned long long)a);
}

inline const char* ExceptionName(DWORD code) {
    switch (code) {
    case EXCEPTION_ACCESS_VIOLATION:      return "access violation";
    case EXCEPTION_ILLEGAL_INSTRUCTION:   return "illegal instruction";
    case EXCEPTION_PRIV_INSTRUCTION:      return "privileged instruction";
    case EXCEPTION_INT_DIVIDE_BY_ZERO:    return "integer divide by zero";
    case EXCEPTION_INT_OVERFLOW:          return "integer overflow";
    case EXCEPTION_STACK_OVERFLOW:        return "stack overflow";
    case EXCEPTION_IN_PAGE_ERROR:         return "in-page error";
    case EXCEPTION_DATATYPE_MISALIGNMENT: return "misaligned data";
    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED: return "array bounds exceeded";
    case EXCEPTION_BREAKPOINT:            return "breakpoint";
    case 0xC0000374:                      return "heap corruption";
    case 0xE06D7363:                      return "C++ exception";
    default:                              return nullptr;
    }
}

// First-chance handler: only logs, never handles. Reports exceptions that fault inside the menu or that have
// menu addresses on the faulting thread's stack (the menu called the code that crashed); the host's own
// exceptions are ignored.
inline LONG CALLBACK OnException(EXCEPTION_POINTERS* ep) {
    const EXCEPTION_RECORD* er = ep->ExceptionRecord;
    const char* name = ExceptionName(er->ExceptionCode);
    if (!name || g_reported >= 8)
        return EXCEPTION_CONTINUE_SEARCH;
    if (InterlockedCompareExchange(&g_inHandler, 1, 0) != 0)
        return EXCEPTION_CONTINUE_SEARCH;

    const CONTEXT* ctx = ep->ContextRecord;
    const uintptr_t pc = reinterpret_cast<uintptr_t>(er->ExceptionAddress);
    uintptr_t found[16];
    int count = 0;
    const NT_TIB* tib = reinterpret_cast<const NT_TIB*>(NtCurrentTeb());
    const uintptr_t lo = reinterpret_cast<uintptr_t>(tib->StackLimit), hi = reinterpret_cast<uintptr_t>(tib->StackBase);
    if (ctx->Rsp >= lo && ctx->Rsp < hi) {
        const uintptr_t stop = hi - ctx->Rsp > 0x8000 ? ctx->Rsp + 0x8000 : hi;
        for (uintptr_t sp = ctx->Rsp; sp + 8 <= stop && count < 16; sp += 8) {
            const uintptr_t v = *reinterpret_cast<const uintptr_t*>(sp);
            if (InImage(v))
                found[count++] = v;
        }
    }

    if (InImage(pc) || count > 0) {
        InterlockedIncrement(&g_reported);
        char where[MAX_PATH + 32];
        DescribeAddress(pc, where, sizeof where);
        Write("!!! CRASH: %s (0x%08lX) at %s", name, er->ExceptionCode, where);
        if (er->ExceptionCode == EXCEPTION_ACCESS_VIOLATION && er->NumberParameters >= 2) {
            const ULONG_PTR kind = er->ExceptionInformation[0];
            Write("    while %s address 0x%llX", kind == 0 ? "reading" : kind == 1 ? "writing" : "executing",
                (unsigned long long)er->ExceptionInformation[1]);
        }
        Write("    rip=%llX rsp=%llX rax=%llX rcx=%llX rdx=%llX r8=%llX r9=%llX", ctx->Rip, ctx->Rsp, ctx->Rax,
            ctx->Rcx, ctx->Rdx, ctx->R8, ctx->R9);
        char list[16 * 24] = "";
        int len = 0;
        for (int i = 0; i < count && len < (int)sizeof list - 24; ++i)
            len += snprintf(list + len, sizeof list - len, " +0x%llX", (unsigned long long)(found[i] - g_base));
        Write("    menu addresses on the stack (newest first):%s", count ? list : " none");
    }
    InterlockedExchange(&g_inHandler, 0);
    return EXCEPTION_CONTINUE_SEARCH;
}

inline void OnLoad() {
    if (!Resolve())
        return;
    FindImageRange();
    char exe[MAX_PATH] = "?";
    if (k32.GetModuleFileNameA)
        k32.GetModuleFileNameA(nullptr, exe, MAX_PATH);
    Write("==== menu DLL loaded: game %s, built %s %s, image 0x%llX-0x%llX, host %s", GameName(), __DATE__, __TIME__,
        (unsigned long long)g_base, (unsigned long long)g_end, exe);
    // Armed here, not in InitMyMenu: this runs during the CRT's DLL_PROCESS_ATTACH, before main.cpp's DllMain
    // stores its function pointers into lpReserved. A fault in DllMain or in a later static initializer would
    // otherwise leave the log ending at the line above with no explanation.
    if (k32.AddVectoredExceptionHandler && k32.AddVectoredExceptionHandler(1, OnException))
        Write("crash logging on (covers DllMain and setup)");
}

inline void FrameHook(ImGuiContext* ctx, ImGuiContextHook* hook) {
    if (hook->Type == ImGuiContextHookType_NewFramePre) {
        const LONG n = InterlockedIncrement(&g_frames);
        if (n == 1)
            Write("frame 1: started (display %.0fx%.0f, font atlas %dx%d, %d fonts)", ctx->IO.DisplaySize.x,
                ctx->IO.DisplaySize.y, ctx->IO.Fonts->TexWidth, ctx->IO.Fonts->TexHeight, ctx->IO.Fonts->Fonts.Size);
        else if (n <= 3 || n == 100 || n == 1000 || n == 10000 || n % 30000 == 0)
            Write("frame %ld: started%s", n, g_menuDraws == 0 ? " (menu still hidden: RenderMyMenu not called yet)" : "");
    } else if (hook->Type == ImGuiContextHookType_RenderPost && g_frames <= 3) {
        Write("frame %ld: finished", (long)g_frames);
    }
}

// Called first thing in InitMyMenu (the ImGui context exists by then)
inline void OnInit(const void* device) {
    Write("InitMyMenu: start (device %p)", device);
    ImGuiContextHook hook;
    hook.Callback = FrameHook;
    hook.Type = ImGuiContextHookType_NewFramePre;
    ImGui::AddContextHook(ImGui::GetCurrentContext(), &hook);
    hook = ImGuiContextHook();
    hook.Callback = FrameHook;
    hook.Type = ImGuiContextHookType_RenderPost;
    ImGui::AddContextHook(ImGui::GetCurrentContext(), &hook);
}

// First few RenderMyMenu calls: the pointers the loader passed and a few fields every SETTINGS has.
// Garbage here (huge sizes, odd keys) means the DLL's SETTINGS layout does not match the cheat's.
template <typename S>
inline void LogMenuFrame(bool* show, S* s, CConfig* cfg) {
    const LONG n = InterlockedIncrement(&g_menuDraws);
    if (n > 3)
        return;
    if (!s) {
        Write("RenderMyMenu call %ld (frame %ld): Settings is NULL (ShowMenu %p, g_pConfig %p)", n, (long)g_frames,
            (void*)show, (void*)cfg);
        return;
    }
    char fonts[96] = "";
    if constexpr (requires { s->InstalledFonts.size(); s->AvailableFonts.size(); })
        snprintf(fonts, sizeof fonts, ", InstalledFonts %zu, AvailableFonts %zu", s->InstalledFonts.size(),
            s->AvailableFonts.size());
    Write("RenderMyMenu call %ld: ShowMenu %d, Settings %p (%zu bytes), g_pConfig %p, iConfig %d, oldiConfig %d, MenuKey 0x%X%s",
        n, show ? (int)*show : -1, (void*)s, sizeof(S), (void*)cfg, s->iConfig, s->oldiConfig, s->Configs.MenuKey, fonts);
}

} // namespace menulog

#define MENU_LOG(...) ::menulog::Write(__VA_ARGS__)
#define MENU_LOG_FRAME(show, settings, cfg) ::menulog::LogMenuFrame(show, settings, cfg)
#else
#define MENU_LOG(...) ((void)0)
#define MENU_LOG_FRAME(show, settings, cfg) ((void)0)
#endif
