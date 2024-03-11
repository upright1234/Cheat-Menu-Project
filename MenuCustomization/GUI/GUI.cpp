#include "GUI.h"
#include  <fstream>

char cfgname[30] = "NewSetting";

void InitMyMenu(ID3D11Device* g_pd3dDevice) {
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
}

void RenderMyMenu(bool* ShowMenu, SETTINGS* Settings, CConfig* g_pConfig) {

    const char* hitboxes[] = { ("Head"), ("Chest"), ("Body"), ("Random") };
    const char* predicts[] = { ("OFF"), ("Movement Only"), ("Bullet Drop Only"), ("Movement + Bullet Drop") };
    const char* boxes[] = { "Square", "Corner" };
    const char* rarities[] = { ("All"), ("Common"), ("Rare"), ("Epic"), ("Legendary") };
    const char* GlowTypes[] = { ("Custom Color"), ("Single Color") };
    const char* LinesTypes[] = { ("Top"), ("Middle"), ("Bottom") };
    const char* HealthTypes[] = { ("Horizontal"), ("Vertical") };
    const char* AimTargetTypes[] = { ("Circle"), ("Triangle") };
    const char* WeaponTypes[] = { ("Huntress DMG"), ("Warforged AR"), ("Nemesis AR"), ("Enforcer AR"), ("Reaper Sniper"), ("Pistol"), ("Thunder Burst SMG"), ("Harbinger SMG"), ("Gatekeeper Shotgun"), ("Hammer Pump Shotgun"), ("Frenzy Auto Shotgun") };
    const char* FontTypes[] = { ("Fortnite"), ("Verdana") };

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
}