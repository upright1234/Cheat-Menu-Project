#include <Windows.h>
#include <dwmapi.h>
#include <iostream>
#include <mutex>
#include <d3d11.h>
#include <shlobj.h>
#include <io.h>
#include <fstream>
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_dx11.h"
#include "Imgui/imgui_impl_win32.h"
#include "CheatSettings/Config.h"
#include "GUI/GUI.h"
#include <Xinput.h>
#include <Xinput.h>
#pragma comment(lib, "Urlmon.lib")
#pragma comment(lib, "Xinput.lib")
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "d3dx11.lib")

bool init = true;

void LoadController()
{
	ImGuiIO &io = ImGui::GetIO();

	XINPUT_STATE controllerState;
	if (XInputGetState(0, &controllerState) == ERROR_SUCCESS)
	{

		// Map controller buttons
		const WORD buttons = controllerState.Gamepad.wButtons;
#define IM_SATURATE(V) (V < 0.0f ? 0.0f : V > 1.0f ? 1.0f \
																									 : V)
#define MAP_BUTTON(KEY_NO, BUTTON_ENUM)                   \
	{                                                       \
		io.AddKeyEvent(KEY_NO, (buttons & BUTTON_ENUM) != 0); \
	}
#define MAP_ANALOG(KEY_NO, VALUE, V0, V1)                      \
	{                                                            \
		float vn = (float)(VALUE - V0) / (float)(V1 - V0);         \
		io.AddKeyAnalogEvent(KEY_NO, vn > 0.10f, IM_SATURATE(vn)); \
	}
		MAP_BUTTON(ImGuiKey_GamepadStart, XINPUT_GAMEPAD_START);
		MAP_BUTTON(ImGuiKey_GamepadBack, XINPUT_GAMEPAD_BACK);
		MAP_BUTTON(ImGuiKey_GamepadFaceLeft, XINPUT_GAMEPAD_X);
		MAP_BUTTON(ImGuiKey_GamepadFaceRight, XINPUT_GAMEPAD_B);
		MAP_BUTTON(ImGuiKey_GamepadFaceUp, XINPUT_GAMEPAD_Y);
		MAP_BUTTON(ImGuiKey_GamepadFaceDown, XINPUT_GAMEPAD_A);
		MAP_BUTTON(ImGuiKey_GamepadDpadLeft, XINPUT_GAMEPAD_DPAD_LEFT);
		MAP_BUTTON(ImGuiKey_GamepadDpadRight, XINPUT_GAMEPAD_DPAD_RIGHT);
		MAP_BUTTON(ImGuiKey_GamepadDpadUp, XINPUT_GAMEPAD_DPAD_UP);
		MAP_BUTTON(ImGuiKey_GamepadDpadDown, XINPUT_GAMEPAD_DPAD_DOWN);
		MAP_BUTTON(ImGuiKey_GamepadL1, XINPUT_GAMEPAD_LEFT_SHOULDER);
		MAP_BUTTON(ImGuiKey_GamepadR1, XINPUT_GAMEPAD_RIGHT_SHOULDER);
		MAP_ANALOG(ImGuiKey_GamepadL2, controllerState.Gamepad.bLeftTrigger, XINPUT_GAMEPAD_TRIGGER_THRESHOLD, 255);
		MAP_ANALOG(ImGuiKey_GamepadR2, controllerState.Gamepad.bRightTrigger, XINPUT_GAMEPAD_TRIGGER_THRESHOLD, 255);
		MAP_BUTTON(ImGuiKey_GamepadL3, XINPUT_GAMEPAD_LEFT_THUMB);
		MAP_BUTTON(ImGuiKey_GamepadR3, XINPUT_GAMEPAD_RIGHT_THUMB);
		MAP_ANALOG(ImGuiKey_GamepadLStickLeft, controllerState.Gamepad.sThumbLX, -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, -32768);
		MAP_ANALOG(ImGuiKey_GamepadLStickRight, controllerState.Gamepad.sThumbLX, +XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, +32767);
		MAP_ANALOG(ImGuiKey_GamepadLStickUp, controllerState.Gamepad.sThumbLY, +XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, +32767);
		MAP_ANALOG(ImGuiKey_GamepadLStickDown, controllerState.Gamepad.sThumbLY, -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, -32768);
		MAP_ANALOG(ImGuiKey_GamepadRStickLeft, controllerState.Gamepad.sThumbRX, -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, -32768);
		MAP_ANALOG(ImGuiKey_GamepadRStickRight, controllerState.Gamepad.sThumbRX, +XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, +32767);
		MAP_ANALOG(ImGuiKey_GamepadRStickUp, controllerState.Gamepad.sThumbRY, +XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, +32767);
		MAP_ANALOG(ImGuiKey_GamepadRStickDown, controllerState.Gamepad.sThumbRY, -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, -32768);
	}
}

