#include "../GUI.h"
#include  <fstream>

#define  IMGUI_DEFINE_MATH_OPERATORS
#include "../../Imgui/imgui_internal.h"
#include "../main.h"
#include "../etc_element.h"

#if IsApex
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
    
    std::vector<std::string> ByWeaponTypes = { ("Disabled"), ("AR")
                            , ("DMR")
                                , ("LMG")
                            , ("Melee")
                            , ("Pistol")
                            , ("Shotgun")
                            , ("SMG")
                                , ("Sniper")
                            , ("Special")
                            , ("Throw") };
    std::vector<std::string> WeaponTypes = { ("AR")
                                , ("DMR")
                                    , ("LMG")
                                , ("Melee")
                                , ("Pistol")
                                , ("Shotgun")
                                , ("SMG")
                                    , ("Sniper")
                                , ("Special")
                                , ("Throw") };
    std::vector<std::string> BoxTypes = { ("Square"), ("Corner") };
    std::vector<std::string> AimTargetTypes = { ("Circle"), ("Triangle") };
    std::vector<std::string> ColorTypes = { ("Static"), ("By Team"), ("By Visible") };
    std::vector<std::string> PositionTypes = { ("Top"), ("Bottom"), ("Left"), ("Right") };
    std::vector<std::string> PositionMinTypes = { ("Top"), ("Bottom") };
    std::vector<std::string> DrawFormatTypes = { ("Circle"), ("Triangle") };
    std::vector<std::string> hitboxes = { ("Head"), ("Chest"), ("Body"), ("Random") };
    std::vector<std::string> RarityTypes = { ("All"), ("Uncommon"), ("Rare"), ("Epic"), ("Legendary") };
    std::vector<std::string> DrawWeaponTypes = { ("By Type"), ("By Name") };
    std::vector<std::string> DrawAttachmentTypes = { ("By Type"), ("By Name"), ("By Current Weapon") };
    std::vector<std::string> AttachmentTypes = {
        ("Sights"),
        ("Barrel Stabilizer"),
        ("Magazine"),
        ("Stock"),
        ("Shotgun Bolt"),
        ("Hop-Up"),
        ("Laser Sight")
    };
    std::vector<std::string> bodyStyles = { "None", "Pink", "Pink Visible Only", "Pulsing 1", "Pulsing Line Invisible Only", "Dark Pulsing Line", "Sharp Pulsing Visible Only", "Sharp Pulsing", "Pulsing Red Line", "Fast Pulsing Invisible", "Pulsing Up", "Solid Pulsing", "Solid Pulsing 2", "Bright", "Bright 2", "Light", "Light Solid", "Red Pulsing Visible Only", "Wave", "Shaded Visible", "Wireframe", "Wireframe Visible Only", "Black", "Black Visible Only" };
    std::vector<std::string> outlineStyles = { "None", "Bright", "Bright Invisible Only", "Dark", "Pink", "White", "Gold Flashing", "Gold", "Brown", "Wave", "Red", "Red Bright", "Heartbeat Visible Only", "Green Invisible Only", "Orange", "Visible Only", "Red" };


    /*//Install font example
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

    //Config By Weapon example
    ImGui::BeginGroup();
    {
        ImGui::Combo("Weapon", &Settings->Aim.ByWeapon, ByWeaponTypes, IM_ARRAYSIZE(ByWeaponTypes), IM_ARRAYSIZE(ByWeaponTypes));   
    }ImGui::EndGroup();

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

    }ImGui::EndGroup();*/
}
#endif