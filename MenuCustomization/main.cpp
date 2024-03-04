#include <Windows.h>
#include <dwmapi.h>
#include <iostream>
#include <mutex>
#include <d3d11.h>
#include <shlobj.h>
#include  <io.h>
#include  <fstream>
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_dx11.h"
#include "Imgui/imgui_impl_win32.h"
#include "CheatSettings/CheatSettings.h"
#include "CheatSettings/Config.h"
#pragma comment(lib,"Urlmon.lib")

bool FileExists(const std::string& Filename)
{
	return _access_s(Filename.c_str(), 04) == 0;
}

std::string LoadChinese() {
	std::string dwnld_URL = "https://cdn.chairfbi.com/ChineseFont.ttf";
	std::string savepath = "ChineseFont.ttf";
	if (!FileExists(savepath)) {
		if (URLDownloadToFileA(NULL, dwnld_URL.c_str(), savepath.c_str(), 0, NULL) != S_OK) {
			return "";
		}
	}
	return FileExists(savepath) ? savepath : "";
}

const ImWchar* GetGlyphRangesChinese()
{
	static const ImWchar ranges[] =
	{
		0x0020, 0x00FF, // Basic Latin + Latin Supplement
		0x3000, 0x30FF, // Punctuations, Hiragana, Katakana
		0x31F0, 0x31FF, // Katakana Phonetic Extensions
		0xFF00, 0xFFEF, // Half-width characters
		0x4e00, 0x9FAF, // CJK Ideograms
		0,
	};
	return &ranges[0];
}

bool InitializeMenu(HWND Window, ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pd3dDeviceContext) {

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.IniFilename = nullptr;
	io.LogFilename = nullptr;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGuiStyle* style = &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	colors[ImGuiCol_FrameBg] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.34f, 0.34f, 0.34f, 0.64f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.34f, 0.34f, 0.34f, 0.74f);
	colors[ImGuiCol_Button] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.34f, 0.34f, 0.34f, 0.64f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.34f, 0.34f, 0.34f, 0.74f);

	//Load Fonts
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 20.0f);
	/*
	//Load Chinese Font
	auto fontPath = LoadChinese();
	if (!fontPath.empty()) {
		ImFontConfig fontConfig;
		fontConfig.MergeMode = true;
		fontConfig.PixelSnapH = true;
		io.Fonts->AddFontFromFileTTF(fontPath.c_str(), 20, &fontConfig, GetGlyphRangesChinese());
	}
	io.AddInputCharacter(0x410);
	io.AddInputCharacter(0x4E00);
	*/
	//Initialize textures

	auto ret = ImGui_ImplWin32_Init(Window);
	if (!ret)
		return false;

	ret = ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
	if (!ret)
		return false;
}

void LoadKeys(ImGuiIO& io) {
	bool isShift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) || (GetAsyncKeyState(VK_LSHIFT) & 0x8000) || (GetAsyncKeyState(VK_RSHIFT) & 0x8000);
	bool isCaps = GetKeyState(0x14);
	for (int i = '0'; i < '9'; i++) {
		if (GetAsyncKeyState(i) & 1) {
			io.AddInputCharacter(i);
		}
	}
	for (int i = 'A'; i < 'Z'; i++) {
		if (GetAsyncKeyState(i) & 1) {
			if (isCaps && !isShift) {
				io.AddInputCharacter(i);
			}
			else if (isShift && !isCaps) {
				io.AddInputCharacter(i);
			}
			else {
				io.AddInputCharacter(i + 32);
			}
		}
	}
}