void LoadKeys()
{

	ImGuiIO &io = ImGui::GetIO();
	(void)io;

	io.MouseDown[0] = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
	io.MouseDown[1] = GetAsyncKeyState(VK_RBUTTON) & 0x8000;
	io.MouseDown[2] = GetAsyncKeyState(VK_MBUTTON) & 0x8000;
	io.MouseDown[3] = GetAsyncKeyState(VK_XBUTTON1) & 0x8000;
	io.MouseDown[4] = GetAsyncKeyState(VK_XBUTTON2) & 0x8000;

	/*for (auto i = 0x08; i <= 0xA5; i++)
	{
		auto key = ImGui_ImplWin32_VirtualKeyToImGuiKey(i);
		if (key != ImGuiKey_None)
		{
			io.AddKeyEvent(key, (GetKeyState(i) & 0x8000) != 0);
		}
	}

	bool isShift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) || (GetAsyncKeyState(VK_LSHIFT) & 0x8000) || (GetAsyncKeyState(VK_RSHIFT) & 0x8000);
	bool isCaps = GetKeyState(0x14);
	for (int i = '0'; i < '9'; i++)
	{
		if (GetAsyncKeyState(i) & 1)
		{
			io.AddInputCharacter(i);
		}
	}
	for (int i = 'A'; i < 'Z'; i++)
	{
		if (GetAsyncKeyState(i) & 1)
		{
			if (isCaps && !isShift)
			{
				io.AddInputCharacter(i);
			}
			else if (isShift && !isCaps)
			{
				io.AddInputCharacter(i);
			}
			else
			{
				io.AddInputCharacter(i + 32);
			}
		}
	}*/

	if ((GetKeyState(VK_LEFT) & 0x8000) != 0)
	{
		io.MouseWheelH = +0.2;
	}
	if ((GetKeyState(VK_RIGHT) & 0x8000) != 0)
	{
		io.MouseWheelH = -0.2;
	}
	if ((GetKeyState(VK_UP) & 0x8000) != 0)
	{
		io.MouseWheel = +0.2;
	}
	if ((GetKeyState(VK_DOWN) & 0x8000) != 0)
	{
		io.MouseWheel = -0.2;
	}
}

bool InitializeMenu(HWND Window, ID3D11Device *g_pd3dDevice, ID3D11DeviceContext *g_pd3dDeviceContext, int *ResellerMenuColor)
{

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	InitMyMenu(g_pd3dDevice);

	auto ret = ImGui_ImplWin32_Init(Window, Window);
	if (!ret)
		return false;

	ret = ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
	if (!ret)
		return false;
}

void RenderMenu(bool *ShowMenu, int GameX, int GameY, int GameWidth, int GameHeight, SETTINGS *Settings, CConfig *g_pConfig, float mouse_wheel)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame(ShowMenu);
	ImGui::NewFrame();

	LoadKeys();
	LoadController();
	if (mouse_wheel != 0.f) {
		ImGuiIO& io = ImGui::GetIO();
		(void)io;
		io.AddMouseWheelEvent(0, mouse_wheel);
	}

	ImGui::SetNextWindowBgAlpha(0.f);
	ImGui::SetNextWindowPos({(float)GameX, (float)GameY}, ImGuiCond_Always);
	ImGui::SetNextWindowSize({(float)GameWidth, (float)GameHeight}, ImGuiCond_Always);
	ImGui::Begin(("##scene"), nullptr, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar);

	if (init)
	{
		ImGui::SetNextWindowPos({(float)GameX + 300, (float)GameY + 300});
		init = false;
	}

	// Put your menu here
	if (*ShowMenu == true)
		RenderMyMenu(ShowMenu, Settings, g_pConfig);

	ImGui::End();
	ImGui::EndFrame();
}

void EndRender()
{
	ImGui::Render();
}

void RenderDx11()
{
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

typedef struct _lpReserved
{
	decltype (&RenderMenu)(RenderMenu);
	decltype (&EndRender)(EndRender);
	decltype (&RenderDx11)(RenderDx11);
	decltype (&InitializeMenu)(InitializeMenu);
};

BOOL WINAPI DllMain(
		HINSTANCE hModule,
		DWORD fdwReason,
		LPVOID lpReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		auto Reserved = reinterpret_cast<_lpReserved *>(lpReserved);
		Reserved->RenderMenu = RenderMenu;
		Reserved->InitializeMenu = InitializeMenu;
		Reserved->EndRender = EndRender;
		Reserved->RenderDx11 = RenderDx11;
		break;
	}

	return TRUE;
}