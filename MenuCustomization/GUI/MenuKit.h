#pragma once
// Shared look for every game menu: window chrome (header + sidebar), auto-height panels and
// compact widgets. Header-only so no project changes are needed; include it inside a game's #if block.
//
//   static int page = 0;
//   kit::BeginShell(desc, page, ShowMenu);
//   switch (page) { case 0: kit::BeginColumns(); kit::BeginPanel("Aimbot"); ... kit::EndPanel(); ... }
//   kit::EndShell();
//
// Colors here match the ImGui style set in InitMyMenu (GUI.cpp): black / near-black base, blue-gray accent.
#include <Windows.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>

#include "../CheatSettings/Config.h"
#include "imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
#include "main.h"
#include "LogoData.h"
#include "DebugLog.h"

namespace kit {

// ---------------------------------------------------------------------------
// Layout + palette
// ---------------------------------------------------------------------------
constexpr float kMenuW = 900.f, kMenuH = 600.f;
constexpr float kHeaderH = 56.f, kSidebarW = 188.f, kGap = 12.f;
constexpr float kComboW = 150.f, kKeyW = 118.f;

constexpr ImU32 kColShell    = IM_COL32(0, 0, 0, 255);
constexpr ImU32 kColPanel    = IM_COL32(13, 13, 15, 255);
constexpr ImU32 kColWell     = IM_COL32(6, 6, 8, 255);
constexpr ImU32 kColLine     = IM_COL32(36, 36, 42, 255);
constexpr ImU32 kColHover    = IM_COL32(24, 24, 28, 255);
constexpr ImU32 kColActive   = IM_COL32(32, 34, 42, 255);
constexpr ImU32 kColAccent   = IM_COL32(86, 128, 190, 255);
constexpr ImU32 kColAccentHi = IM_COL32(104, 146, 206, 255);
constexpr ImU32 kColText     = IM_COL32(226, 232, 242, 255);
constexpr ImU32 kColTextMid  = IM_COL32(178, 188, 206, 255);
constexpr ImU32 kColDim      = IM_COL32(124, 136, 158, 255);
constexpr ImU32 kColOn       = IM_COL32(88, 196, 132, 255);
constexpr ImU32 kColOff      = IM_COL32(70, 78, 96, 255);

constexpr ImGuiColorEditFlags kColorFlags = ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_NoTooltip |
    ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar |
    ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop |
    ImGuiColorEditFlags_PickerHueBar;
constexpr ImGuiColorEditFlags kColorFlagsNoAlpha = (kColorFlags & ~(ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_AlphaBar)) |
    ImGuiColorEditFlags_NoAlpha;

inline const char* const kColorModes[] = { "Static", "By Team", "By Visibility" };

// Background opacity (percent) for the window, header, sidebar and panels; text and controls stay opaque.
// Not part of SETTINGS, so it resets to the default when the menu is reloaded.
inline int g_bgOpacity = 75;
inline ImU32 Fade(ImU32 col) {
    const ImU32 a = (ImU32)(((col >> IM_COL32_A_SHIFT) & 0xFF) * g_bgOpacity / 100);
    return (col & ~IM_COL32_A_MASK) | (a << IM_COL32_A_SHIFT);
}

// ---------------------------------------------------------------------------
// Small widgets. Rows put the label on the left and the control on the right edge of the panel.
// ---------------------------------------------------------------------------
inline float RightEdge() { return ImGui::GetWindowContentRegionMax().x; }

inline void Label(const char* text) {
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(text);
}

inline void Hint(const char* text) {
    ImGui::PushStyleColor(ImGuiCol_Text, kColDim);
    ImGui::TextWrapped("%s", text);
    ImGui::PopStyleColor();
}

inline void SubHeader(const char* text) {
    ImGui::Dummy(ImVec2(0.f, 2.f));
    ImGui::PushStyleColor(ImGuiCol_Text, kColDim);
    ImGui::TextUnformatted(text);
    ImGui::PopStyleColor();
}

inline bool ActionButton(const char* id, const char* text, ImVec2 size = ImVec2(0.f, 0.f), bool accent = false) {
    if (size.x <= 0.f) size.x = ImGui::GetContentRegionAvail().x;
    if (size.y <= 0.f) size.y = ImGui::GetFrameHeight();
    const ImVec2 p = ImGui::GetCursorScreenPos();
    const bool pressed = ImGui::InvisibleButton(id, size);
    const bool hovered = ImGui::IsItemHovered();
    const bool held = ImGui::IsItemActive();

    ImDrawList* dl = ImGui::GetWindowDrawList();
    const ImU32 bg = accent ? (hovered ? kColAccentHi : kColAccent)
                            : ImGui::GetColorU32(held ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
    dl->AddRectFilled(p, p + size, bg, 6.f);
    if (!accent)
        dl->AddRect(p, p + size, kColLine, 6.f);
    const ImVec2 ts = ImGui::CalcTextSize(text, nullptr, true);
    dl->AddText(p + (size - ts) * 0.5f, kColText, text, ImGui::FindRenderedTextEnd(text));
    return pressed;
}

// The custom slider (imgui_widgets.cpp) draws its label/value ~6px above its layout slot, so give it room
inline bool SliderF(const char* label, float* v, float mn, float mx, const char* fmt = "%.1f") {
    ImGui::Dummy(ImVec2(0.f, 4.f));
    return ImGui::SliderFloat(label, v, mn, mx, fmt);
}

inline bool SliderI(const char* label, int* v, int mn, int mx, const char* fmt = "%d") {
    ImGui::Dummy(ImVec2(0.f, 4.f));
    return ImGui::SliderInt(label, v, mn, mx, fmt);
}

// The custom ColorEdit4 draws a fixed 22px swatch 60px left of the cursor; this puts it with its right edge at `right`.
// yOffset centers it on a checkbox/label row; pass 0 for extra swatches chained on the same row.
constexpr float kSwatch = 22.f;
inline bool Swatch(const char* id, float* col, float right, ImGuiColorEditFlags flags = kColorFlags, float yOffset = 1.f) {
    ImGui::SameLine(right - kSwatch + 60.f);
    if (yOffset != 0.f)
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + yOffset);
    return ImGui::ColorEdit4(id, col, flags);
}

inline void ColorRow(const char* label, float* col, ImGuiColorEditFlags flags = kColorFlags) {
    ImGui::PushID(label);
    Label(label);
    Swatch("##c", col, RightEdge(), flags);
    ImGui::PopID();
}

inline bool ToggleColor(const char* label, bool* v, float* col, ImGuiColorEditFlags flags = kColorFlags) {
    ImGui::PushID(label);
    const bool changed = ImGui::Checkbox(label, v);
    Swatch("##c", col, RightEdge(), flags);
    ImGui::PopID();
    return changed;
}

inline bool ComboRow(const char* label, int* v, const char* const items[], int count) {
    ImGui::PushID(label);
    Label(label);
    ImGui::SameLine(RightEdge() - kComboW);
    ImGui::SetNextItemWidth(kComboW + 60.f); // custom BeginCombo subtracts 60px
    const bool changed = ImGui::Combo("##v", v, items, count, count);
    ImGui::PopID();
    return changed;
}

// "A\0B\0C\0" style item list
inline bool ComboRow(const char* label, int* v, const char* itemsZeroSeparated) {
    ImGui::PushID(label);
    Label(label);
    ImGui::SameLine(RightEdge() - kComboW);
    ImGui::SetNextItemWidth(kComboW + 60.f);
    const bool changed = ImGui::Combo("##v", v, itemsZeroSeparated);
    ImGui::PopID();
    return changed;
}

inline bool ComboRow(const char* label, int* v, const std::vector<std::string>& items) {
    ImGui::PushID(label);
    Label(label);
    ImGui::SameLine(RightEdge() - kComboW);
    ImGui::SetNextItemWidth(kComboW + 60.f);
    const int count = (int)items.size();
    const char* preview = (*v >= 0 && *v < count) ? items[*v].c_str() : "None";
    bool changed = false;
    if (ImGui::BeginCombo("##v", preview)) {
        for (int i = 0; i < count; ++i) {
            const bool selected = *v == i;
            if (ImGui::Selectable(items[i].c_str(), selected)) {
                *v = i;
                changed = true;
            }
            if (selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::PopID();
    return changed;
}

// Multi-select over a bool array, summary shown in the preview ("Head, Chest" / "3 selected")
inline void MultiComboRow(const char* label, bool* values, const char* const items[], int count) {
    ImGui::PushID(label);
    Label(label);
    ImGui::SameLine(RightEdge() - kComboW);
    ImGui::SetNextItemWidth(kComboW + 60.f);
    char preview[64] = "None";
    int selected = 0;
    for (int i = 0; i < count; ++i)
        if (values[i]) ++selected;
    if (selected == 1) {
        for (int i = 0; i < count; ++i)
            if (values[i]) snprintf(preview, sizeof preview, "%s", items[i]);
    } else if (selected > 1) {
        snprintf(preview, sizeof preview, "%d selected", selected);
    }
    if (ImGui::BeginCombo("##v", preview)) {
        for (int i = 0; i < count; ++i)
            if (ImGui::Selectable(items[i], values[i], ImGuiSelectableFlags_DontClosePopups))
                values[i] = !values[i];
        ImGui::EndCombo();
    }
    ImGui::PopID();
}

inline void MultiComboRow(const char* label, bool* values, const std::vector<std::string>& items) {
    std::vector<const char*> ptrs;
    ptrs.reserve(items.size());
    for (const auto& s : items) ptrs.push_back(s.c_str());
    MultiComboRow(label, values, ptrs.data(), (int)ptrs.size());
}

// Key binding: click, then press a key / mouse button. Esc clears the bind. Values are Win32 VK codes.
inline ImGuiID g_listenId = 0;
inline bool g_listenArmed = false;

// MapVirtualKeyA / GetKeyNameTextA are looked up at runtime so the DLL imports exactly the same functions as the
// stock template (the loader maps the DLL itself). Plain globals, not function-local statics: those would need
// thread-safe init, which uses TLS.
inline decltype(&::MapVirtualKeyA) g_mapVirtualKey = nullptr;
inline decltype(&::GetKeyNameTextA) g_getKeyNameText = nullptr;

inline void KeyName(int vk, char* buf, int size) {
    switch (vk) {
    case 0:           snprintf(buf, size, "None");    return;
    case VK_LBUTTON:  snprintf(buf, size, "Mouse 1"); return;
    case VK_RBUTTON:  snprintf(buf, size, "Mouse 2"); return;
    case VK_MBUTTON:  snprintf(buf, size, "Mouse 3"); return;
    case VK_XBUTTON1: snprintf(buf, size, "Mouse 4"); return;
    case VK_XBUTTON2: snprintf(buf, size, "Mouse 5"); return;
    }
    if (!g_mapVirtualKey || !g_getKeyNameText) {
        if (const HMODULE user32 = LoadLibraryA("user32.dll")) {
            g_mapVirtualKey = reinterpret_cast<decltype(g_mapVirtualKey)>(GetProcAddress(user32, "MapVirtualKeyA"));
            g_getKeyNameText = reinterpret_cast<decltype(g_getKeyNameText)>(GetProcAddress(user32, "GetKeyNameTextA"));
        }
    }
    LONG lparam = g_mapVirtualKey ? (LONG)(g_mapVirtualKey((UINT)vk, MAPVK_VK_TO_VSC) & 0xFF) << 16 : 0;
    switch (vk) {
    case VK_INSERT: case VK_DELETE: case VK_HOME: case VK_END: case VK_PRIOR: case VK_NEXT:
    case VK_LEFT: case VK_RIGHT: case VK_UP: case VK_DOWN: case VK_DIVIDE: case VK_NUMLOCK:
    case VK_RCONTROL: case VK_RMENU:
        if (lparam)
            lparam |= 1 << 24; // extended key, otherwise these report as numpad keys
        break;
    }
    if (lparam == 0 || !g_getKeyNameText || g_getKeyNameText(lparam, buf, size) <= 0)
        snprintf(buf, size, "Key 0x%02X", vk);
}

// Right-aligned bind button on the current row
inline bool KeyButton(int* key) {
    const ImGuiID id = ImGui::GetID("##key");
    const bool listening = g_listenId == id;
    ImGui::SameLine(RightEdge() - kKeyW);
    char name[64];
    if (listening) snprintf(name, sizeof name, "%s", "Press a key...");
    else KeyName(*key, name, sizeof name);
    if (ActionButton("##key", name, ImVec2(kKeyW, 0.f), listening)) {
        g_listenId = id;
        g_listenArmed = false;
    }

    if (g_listenId != id)
        return false;
    int pressed = 0;
    for (int vk = 1; vk < 255 && !pressed; ++vk)
        if (GetAsyncKeyState(vk) & 0x8000)
            pressed = vk;
    if (!g_listenArmed) {
        g_listenArmed = pressed == 0; // wait until the click that started listening is released
        return false;
    }
    if (!pressed)
        return false;
    *key = pressed == VK_ESCAPE ? 0 : pressed;
    g_listenId = 0;
    return true;
}

inline bool KeyRow(const char* label, int* key) {
    ImGui::PushID(label);
    Label(label);
    const bool changed = KeyButton(key);
    ImGui::PopID();
    return changed;
}

// Checkbox + bind on one row (replaces ImGui::BinderBox)
inline bool ToggleKey(const char* label, bool* v, int* key) {
    ImGui::PushID(label);
    const bool changed = ImGui::Checkbox(label, v);
    KeyButton(key);
    ImGui::PopID();
    return changed;
}

// Checkbox with an amber "(Risky)" tag right after its label (higher detection risk)
constexpr ImU32 kColWarn = IM_COL32(232, 168, 64, 255);

inline bool RiskyCheckbox(const char* label, bool* v) {
    const bool changed = ImGui::Checkbox(label, v);
    // The custom Checkbox draws its label at +35px, vertically centred on 15px
    const ImVec2 min = ImGui::GetItemRectMin();
    const ImVec2 ls = ImGui::CalcTextSize(label, nullptr, true);
    const ImVec2 tagPos(min.x + 35.f + ls.x + 6.f, min.y + 15.f - ls.y * 0.5f);
    ImGui::GetWindowDrawList()->AddText(tagPos, kColWarn, "(Risky)");
    if (ImGui::IsMouseHoveringRect(tagPos, tagPos + ImGui::CalcTextSize("(Risky)")))
        ImGui::SetTooltip("Higher detection risk");
    return changed;
}

// RiskyCheckbox with a gear button at the right edge that opens a settings popup.
//   if (BeginRiskyToggle("Memory Aim", &v, "Controller Binds")) { ...popup rows...; EndRiskyToggle(); }
inline bool BeginRiskyToggle(const char* label, bool* v, const char* popupTitle) {
    ImGui::PushID(label);
    RiskyCheckbox(label, v);
    const float sz = ImGui::GetFrameHeight();
    ImDrawList* dl = ImGui::GetWindowDrawList();

    // Gear button at the right edge of the checkbox row
    ImGui::SameLine(RightEdge() - sz);
    const ImVec2 g = ImGui::GetCursorScreenPos();
    if (ImGui::InvisibleButton("##gear", ImVec2(sz, sz)))
        ImGui::OpenPopup("##settings");
    const bool hovered = ImGui::IsItemHovered();
    if (hovered)
        ImGui::SetTooltip("%s", popupTitle);
    const ImVec2 c = g + ImVec2(sz * 0.5f, sz * 0.5f + 2.f);
    const ImU32 gearCol = hovered ? kColText : kColDim;
    if (hovered)
        dl->AddRectFilled(g, g + ImVec2(sz, sz), kColHover, 6.f);
    for (int i = 0; i < 8; ++i) {
        const float a = i * IM_PI / 4.f;
        dl->AddLine(c + ImVec2(cosf(a), sinf(a)) * 5.f, c + ImVec2(cosf(a), sinf(a)) * 8.f, gearCol, 2.5f);
    }
    dl->AddCircle(c, 5.f, gearCol, 12, 2.f);

    ImGui::SetNextWindowPos(g + ImVec2(sz, sz + 4.f), ImGuiCond_Appearing, ImVec2(1.f, 0.f));
    ImGui::SetNextWindowSize(ImVec2(320.f, 0.f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(14.f, 12.f));
    const bool open = ImGui::BeginPopup("##settings");
    ImGui::PopStyleVar();
    if (!open) {
        ImGui::PopID();
        return false;
    }
    ImGui::PushStyleColor(ImGuiCol_Text, kColTextMid);
    ImGui::TextUnformatted(popupTitle);
    ImGui::PopStyleColor();
    ImGui::Separator();
    return true;
}

inline void EndRiskyToggle() {
    ImGui::EndPopup();
    ImGui::PopID();
}

// Shared by every ESP element that has ColorType / StaticColor / VisibleColor / NonVisibleColor / TeamColorAlpha
template <typename T>
inline void ColorMode(T& s) {
    ImGui::PushID(&s);
    ComboRow("Color Mode", &s.ColorType, kColorModes, IM_ARRAYSIZE(kColorModes));
    switch (s.ColorType) {
    case 0: ColorRow("Color", s.StaticColor); break;
    case 1: SliderI("Team Color Alpha", &s.TeamColorAlpha, 0, 255); break;
    case 2:
        ColorRow("Visible", s.VisibleColor);
        ColorRow("Not Visible", s.NonVisibleColor);
        break;
    }
    ImGui::PopID();
}

// Pill-style tab strip spanning the panel width
inline void Segmented(const char* id, int* v, const char* const items[], int count) {
    ImGui::PushID(id);
    const float w = ImGui::GetContentRegionAvail().x, h = 30.f;
    const ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList* dl = ImGui::GetWindowDrawList();
    dl->AddRectFilled(p, p + ImVec2(w, h), kColWell, 7.f);
    const float segW = w / count;
    for (int i = 0; i < count; ++i) {
        const ImVec2 a(p.x + segW * i, p.y), b(a.x + segW, p.y + h);
        ImGui::SetCursorScreenPos(a);
        ImGui::PushID(i);
        if (ImGui::InvisibleButton("##seg", ImVec2(segW, h)))
            *v = i;
        const bool hovered = ImGui::IsItemHovered();
        ImGui::PopID();
        const bool selected = *v == i;
        if (selected)
            dl->AddRectFilled(a + ImVec2(3.f, 3.f), b - ImVec2(3.f, 3.f), kColActive, 5.f);
        const ImVec2 ts = ImGui::CalcTextSize(items[i]);
        dl->PushClipRect(a, b, true);
        dl->AddText(ImVec2(a.x + (segW - ts.x) * 0.5f, a.y + (h - ts.y) * 0.5f),
            selected ? kColText : hovered ? kColTextMid : kColDim, items[i]);
        dl->PopClipRect();
    }
    ImGui::SetCursorScreenPos(p);
    ImGui::Dummy(ImVec2(w, h));
    ImGui::PopID();
}

inline void Segmented(const char* id, int* v, const std::vector<std::string>& items) {
    std::vector<const char*> ptrs;
    ptrs.reserve(items.size());
    for (const auto& s : items) ptrs.push_back(s.c_str());
    if (!ptrs.empty())
        Segmented(id, v, ptrs.data(), (int)ptrs.size());
}

// ---------------------------------------------------------------------------
// Panels: bordered cards that size to their content (height measured last frame)
// ---------------------------------------------------------------------------
inline float g_colW = 0.f;
inline ImGuiID g_panelIds[8];
inline int g_panelDepth = 0;

inline void BeginColumns() {
    g_colW = (float)(int)((ImGui::GetContentRegionAvail().x - kGap) * 0.5f);
    ImGui::BeginGroup();
}
inline void NextColumn() {
    ImGui::EndGroup();
    ImGui::SameLine(0.f, kGap);
    ImGui::BeginGroup();
}
inline void EndColumns() { ImGui::EndGroup(); }

// width 0 = current column width; pass ImGui::GetContentRegionAvail().x for a full-width panel
inline void BeginPanel(const char* title, float width = 0.f) {
    const ImGuiID id = ImGui::GetID(title);
    const float h = ImGui::GetStateStorage()->GetFloat(id, 80.f);

    ImGui::PushStyleColor(ImGuiCol_ChildBg, Fade(kColPanel));
    ImGui::PushStyleColor(ImGuiCol_Border, kColLine);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(14.f, 12.f));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 8.f);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.f);
    ImGui::BeginChild(id, ImVec2(width > 0.f ? width : g_colW, h), true,
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    g_panelIds[g_panelDepth++] = id;

    ImGui::PushStyleColor(ImGuiCol_Text, kColTextMid);
    ImGui::TextUnformatted(title, ImGui::FindRenderedTextEnd(title));
    ImGui::PopStyleColor();
    const ImVec2 p = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x, p.y - 3.f),
        ImVec2(p.x + ImGui::GetContentRegionAvail().x, p.y - 3.f), kColLine);
    ImGui::Dummy(ImVec2(0.f, 2.f));
}

inline void EndPanel() {
    const ImGuiStyle& style = ImGui::GetStyle();
    // A panel scrolled fully out of view skips its items; keep last frame's height instead of measuring nothing
    const bool skipped = ImGui::GetCurrentWindow()->SkipItems;
    const float h = ImGui::GetCursorPosY() - style.ItemSpacing.y + style.WindowPadding.y;
    const ImGuiID id = g_panelIds[--g_panelDepth];
    ImGui::EndChild();
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(2);
    if (!skipped)
        ImGui::GetStateStorage()->SetFloat(id, h);
    ImGui::Dummy(ImVec2(0.f, kGap - style.ItemSpacing.y));
}

// ---------------------------------------------------------------------------
// Config helpers (every SETTINGS struct has iConfig / oldiConfig)
//
// CConfig is owned by the loader and its STL members come from the loader's build, so touch it
// the way the stock template does: list from szConfigs ('\0'-separated, the same string the stock
// ImGui::Combo reads), only index vecConfigs when the user clicks, and never read it every frame.
// ---------------------------------------------------------------------------
template <typename S>
inline void SwitchConfig(S* s, CConfig* cfg, int index) {
    if (index == s->oldiConfig)
        return;
    s->iConfig = index;
    if (s->oldiConfig >= 0) {
        cfg->InitPath(&cfg->vecConfigs[s->oldiConfig][0u]);
        cfg->Save();
    }
    s->oldiConfig = s->iConfig;
    cfg->InitPath(&cfg->vecConfigs[s->iConfig][0u]);
    cfg->Read();
}

// Standard "Configs" page: preset list with save/reload + create
template <typename S>
inline void ConfigsPage(S* s, CConfig* cfg, char* nameBuf, int nameBufSize) {
    if (!cfg) {
        Hint("Config system unavailable.");
        return;
    }
    int count = 0;
    const bool valid = s->iConfig >= 0;

    BeginColumns();
    BeginPanel("Presets");
    {
        for (const char* name = cfg->szConfigs.c_str(); *name; name += strlen(name) + 1, ++count) {
            ImGui::PushID(count);
            if (ImGui::Selectable(name, s->iConfig == count, 0, ImVec2(0.f, 26.f)))
                SwitchConfig(s, cfg, count);
            ImGui::PopID();
        }
        if (count == 0)
            Hint("No presets found yet. Create one on the right.");
        ImGui::Dummy(ImVec2(0.f, 2.f));
        const float half = (ImGui::GetContentRegionAvail().x - 8.f) * 0.5f;
        if (ActionButton("##save", "Save", ImVec2(half, 30.f), true) && valid && s->iConfig < count) {
            cfg->InitPath(&cfg->vecConfigs[s->iConfig][0u]);
            cfg->Save();
        }
        ImGui::SameLine(0.f, 8.f);
        if (ActionButton("##reload", "Reload", ImVec2(half, 30.f)) && valid && s->iConfig < count) {
            cfg->InitPath(&cfg->vecConfigs[s->iConfig][0u]);
            cfg->Read();
        }
        Hint("Switching presets saves the current one first.");
    }
    EndPanel();

    NextColumn();
    BeginPanel("New Preset");
    {
        Label("Name");
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::InputText("##cfgname", nameBuf, (size_t)nameBufSize);
        if (ActionButton("##create", "Create Preset", ImVec2(0.f, 30.f)))
            cfg->CreateConfig(nameBuf);
    }
    EndPanel();

    BeginPanel("Appearance");
    {
        SliderI("Background Opacity", &g_bgOpacity, 30, 100, "%d%%");
        Hint("Lower values let the game show through the menu. Resets when the menu reloads.");
    }
    EndPanel();
    EndColumns();
}

// Install a system font + apply one font to every label. applyAll(int fontIndex) sets the game's FontStyle fields.
template <typename S, typename ApplyFn>
inline void FontsPanel(S* s, ApplyFn applyAll) {
    BeginPanel("Fonts");
    static int systemFont = 0;
    static int fontForAll = 0;
    SubHeader("Install a system font");
    ComboRow("System Font", &systemFont, s->AvailableFonts);
    if (ActionButton("##install", "Install Font")) {
        if (systemFont >= 0 && systemFont < (int)s->AvailableFonts.size()) {
            const std::string& name = s->AvailableFonts[systemFont];
            if (std::find(s->InstalledFonts.begin(), s->InstalledFonts.end(), name) == s->InstalledFonts.end())
                s->InstalledFonts.push_back(name);
        }
    }
    SubHeader("Use one font for every label");
    ComboRow("Font", &fontForAll, s->InstalledFonts);
    if (ActionButton("##apply", "Apply To All Labels"))
        applyAll(fontForAll);
    char installed[48];
    snprintf(installed, sizeof installed, "%d fonts installed", (int)s->InstalledFonts.size());
    Hint(installed);
    EndPanel();
}

// ---------------------------------------------------------------------------
// Chrome: header + sidebar + page area
// ---------------------------------------------------------------------------
struct NavItem { const char* name; const char* desc; int section; };
struct Chip { const char* label; bool* value; };

struct ShellDesc {
    const char* brand = "MENU";                 // header title
    const char* tagline = nullptr;              // optional dim text after the title
    const char* const* sections = nullptr;      // sidebar group labels, indexed by NavItem::section
    const NavItem* pages = nullptr;
    int pageCount = 0;
    const Chip* chips = nullptr;                // quick toggles in the header (optional)
    int chipCount = 0;
    const char* footer = nullptr;               // optional first footer line
    int menuKey = VK_INSERT;                    // shown as "<key> to hide"
};

inline float ChipWidth(const char* label) { return ImGui::CalcTextSize(label).x + 32.f; }

inline void DrawChip(const char* label, bool* v, ImVec2 pos) {
    const ImVec2 size(ChipWidth(label), 26.f);
    ImGui::SetCursorScreenPos(pos);
    ImGui::PushID(label);
    if (ImGui::InvisibleButton("##chip", size))
        *v = !*v;
    const bool hovered = ImGui::IsItemHovered();
    ImGui::PopID();
    if (hovered)
        ImGui::SetTooltip("Click to toggle %s", label);

    ImDrawList* dl = ImGui::GetWindowDrawList();
    dl->AddRectFilled(pos, pos + size, hovered ? kColHover : kColPanel, 13.f);
    dl->AddRect(pos, pos + size, *v ? IM_COL32(52, 84, 70, 255) : kColLine, 13.f);
    dl->AddCircleFilled(pos + ImVec2(14.f, 13.f), 4.f, *v ? kColOn : kColOff);
    const float th = ImGui::CalcTextSize(label).y;
    dl->AddText(pos + ImVec2(24.f, (size.y - th) * 0.5f), *v ? kColText : kColDim, label);
}

inline ImFont* TitleFont() { return Title_Font ? Title_Font : ImGui::GetFont(); }

inline void DrawHeader(const ShellDesc& d, ImVec2 o, bool* ShowMenu) {
    ImDrawList* dl = ImGui::GetWindowDrawList();

    // Logo (embedded texture, drawn 1:1 at whole pixels so it stays crisp) + title
    float textX = 54.f;
    if (Logo_Texture) {
        const ImVec2 logoMin(IM_FLOOR(o.x + 14.f), IM_FLOOR(o.y + (kHeaderH - kLogoHeight) * 0.5f));
        dl->AddImage((ImTextureID)Logo_Texture, logoMin, logoMin + ImVec2((float)kLogoWidth, (float)kLogoHeight));
        textX = 14.f + kLogoWidth + 10.f;
    } else {
        dl->AddRectFilled(o + ImVec2(18.f, 16.f), o + ImVec2(42.f, 40.f), kColAccent, 6.f);
        dl->AddRectFilled(o + ImVec2(25.f, 23.f), o + ImVec2(35.f, 33.f), kColShell, 2.f);
    }
    ImFont* titleFont = TitleFont();
    const float titleSize = titleFont->FontSize;
    dl->AddText(titleFont, titleSize, o + ImVec2(textX, (kHeaderH - titleSize) * 0.5f), kColText, d.brand);
    const float titleW = titleFont->CalcTextSizeA(titleSize, FLT_MAX, 0.f, d.brand).x;
    if (d.tagline)
        dl->AddText(o + ImVec2(textX + titleW + 10.f, (kHeaderH - ImGui::GetFontSize()) * 0.5f + 1.f), kColDim, d.tagline);

    // Close button (hides the menu)
    const ImVec2 closePos = o + ImVec2(kMenuW - 18.f - 26.f, (kHeaderH - 26.f) * 0.5f);
    ImGui::SetCursorScreenPos(closePos);
    if (ImGui::InvisibleButton("##close", ImVec2(26.f, 26.f)))
        *ShowMenu = false;
    const bool closeHovered = ImGui::IsItemHovered();
    if (closeHovered)
        dl->AddRectFilled(closePos, closePos + ImVec2(26.f, 26.f), kColHover, 6.f);
    const ImU32 xCol = closeHovered ? kColText : kColDim;
    dl->AddLine(closePos + ImVec2(8.f, 8.f), closePos + ImVec2(18.f, 18.f), xCol, 1.5f);
    dl->AddLine(closePos + ImVec2(18.f, 8.f), closePos + ImVec2(8.f, 18.f), xCol, 1.5f);

    // Quick toggles, right-aligned before the close button
    float x = closePos.x - 12.f;
    for (int i = d.chipCount - 1; i >= 0; --i) {
        if (!d.chips[i].value)
            continue;
        x -= ChipWidth(d.chips[i].label);
        DrawChip(d.chips[i].label, d.chips[i].value, ImVec2(x, o.y + (kHeaderH - 26.f) * 0.5f));
        x -= 8.f;
    }
}

inline void DrawSidebar(const ShellDesc& d, ImVec2 o, int& page) {
    ImDrawList* dl = ImGui::GetWindowDrawList();
    const float fs = ImGui::GetFontSize();
    float y = o.y + kHeaderH + 14.f;
    int lastSection = -1;
    float groupTop = 0.f;

    // Category headers (bright, accent marker, divider) with their pages indented under a guide line
    const float guideX = o.x + 23.f;
    const float itemX = o.x + 32.f, itemR = o.x + kSidebarW - 10.f, itemH = 30.f;

    for (int i = 0; i < d.pageCount; ++i) {
        if (d.sections && d.pages[i].section != lastSection) {
            if (lastSection != -1)
                y += 12.f;
            lastSection = d.pages[i].section;
            const char* name = d.sections[lastSection];
            dl->AddRectFilled(ImVec2(o.x + 16.f, y + 2.f), ImVec2(o.x + 19.f, y + fs - 2.f), kColAccent, 1.5f);
            dl->AddText(ImVec2(o.x + 26.f, y), kColText, name);
            const float lineX = o.x + 26.f + ImGui::CalcTextSize(name).x + 8.f;
            dl->AddLine(ImVec2(lineX, y + fs * 0.5f), ImVec2(itemR, y + fs * 0.5f), kColLine);
            y += fs + 8.f;
            groupTop = y;
        }

        const ImVec2 a(itemX, y), b(itemR, y + itemH);
        ImGui::SetCursorScreenPos(a);
        ImGui::PushID(i);
        if (ImGui::InvisibleButton("##nav", b - a) && page != i) {
            page = i;
            g_listenId = 0; // drop a pending key bind from the previous page
        }
        const bool hovered = ImGui::IsItemHovered();
        ImGui::PopID();

        const bool active = page == i;
        if (active || hovered)
            dl->AddRectFilled(a, b, active ? kColActive : kColHover, 6.f);
        // Branch tick from the guide line; the active page gets an accent tick
        dl->AddLine(ImVec2(guideX, a.y + itemH * 0.5f), ImVec2(a.x - 2.f, a.y + itemH * 0.5f), active ? kColAccent : kColLine, active ? 2.f : 1.f);
        dl->AddText(ImVec2(a.x + 10.f, a.y + (itemH - fs) * 0.5f),
            active ? kColText : hovered ? kColTextMid : kColDim, d.pages[i].name);
        y += itemH + 2.f;

        const bool groupEnds = i + 1 == d.pageCount || (d.sections && d.pages[i + 1].section != lastSection);
        if (d.sections && groupEnds)
            dl->AddLine(ImVec2(guideX, groupTop), ImVec2(guideX, y - 2.f - itemH * 0.5f), kColLine);
    }

    // Footer: preset + how to hide
    const float fy = o.y + kMenuH - 58.f;
    dl->AddLine(ImVec2(o.x + 14.f, fy), ImVec2(o.x + kSidebarW - 14.f, fy), kColLine);
    dl->PushClipRect(ImVec2(o.x, fy), ImVec2(o.x + kSidebarW - 12.f, o.y + kMenuH), true);
    if (d.footer)
        dl->AddText(ImVec2(o.x + 18.f, fy + 10.f), kColTextMid, d.footer);
    char key[48], hint[80];
    KeyName(d.menuKey, key, sizeof key);
    snprintf(hint, sizeof hint, "%s to hide", key);
    dl->AddText(ImVec2(o.x + 18.f, fy + 12.f + fs), kColDim, hint);
    dl->PopClipRect();
}

// Opens the menu window, draws chrome + page title and begins the scrolling page body.
inline void BeginShell(const ShellDesc& d, int& page, bool* ShowMenu) {
    if (page < 0 || page >= d.pageCount)
        page = 0;

    ImGui::SetNextWindowPos(ImVec2(200.f, 140.f), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(kMenuW, kMenuH), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(g_bgOpacity / 100.f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.f);
    ImGui::Begin("##custom_menu", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    ImGui::PopStyleVar(3);

    const ImVec2 o = ImGui::GetWindowPos();
    ImDrawList* dl = ImGui::GetWindowDrawList();

    // Shell: header strip + sidebar, inset 1px so the window border stays visible
    // (the header/sidebar tint is drawn over the already-translucent window background, so keep it light)
    const ImU32 shell = IM_COL32(0, 0, 0, 110);
    dl->AddRectFilled(o + ImVec2(1.f, 1.f), o + ImVec2(kMenuW - 1.f, kHeaderH), Fade(shell), 10.f, ImDrawFlags_RoundCornersTop);
    dl->AddRectFilled(o + ImVec2(1.f, kHeaderH), o + ImVec2(kSidebarW, kMenuH - 1.f), Fade(shell), 10.f, ImDrawFlags_RoundCornersBottomLeft);
    dl->AddLine(o + ImVec2(1.f, kHeaderH), o + ImVec2(kMenuW - 1.f, kHeaderH), kColLine);
    dl->AddLine(o + ImVec2(kSidebarW, kHeaderH), o + ImVec2(kSidebarW, kMenuH - 1.f), kColLine);

    DrawHeader(d, o, ShowMenu);
    DrawSidebar(d, o, page);

    // Page title + description
    const float contentX = kSidebarW + 18.f;
    ImFont* titleFont = TitleFont();
    const NavItem& item = d.pages[page];
    dl->AddText(titleFont, titleFont->FontSize, o + ImVec2(contentX, kHeaderH + 14.f), kColText, item.name);
    if (item.desc) {
        const float titleW = titleFont->CalcTextSizeA(titleFont->FontSize, FLT_MAX, 0.f, item.name).x;
        dl->AddText(o + ImVec2(contentX + titleW + 12.f, kHeaderH + 14.f + (titleFont->FontSize - ImGui::GetFontSize()) * 0.5f + 1.f),
            kColDim, item.desc);
    }

    // Page body (scrolls when taller than the window)
    // Pushing the page ID first gives each page its own child window, so scroll position is per page
    ImGui::SetCursorPos(ImVec2(contentX, kHeaderH + 48.f));
    ImGui::PushID(page);
    ImGui::BeginChild("##page", ImVec2(kMenuW - contentX - 16.f, kMenuH - kHeaderH - 60.f), false);
}

inline void EndShell() {
    ImGui::EndChild();
    ImGui::PopID();
    ImGui::End();
}

} // namespace kit
