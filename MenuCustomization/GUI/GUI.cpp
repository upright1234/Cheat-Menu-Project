#include "GUI.h"
#include "icon_font.h"
#include <fstream>
#include <d3dx11.h>

void InitMyMenu(ID3D11Device* g_pd3dDevice) {

	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.IniFilename = nullptr;
	io.LogFilename = nullptr;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImFontConfig m_config;
	io.Fonts->AddFontDefault();
	icons = io.Fonts->AddFontFromMemoryTTF(&icon, sizeof icon, 28);

	ImGui::StyleColorsDark();

	std::ifstream file("C:\\str.png");
	if (file.is_open())
	{
		file.close();
		D3DX11_IMAGE_LOAD_INFO info; ID3DX11ThreadPump* pump{ nullptr };
		D3DX11CreateShaderResourceViewFromFileA(g_pd3dDevice, "C:\\str.png", &info, pump, &logo, 0);
	}
}

