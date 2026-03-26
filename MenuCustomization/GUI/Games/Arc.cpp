#include "../GUI.h"
#include  <fstream>

#define  IMGUI_DEFINE_MATH_OPERATORS
#include "../../Imgui/imgui_internal.h"
#include "../main.h"
#include "../etc_element.h"

#if IsArc
float animation_ = 0.0f;

#define ALPHA (ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar)
#define NO_ALPHA (ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar)

char cfgname[30] = "NewSetting";
int selectedItem = 0;
bool firstTime = true;
void RenderMyMenu(bool* ShowMenu, SETTINGS* Settings, CConfig* g_pConfig) {

    //example force disable option
    Settings->Aim.Enable = false;

    if (!ShowMenu)
        return;
    
    std::vector<std::string> BoxTypes = { ("Square"), ("Corner") };
    std::vector<std::string> ColorTypes = { ("Static"), ("By Team"), ("By Visible") };
    std::vector<std::string> PositionTypes = { ("Top"), ("Bottom"), ("Left"), ("Right") };
    std::vector<std::string> PositionMinTypes = { ("Top"), ("Bottom") };
    std::vector<std::string> DrawFormatTypes = { ("Circle"), ("Triangle") };
    std::vector<std::string> hitboxes = { ("Head"), ("Chest"), ("Body"), ("Random") };
    std::vector<std::string> DroneTypeLabels = {
        "Wasp", "Hornet", "Rocketeer", "Pop", "Fireball", "Tick",
        "Turret", "Sentinel", "Snitch", "Bastion", "Spotter", "Leaper",
        "Surveyor", "Queen", "Bombardier", "Shredder", "Matriarch", "Flamethrower",
        "Rollbot", "Smoke Trail"
    };
    std::vector<std::string> ContainerTypeLabels = {
        "Lockers", "Crates", "Desks/Closets", "Trash", "Nature", "Vehicles",
        "Raider Loot", "Shelf Drawers", "Cabinets", "Consoles", "Servers",
        "Wall Panels", "Special Crates", "Workbench/Android", "Supply Call Station"
    };

    //Install font example
    static int selectedSystemFont = 0;
    ImGui::Combo((("Available System Fonts")), &selectedSystemFont, Settings->AvailableFonts);
    if (ImGui::Button((("Load Font")), { 100, 25 })) {
        if (selectedSystemFont >= 0 && selectedSystemFont < Settings->AvailableFonts.size()) {
            std::string fontName = Settings->AvailableFonts[selectedSystemFont];
            if (std::find(Settings->InstalledFonts.begin(), Settings->InstalledFonts.end(), fontName) == Settings->InstalledFonts.end()) {
                Settings->InstalledFonts.push_back(fontName);
            }
        }
    }

    //Apply font to all render example
    static int selectedFontForAll = 0;
    ImGui::Combo((("Select Font for All")), &selectedFontForAll, Settings->InstalledFonts);
    if (ImGui::Button((("Apply to All")), { 150, 25 })) {
        Settings->Visuals.Players.Weapon.FontStyle = selectedFontForAll;
        Settings->Visuals.Players.Distance.FontStyle = selectedFontForAll;
        Settings->Visuals.Items.FontStyle = selectedFontForAll;
    }

    //Config selector example
    ImGui::BeginGroup();
    {
        if (ImGui::Combo("Select Your Config", &Settings->iConfig, g_pConfig->szConfigs.c_str())) {
            if (Settings->oldiConfig != Settings->iConfig) {
                g_pConfig->InitPath(&g_pConfig->vecConfigs[Settings->oldiConfig][0u]);
                g_pConfig->Save();
                Settings->oldiConfig = Settings->iConfig;
                g_pConfig->InitPath(&g_pConfig->vecConfigs[Settings->iConfig][0u]);
                g_pConfig->Read();
            }
        }
        ImGui::InputText("Config Name", cfgname, 30);
        if (ImGui::Button("Add Config")) {
            g_pConfig->CreateConfig(cfgname);
        }

    }ImGui::EndGroup();
}
#endif