bool init = true;
bool EnableEsp = true;
char cfgname[30] = "NewSetting";
void RenderMenu(bool* ShowMenu, int GameX, int GameY, int GameWidth, int GameHeight, SETTINGS* Settings, CConfig* g_pConfig)
{
	if (!(*ShowMenu))
		return;

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGuiIO& io = ImGui::GetIO();

	io.MouseDown[0] = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
	io.MouseDown[1] = GetAsyncKeyState(VK_RBUTTON) & 0x8000;
	io.MouseDown[2] = GetAsyncKeyState(VK_MBUTTON) & 0x8000;
	io.MouseDown[3] = GetAsyncKeyState(VK_XBUTTON1) & 0x8000;
	io.MouseDown[4] = GetAsyncKeyState(VK_XBUTTON2) & 0x8000;

	for (int i = 0; i < 512; i++) {
		auto key = ImGui_ImplWin32_VirtualKeyToImGuiKey(i);
		if (key != ImGuiKey_None) {
			io.AddKeyEvent(key, (GetKeyState(i) & 0x8000) != 0);
		}
	}
	LoadKeys(io);

	if ((GetKeyState(VK_LEFT) & 0x8000) != 0) {
		io.MouseWheelH = +0.2;
	}
	if ((GetKeyState(VK_RIGHT) & 0x8000) != 0) {
		io.MouseWheelH = -0.2;
	}
	if ((GetKeyState(VK_UP) & 0x8000) != 0) {
		io.MouseWheel = +0.2;
	}
	if ((GetKeyState(VK_DOWN) & 0x8000) != 0) {
		io.MouseWheel = -0.2;
	}

	ImGui::SetNextWindowBgAlpha(0.f);
	ImGui::SetNextWindowPos({ (float)GameX, (float)GameY }, ImGuiCond_Always);
	ImGui::SetNextWindowSize({ (float)GameWidth, (float)GameHeight }, ImGuiCond_Always);
	ImGui::Begin(("##scene"), nullptr, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar);

	if (init) {
		ImGui::SetNextWindowPos({ (float)GameX + 300, (float)GameY + 300 });
		init = false;
	}

	ImGui::SetNextWindowSize(ImVec2(300, 300));
	ImGui::Begin("Fortnite", ShowMenu, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);
	{
		const auto& p = ImGui::GetWindowPos();
		ImGui::BeginChild("Child0", ImVec2(300, 300), true);
		{
			//draw functions
			ImGui::SetCursorPos(ImVec2(15, 15));
			ImGui::BeginGroup();
			{
				ImGui::Checkbox("Enable ESP", &Settings->Visuals.Players.Enable);
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Text("Select Your Config:");
				ImGui::Spacing();
				ImGui::Spacing();
				if (ImGui::Combo("##Config", &Settings->iConfig, g_pConfig->szConfigs.c_str()))
				{
					if (Settings->oldiConfig != Settings->iConfig)
					{
						g_pConfig->InitPath(&g_pConfig->vecConfigs[Settings->oldiConfig][0u]);
						g_pConfig->Save();
						Settings->oldiConfig = Settings->iConfig;
						g_pConfig->InitPath(&g_pConfig->vecConfigs[Settings->iConfig][0u]);
						g_pConfig->Read();
					}
				}

				ImGui::Text("New Config:");
				ImGui::InputText("Config Name", cfgname, 30);
				if (ImGui::Button("Add Config") && strlen(cfgname) > 2)
				{
					std::string cfstring = cfgname;
					std::string name = g_pConfig->pathname + "\\";
					std::string cfgpath = name + cfstring;
					std::ofstream outfile(cfgpath);
					outfile << " " << std::endl;
					outfile.close();
					g_pConfig->ParseConfigs();
					g_pConfig->InitPath((char*)cfstring.c_str());
					g_pConfig->Save();
				}
			}
			ImGui::EndGroup();
		}
		ImGui::EndChild();
	} ImGui::End();

	ImGui::End();
	ImGui::EndFrame();

}

void EndRender() {
	ImGui::Render();
}

void RenderDx11() {
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

typedef struct _lpReserved
{
	decltype(&RenderMenu)(RenderMenu);
	decltype(&EndRender)(EndRender);
	decltype(&RenderDx11)(RenderDx11);
	decltype(&InitializeMenu)(InitializeMenu);
};

BOOL WINAPI DllMain(
	HINSTANCE hModule,
	DWORD fdwReason,
	LPVOID lpReserved)
{
		switch (fdwReason)
		{
		case DLL_PROCESS_ATTACH:
			auto Reserved = reinterpret_cast<_lpReserved*>(lpReserved);
			Reserved->RenderMenu = RenderMenu;
			Reserved->InitializeMenu = InitializeMenu;
			Reserved->EndRender = EndRender;
			Reserved->RenderDx11 = RenderDx11;
			break;
		}

	return TRUE;


}