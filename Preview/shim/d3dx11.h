#pragma once
// Preview-only stand-in for the legacy DirectX SDK (June 2010) d3dx11.h.
// Only covers what GUI.cpp uses. The image load always fails, so `users`
// stays null and the menu skips drawing it (every use is null-checked).
// This folder is first on the preview include path; the real DLL build never sees it.
#include <d3d11.h>

struct D3DX11_IMAGE_LOAD_INFO {};
struct ID3DX11ThreadPump;

inline HRESULT D3DX11CreateShaderResourceViewFromFileA(ID3D11Device*, LPCSTR, D3DX11_IMAGE_LOAD_INFO*,
    ID3DX11ThreadPump*, ID3D11ShaderResourceView** ppView, HRESULT* pHResult)
{
    if (ppView) *ppView = nullptr;
    if (pHResult) *pHResult = E_NOTIMPL;
    return E_NOTIMPL;
}
