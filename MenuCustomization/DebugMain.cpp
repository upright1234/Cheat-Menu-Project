#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <tchar.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#define IMGUI_DEFINE_MATH_OPERATORS
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_dx11.h"
#include "Imgui/imgui_impl_win32.h"

#include "CheatSettings/Config.h"
#include "GUI/GUI.h"

// Stubs de configuração apenas para modo standalone de debug
void InitPath(char* config_name) {}
void CreateConfig(char* config_name) {}
void Read() {}
void Save() {}
void ParseConfigs() {}

// API do menu implementada em main.cpp
bool InitializeMenu(HWND Window, ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pd3dDeviceContext, int* ResellerMenuColor);
void RenderMenu(bool* ShowMenu, int GameX, int GameY, int GameWidth, int GameHeight, SETTINGS* Settings, CConfig* g_pConfig, float mouse_wheel);
void EndRender();
void RenderDx11();

// DirectX 11 globals
static ID3D11Device*           g_pd3dDevice           = nullptr;
static ID3D11DeviceContext*    g_pd3dDeviceContext    = nullptr;
static IDXGISwapChain*         g_pSwapChain           = nullptr;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

// Estado do menu / janela
static bool  g_ShowMenu    = true;
static int   g_GameX       = 0;
static int   g_GameY       = 0;
static int   g_GameWidth   = 1280;
static int   g_GameHeight  = 800;
static float g_MouseWheel  = 0.f;

static SETTINGS g_Settings{};
static CConfig  g_Config{};

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Forward declarations
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool CreateDeviceD3D(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 0;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[] = {
    D3D_FEATURE_LEVEL_11_0,
    D3D_FEATURE_LEVEL_10_1,
    D3D_FEATURE_LEVEL_10_0,
    D3D_FEATURE_LEVEL_9_3,
    };

    if (D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            createDeviceFlags,
            featureLevelArray,
            2,
            D3D11_SDK_VERSION,
            &sd,
            &g_pSwapChain,
            &g_pd3dDevice,
            &featureLevel,
            &g_pd3dDeviceContext) != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    if (g_pSwapChain)
    {
        g_pSwapChain->Release();
        g_pSwapChain = nullptr;
    }
    if (g_mainRenderTargetView)
    {
        g_mainRenderTargetView->Release();
        g_mainRenderTargetView = nullptr;
    }
    if (g_pd3dDeviceContext)
    {
        g_pd3dDeviceContext->Release();
        g_pd3dDeviceContext = nullptr;
    }
    if (g_pd3dDevice)
    {
        g_pd3dDevice->Release();
        g_pd3dDevice = nullptr;
    }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView)
    {
        g_mainRenderTargetView->Release();
        g_mainRenderTargetView = nullptr;
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_GameWidth  = LOWORD(lParam);
        g_GameHeight = HIWORD(lParam);
        if (g_pd3dDevice != nullptr && g_pSwapChain != nullptr)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_GameWidth, g_GameHeight, DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
        }
        return 0;
    case WM_MOUSEWHEEL:
        g_MouseWheel += static_cast<float>(GET_WHEEL_DELTA_WPARAM(wParam)) / static_cast<float>(WHEEL_DELTA);
        return 0;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

int main()
{
    // Registrar classe da janela
    WNDCLASSEXW wc = {
        sizeof(wc),
        CS_CLASSDC,
        WndProc,
        0L,
        0L,
        0,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        L"DX11_Standalone_Menu",
        nullptr
    };
    ::RegisterClassExW(&wc);

    RECT rc = { 0, 0, g_GameWidth, g_GameHeight };
    HWND hwnd = ::CreateWindowW(
        wc.lpszClassName,
        L"Menu DX11 Standalone",
        WS_OVERLAPPEDWINDOW,
        100,
        100,
        rc.right - rc.left,
        rc.bottom - rc.top,
        nullptr,
        nullptr,
        wc.hInstance,
        nullptr);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Configuração básica de arquivos e config
    g_Config.InitPath     = &InitPath;
    g_Config.Read         = &Read;
    g_Config.Save         = &Save;
    g_Config.ParseConfigs = &ParseConfigs;
    g_Config.CreateConfig = &CreateConfig;

    InitPath(nullptr);
    ParseConfigs();
    Read();

    int resellerColor[4] = { 255, 255, 255, 255 };
    if (!InitializeMenu(hwnd, g_pd3dDevice, g_pd3dDeviceContext, resellerColor))
    {
        CleanupRenderTarget();
        CleanupDeviceD3D();
        ::DestroyWindow(hwnd);
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    while (msg.message != WM_QUIT)
    {
        if (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            continue;
        }

        // Toggle de menu (INSERT por padrão)
        if (GetAsyncKeyState(g_Settings.Configs.MenuKey) & 1)
            g_ShowMenu = !g_ShowMenu;

        // Atualizar dimensões do cliente
        RECT client;
        ::GetClientRect(hwnd, &client);
        g_GameX      = client.left;
        g_GameY      = client.top;
        g_GameWidth  = client.right - client.left;
        g_GameHeight = client.bottom - client.top;

        float clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };
        RenderMenu(&g_ShowMenu, g_GameX, g_GameY, g_GameWidth, g_GameHeight, &g_Settings, &g_Config, g_MouseWheel);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clearColor);
        ImGui::Render();
        EndRender();
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        g_MouseWheel = 0.f;
        RenderDx11();
        g_pSwapChain->Present(g_Settings.Configs.VSync ? 1 : 0, 0);
    }

    // Shutdown ImGui + DX11
    CleanupRenderTarget();
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    CleanupDeviceD3D();

    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}


