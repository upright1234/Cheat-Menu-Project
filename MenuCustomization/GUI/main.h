#pragma once
#include "imgui_internal.h"
#include "imgui_setting.h"
#include <D3D11.h>


inline void CustomStyleColor()
{
    ImGuiStyle& s = ImGui::GetStyle();
    ImGuiContext& g = *GImGui;

    s.Colors[ImGuiCol_WindowBg] = ImLerp(s.Colors[ImGuiCol_WindowBg], ImColor(0.06f, 0.06f, 0.06f, 1.f), g.IO.DeltaTime * 5.f);
    settings::tabs::text = ImColor(255, 255, 255, 255);
    s.Colors[ImGuiCol_ChildBg] = ImLerp(s.Colors[ImGuiCol_ChildBg], ImColor(0.06f, 0.06f, 0.06f, 1.f), g.IO.DeltaTime * 5.f);
    s.Colors[ImGuiCol_Text] = ImLerp(s.Colors[ImGuiCol_Text], ImColor(1.f, 1.f, 1.f, 1.f), g.IO.DeltaTime * 5.f);


    s.WindowPadding = ImVec2(0, 0);
    s.ChildRounding = 4.f;
    s.WindowRounding = 5.f;
}

inline ID3D11ShaderResourceView* users = nullptr;
inline ID3D11ShaderResourceView* Logo_Texture = nullptr; // header logo, created from LogoData.h in InitMyMenu
inline ImFont* Main_Font = nullptr;
inline ImFont* Font1 = nullptr;
inline ImFont* Title_Font = nullptr; // Main_Font at a larger size, for menu headings
inline ImFont* ico = nullptr;
inline ImFont* icons = nullptr;
#define ALPHA    ( ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoBorder )
