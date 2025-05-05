#include "GUI.h"
#include <fstream>
#include <d3dx11.h>
#include "imgui_setting.h"
#include "main.h"
#include "Font.h"

void InitMyMenu(ID3D11Device* g_pd3dDevice) {

	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.IniFilename = nullptr;
	io.LogFilename = nullptr;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	 
	ImGui::StyleColorsDark();


	ImGuiStyle* style = &ImGui::GetStyle();
	ImVec4* colors = style->Colors;
	colors[ImGuiCol_FrameBg] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.34f, 0.34f, 0.34f, 0.64f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.34f, 0.34f, 0.34f, 0.74f);
	colors[ImGuiCol_Button] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.34f, 0.34f, 0.34f, 0.64f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.34f, 0.34f, 0.34f, 0.74f);
	colors[ImGuiCol_Header] = ImColor(settings::general_color.Value.x, settings::general_color.Value.y, settings::general_color.Value.z, 0.54f);
	colors[ImGuiCol_HeaderHovered] = ImColor(settings::general_color.Value.x, settings::general_color.Value.y, settings::general_color.Value.z, 0.64f);
	colors[ImGuiCol_HeaderActive] = ImColor(settings::general_color.Value.x, settings::general_color.Value.y, settings::general_color.Value.z, 0.74f);

	ImGui::StyleColorsDark();

	Main_Font = io.Fonts->AddFontFromMemoryTTF(&MainFont, sizeof MainFont, 21.f, NULL, io.Fonts->GetGlyphRangesCyrillic());

    Font1 = io.Fonts->AddFontFromMemoryTTF(&Font, sizeof Font, 20.f, NULL, io.Fonts->GetGlyphRangesCyrillic());

    ico = io.Fonts->AddFontFromMemoryTTF(&icon, sizeof icon, 30.f, NULL, io.Fonts->GetGlyphRangesCyrillic());

    icons = io.Fonts->AddFontFromMemoryTTF(&Icon, sizeof Icon, 20.f, NULL, io.Fonts->GetGlyphRangesCyrillic());


	std::ifstream file("C:\\str.png");
	if (file.is_open())
	{
		file.close();
		D3DX11_IMAGE_LOAD_INFO info; ID3DX11ThreadPump* pump{ nullptr };
		D3DX11CreateShaderResourceViewFromFileA(g_pd3dDevice, "C:\\str.png", &info, pump, &users, 0);
	}
}

