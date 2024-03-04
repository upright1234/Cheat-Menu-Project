#include <Windows.h>
#include <dwmapi.h>
#include <iostream>
#include <mutex>
#include <d3d11.h>
#include <shlobj.h>
#include  <io.h>
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_dx11.h"
#include "Imgui/imgui_impl_win32.h"
#include "CheatSettings.h"

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
	//Initialize textures

	auto ret = ImGui_ImplWin32_Init(Window);
	if (!ret)
		return false;

	ret = ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
	if (!ret)
		return false;
}

bool init = true;
bool EnableEsp = true;
void RenderMenu(bool* ShowMenu, int GameX, int GameY, int GameWidth, int GameHeight, SETTINGS* Settings)
{
	if (!(*ShowMenu))
		return;

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

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

void EntryPoint(PVOID Context) {
	auto Reserved = reinterpret_cast<_lpReserved*>(Context);
	Reserved->RenderMenu = RenderMenu;
	Reserved->InitializeMenu = InitializeMenu;
	Reserved->EndRender = EndRender;
	Reserved->RenderDx11 = RenderDx11;
}

BOOL WINAPI DllMain(
	HINSTANCE hModule,
	DWORD fdwReason,
	LPVOID lpReserved)
{
		switch (fdwReason)
		{
		case DLL_PROCESS_ATTACH:
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)EntryPoint, lpReserved, 0, 0);
			break;
		}

	return TRUE;


}