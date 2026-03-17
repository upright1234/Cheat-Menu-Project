#include "../GUI.h"
#include  <fstream>

#define  IMGUI_DEFINE_MATH_OPERATORS
#include "../../Imgui/imgui_internal.h"
#include "../main.h"
#include "../etc_element.h"

#if IsRust
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
    
    const std::vector<std::string> box_types = { ("Square"), ("Corner") };
    const std::vector<std::string> color_types = { ("Static"), ("By Team"), ("By Visible") };
    const std::vector<std::string> position_types = { ("Top"), ("Bottom"), ("Left"), ("Right") };
    const std::vector<std::string> position_min_types = { ("Top"), ("Bottom") };
    const std::vector<std::string> hitboxes = { ("Head"), ("Chest"), ("Body"), ("Nearest") };
    const std::vector<std::string> target_types = { ("Circle"), ("Triangle") };
    const std::vector<std::string> chams_material_types = {
        ("Red [Default]"),
        ("White (visible through walls)"),
        ("Mirror (visible through walls)"),
        ("Gray (visible through walls)"),
        ("Bright sky blue")
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
        Settings->Visuals.Players.NickName.FontStyle = selectedFontForAll;
        Settings->Visuals.Players.Weapon.FontStyle = selectedFontForAll;
        Settings->Visuals.Players.Distance.FontStyle = selectedFontForAll;
        Settings->Visuals.Players.Status.FontStyle = selectedFontForAll;
        Settings->Visuals.Players.Velocity.FontStyle = selectedFontForAll;
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