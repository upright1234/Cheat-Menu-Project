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
#include "GUI/GUI.h"
#include <Xinput.h>
#include <Xinput.h>
#pragma comment(lib,"Urlmon.lib")
#pragma comment(lib,"Xinput.lib")

bool init = true;

void LoadController() {
	ImGuiIO& io = ImGui::GetIO();

	XINPUT_STATE controllerState;
	if (XInputGetState(0, &controllerState) == ERROR_SUCCESS) {

		// Map controller buttons
		const WORD buttons = controllerState.Gamepad.wButtons;
		io.NavInputs[ImGuiNavInput_Activate] = (buttons & XINPUT_GAMEPAD_A) ? 1.0f : 0.0f;
		io.NavInputs[ImGuiNavInput_Cancel] = (buttons & XINPUT_GAMEPAD_B) ? 1.0f : 0.0f;
		io.NavInputs[ImGuiNavInput_Menu] = (buttons & XINPUT_GAMEPAD_X) ? 1.0f : 0.0f;
		io.NavInputs[ImGuiNavInput_Input] = (buttons & XINPUT_GAMEPAD_Y) ? 1.0f : 0.0f;

		// Additional controller triggers
		io.NavInputs[ImGuiNavInput_FocusPrev] = (buttons & XINPUT_GAMEPAD_LEFT_SHOULDER) ? 1.0f : 0.0f; // L1
		io.NavInputs[ImGuiNavInput_FocusNext] = (buttons & XINPUT_GAMEPAD_RIGHT_SHOULDER) ? 1.0f : 0.0f; // R1
		float l2 = controllerState.Gamepad.bLeftTrigger / 255.0f;
		float r2 = controllerState.Gamepad.bRightTrigger / 255.0f;

		// Map controller sticks
		float lx = controllerState.Gamepad.sThumbLX / 32767.0f;
		float ly = controllerState.Gamepad.sThumbLY / 32767.0f;

		io.NavInputs[ImGuiNavInput_LStickLeft] = (lx < -0.3f) ? -lx : 0.0f;
		io.NavInputs[ImGuiNavInput_LStickRight] = (lx > 0.3f) ? lx : 0.0f;
		io.NavInputs[ImGuiNavInput_LStickUp] = (ly > 0.3f) ? ly : 0.0f;
		io.NavInputs[ImGuiNavInput_LStickDown] = (ly < -0.3f) ? -ly : 0.0f;
	}
}

void LoadKeys() {

	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	io.MouseDown[0] = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
	io.MouseDown[1] = GetAsyncKeyState(VK_RBUTTON) & 0x8000;
	io.MouseDown[2] = GetAsyncKeyState(VK_MBUTTON) & 0x8000;
	io.MouseDown[3] = GetAsyncKeyState(VK_XBUTTON1) & 0x8000;
	io.MouseDown[4] = GetAsyncKeyState(VK_XBUTTON2) & 0x8000;

	for (auto i = 0x08; i <= 0xA5; i++) {
		auto key = ImGui_ImplWin32_VirtualKeyToImGuiKey(i);
		if (key != ImGuiKey_None) {
			io.AddKeyEvent(key, (GetKeyState(i) & 0x8000) != 0);
		}
	}

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
}

bool InitializeMenu(HWND Window, ID3D11Device* g_pd3dDevice, ID3D11DeviceContext* g_pd3dDeviceContext) {

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	InitMyMenu(g_pd3dDevice);

	auto ret = ImGui_ImplWin32_Init(Window);
	if (!ret)
		return false;

	ret = ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
	if (!ret)
		return false;
}

void RenderMenu(bool* ShowMenu, int GameX, int GameY, int GameWidth, int GameHeight, SETTINGS* Settings, CConfig* g_pConfig)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	
	LoadKeys();
	LoadController();

	ImGui::SetNextWindowBgAlpha(0.f);
	ImGui::SetNextWindowPos({ (float)GameX, (float)GameY }, ImGuiCond_Always);
	ImGui::SetNextWindowSize({ (float)GameWidth, (float)GameHeight }, ImGuiCond_Always);
	ImGui::Begin(("##scene"), nullptr, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar);

	if (init) {
		ImGui::SetNextWindowPos({ (float)GameX + 300, (float)GameY + 300 });
		init = false;
	}


	//Put your menu here
	if (*ShowMenu == true)
		RenderMyMenu(ShowMenu, Settings, g_pConfig);

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