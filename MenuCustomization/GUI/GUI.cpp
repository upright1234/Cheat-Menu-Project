#include "GUI.h"
#include "imgui_setting.h"
#include "main.h"
#include "Font.h"
#include "LogoData.h"
#include "DebugLog.h"

#if MENU_DEBUG_LOG
// Runs while the DLL's C runtime starts up (before DllMain), so the log shows whether the DLL was loaded at all
static const bool g_logLoaded = (menulog::OnLoad(), true);
#endif

// Uploads the embedded logo (LogoData.h) as an immutable texture; the menu header draws it
static ID3D11ShaderResourceView* CreateLogoTexture(ID3D11Device* device) {
	if (!device)
		return nullptr;
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = kLogoWidth;
	desc.Height = kLogoHeight;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = kLogoRGBA;
	data.SysMemPitch = kLogoWidth * 4;

	ID3D11Texture2D* tex = nullptr;
	if (FAILED(device->CreateTexture2D(&desc, &data, &tex)))
		return nullptr;
	ID3D11ShaderResourceView* srv = nullptr;
	device->CreateShaderResourceView(tex, nullptr, &srv);
	tex->Release();
	return srv;
}

void InitMyMenu(ID3D11Device* g_pd3dDevice) {
#if MENU_DEBUG_LOG
	menulog::OnInit(g_pd3dDevice);
#endif
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.IniFilename = nullptr;
	io.LogFilename = nullptr;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	// Colors read by the customized widgets in Imgui/imgui_widgets.cpp (checkbox/slider fill and label text).
	// tabs::text defaults to transparent, which hides checkbox and slider labels if left unset.
	settings::general_color = ImColor(86, 128, 190, 255);
	settings::tabs::text = ImColor(214, 222, 235, 255);
	settings::tabs::text_hov = ImColor(236, 241, 250, 255);
	settings::tabs::text_active = ImColor(255, 255, 255, 255);

	// Black / near-black base (shown see-through, see kit::g_bgOpacity) with blue-gray accents
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;
	auto rgb = [](int r, int g, int b, int a = 255) { return ImVec4(r / 255.f, g / 255.f, b / 255.f, a / 255.f); };

	colors[ImGuiCol_Text] = rgb(230, 232, 238);
	colors[ImGuiCol_TextDisabled] = rgb(116, 120, 132);
	colors[ImGuiCol_WindowBg] = rgb(0, 0, 0);
	colors[ImGuiCol_ChildBg] = rgb(0, 0, 0, 0);
	colors[ImGuiCol_PopupBg] = rgb(10, 10, 12, 250);
	colors[ImGuiCol_Border] = rgb(36, 36, 42);
	colors[ImGuiCol_BorderShadow] = rgb(0, 0, 0, 0);
	colors[ImGuiCol_FrameBg] = rgb(24, 24, 28);
	colors[ImGuiCol_FrameBgHovered] = rgb(32, 32, 38);
	colors[ImGuiCol_FrameBgActive] = rgb(40, 42, 50);
	colors[ImGuiCol_TitleBg] = rgb(0, 0, 0);
	colors[ImGuiCol_TitleBgActive] = rgb(8, 8, 10);
	colors[ImGuiCol_TitleBgCollapsed] = rgb(0, 0, 0);
	colors[ImGuiCol_MenuBarBg] = rgb(8, 8, 10);
	colors[ImGuiCol_ScrollbarBg] = rgb(0, 0, 0, 0);
	colors[ImGuiCol_ScrollbarGrab] = rgb(44, 44, 52);
	colors[ImGuiCol_ScrollbarGrabHovered] = rgb(60, 60, 70);
	colors[ImGuiCol_ScrollbarGrabActive] = rgb(78, 80, 92);
	colors[ImGuiCol_CheckMark] = rgb(150, 185, 235);
	colors[ImGuiCol_SliderGrab] = rgb(86, 128, 190);
	colors[ImGuiCol_SliderGrabActive] = rgb(110, 150, 210);
	colors[ImGuiCol_Button] = rgb(22, 22, 26);
	colors[ImGuiCol_ButtonHovered] = rgb(34, 34, 40);
	colors[ImGuiCol_ButtonActive] = rgb(46, 48, 58);
	colors[ImGuiCol_Header] = rgb(30, 32, 40);
	colors[ImGuiCol_HeaderHovered] = rgb(38, 40, 50);
	colors[ImGuiCol_HeaderActive] = rgb(48, 52, 66);
	colors[ImGuiCol_Separator] = rgb(36, 36, 42);
	colors[ImGuiCol_SeparatorHovered] = rgb(60, 62, 74);
	colors[ImGuiCol_SeparatorActive] = rgb(86, 128, 190);
	colors[ImGuiCol_Tab] = rgb(13, 13, 15);
	colors[ImGuiCol_TabHovered] = rgb(34, 34, 40);
	colors[ImGuiCol_TabActive] = rgb(30, 32, 40);
	colors[ImGuiCol_TabUnfocused] = rgb(13, 13, 15);
	colors[ImGuiCol_TabUnfocusedActive] = rgb(30, 32, 40);
	colors[ImGuiCol_TextSelectedBg] = rgb(86, 128, 190, 90);
	colors[ImGuiCol_NavHighlight] = rgb(86, 128, 190);

	style.WindowPadding = ImVec2(12.0f, 12.0f);
	style.FramePadding = ImVec2(8.0f, 5.0f);
	style.ItemSpacing = ImVec2(8.0f, 7.0f);
	style.ItemInnerSpacing = ImVec2(6.0f, 4.0f);
	style.ScrollbarSize = 8.0f;
	style.WindowBorderSize = 1.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupBorderSize = 1.0f;
	style.FrameBorderSize = 0.0f;
	style.WindowRounding = 10.0f;
	style.ChildRounding = 8.0f;
	style.PopupRounding = 6.0f;
	style.FrameRounding = 6.0f;
	style.GrabRounding = 6.0f;
	style.ScrollbarRounding = 6.0f;
	style.TabRounding = 6.0f;
	MENU_LOG("InitMyMenu: style set");

	// Font data lives in static arrays (Font.h), so the atlas must not try to free it
	ImFontConfig fontCfg;
	fontCfg.FontDataOwnedByAtlas = false;

	// Main_Font is the default UI font (added first); Title_Font is the same face for headings
	Main_Font = io.Fonts->AddFontFromMemoryTTF(&MainFont, sizeof MainFont, 16.f, &fontCfg, io.Fonts->GetGlyphRangesCyrillic());
	Title_Font = io.Fonts->AddFontFromMemoryTTF(&MainFont, sizeof MainFont, 19.f, &fontCfg, io.Fonts->GetGlyphRangesCyrillic());
	Font1 = io.Fonts->AddFontFromMemoryTTF(&Font, sizeof Font, 20.f, &fontCfg, io.Fonts->GetGlyphRangesCyrillic());
	ico = io.Fonts->AddFontFromMemoryTTF(&icon, sizeof icon, 30.f, &fontCfg, io.Fonts->GetGlyphRangesCyrillic());
	icons = io.Fonts->AddFontFromMemoryTTF(&Icon, sizeof Icon, 20.f, &fontCfg, io.Fonts->GetGlyphRangesCyrillic());
	MENU_LOG("InitMyMenu: %d fonts added", io.Fonts->Fonts.Size);

	Logo_Texture = CreateLogoTexture(g_pd3dDevice);
	MENU_LOG("InitMyMenu: logo texture %s", Logo_Texture ? "created" : "NOT created");

	// The template's optional "C:\str.png" loader is gone: nothing read the texture it produced (`users`), the
	// header draws the embedded logo from LogoData.h instead, and under the loader (2026-10-14 log) init stopped
	// inside that block. Dropping it also removes the legacy DirectX SDK (d3dx11) dependency from this build.
	MENU_LOG("InitMyMenu: done");
}

