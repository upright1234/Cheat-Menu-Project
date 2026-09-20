#include "../GUI.h"
#include  <fstream>

#define  IMGUI_DEFINE_MATH_OPERATORS
#include "../../Imgui/imgui_internal.h"
#include "../main.h"
#include "../etc_element.h"

#if IsWarDogs
#define ALPHA (ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar)
#define NO_ALPHA (ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar)

char cfgname[30] = "NewSetting";
void RenderMyMenu(bool* ShowMenu, SETTINGS* Settings, CConfig* g_pConfig) {
    if (Settings->selectedWeapon < 0 || Settings->selectedWeapon >= kWDWeaponCategoryCount)
        Settings->selectedWeapon = 0;

    SETTINGS::AimStruct* currentAimSettings = Settings->Aim.ByWeapon
        ? &Settings->aimbot[Settings->selectedWeapon]
        : &Settings->Aim;

    //example force disable option
    currentAimSettings->Enable = false;

    if (!ShowMenu)
        return;

    std::vector<std::string> ColorTypes = { ("Static"), ("By Team"), ("By Visible") };
    const char* const HitBoxes[] = { "Head", "Chest", "Body", "Random" };

    ImGui::BeginGroup();
    {
        auto& box = Settings->Visuals.Players.Box;
        ImGui::Checkbox("Box", &box.Enable);
        ImGui::SameLine();
        ImGui::ColorEdit4("##BoxStatic", box.StaticColor, ALPHA);
        ImGui::Combo("Box Color Type", &box.ColorType, ColorTypes);
        if (box.ColorType == 1) {
            ImGui::SliderInt("Team Alpha", &box.TeamColorAlpha, 0, 255);
        }
        else if (box.ColorType == 2) {
            ImGui::ColorEdit4("Visible Color", box.VisibleColor, ALPHA);
            ImGui::ColorEdit4("Non-Visible Color", box.NonVisibleColor, ALPHA);
        }
    }ImGui::EndGroup();

    ImGui::Checkbox("Configuration By Weapon", &Settings->Aim.ByWeapon);
    if (Settings->Aim.ByWeapon)
        ImGui::Combo("Current Weapon", &Settings->selectedWeapon, kWDWeaponCategoryNames,
            kWDWeaponCategoryCount, kWDWeaponCategoryCount);

    currentAimSettings = Settings->Aim.ByWeapon
        ? &Settings->aimbot[Settings->selectedWeapon]
        : &Settings->Aim;
    currentAimSettings->Enable = false;

    //Aimbot hitbox example (int index of the list)
    ImGui::Combo("HitBox", &currentAimSettings->HitBox, HitBoxes, IM_ARRAYSIZE(HitBoxes), IM_ARRAYSIZE(HitBoxes));

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

    //Apply font to all render example (a FontStyle is the index in Settings->InstalledFonts)
    static int selectedFontForAll = 0;
    ImGui::Combo((("Select Font for All")), &selectedFontForAll, Settings->InstalledFonts);
    if (ImGui::Button((("Apply to All")), { 150, 25 })) {
        Settings->Visuals.Players.NickName.FontStyle = selectedFontForAll;
        Settings->Visuals.Players.Weapon.FontStyle = selectedFontForAll;
        Settings->Visuals.Players.Distance.FontStyle = selectedFontForAll;
        Settings->Visuals.WorldEsp.FontStyle = selectedFontForAll;
    }

    //World ESP example. Item categories are tables indexed by the WDCategory / WDItemGroup enums (see WarDogs.h):
    //  Categories.Style[WDCategory]  toggle (.Enable) + color (.Color) of ONE category
    //  GroupDistance[WDItemGroup]    max distance shared by the categories of a group
    //WDC_Other is never drawn by the cheat: do not put it in the menu.
    ImGui::BeginGroup();
    {
        auto& world = Settings->Visuals.WorldEsp;

        //Ammo has no group: one category, drawn up to WorldEsp.MaxDistance
        auto& ammo = world.Categories.Style[WDC_Ammo];
        ImGui::Checkbox(kWDCategoryNames[WDC_Ammo], &ammo.Enable);
        ImGui::SameLine();
        ImGui::ColorEdit4("##Ammo", ammo.Color, ALPHA);

        //These kinds have no category: a toggle and one color each
        ImGui::Checkbox("Containers", &world.ContainersEnable);
        ImGui::SameLine();
        ImGui::ColorEdit4("##Containers", world.ColorContainer, ALPHA);
        ImGui::Checkbox("Bodies", &world.BodiesEnable);
        ImGui::SameLine();
        ImGui::ColorEdit4("##Bodies", world.ColorBody, ALPHA);
        ImGui::Checkbox("Deployables", &world.DeployablesEnable);
        ImGui::SameLine();
        ImGui::ColorEdit4("##Deployables", world.ColorDeployable, ALPHA);

        //One block per group: max distance + a toggle and color for each category of the group
        //(kWDGroupWeapons / kWDGroupEquipment / kWDGroupGear / kWDGroupAttachments list them)
        auto GroupBlock = [&](WDItemGroup group, const auto& categories) {
            ImGui::PushID(kWDItemGroupNames[group]);
            ImGui::TextUnformatted(kWDItemGroupNames[group]);
            ImGui::SliderInt("Max Distance", &world.GroupDistance[group], 0, 400);
            for (WDCategory category : categories) {
                auto& style = world.Categories.Style[category];
                ImGui::Checkbox(kWDCategoryNames[category], &style.Enable);
                ImGui::SameLine();
                ImGui::ColorEdit4("##Color", style.Color, ALPHA);
            }
            ImGui::PopID();
        };
        GroupBlock(WDG_Weapons, kWDGroupWeapons);
        GroupBlock(WDG_Equipment, kWDGroupEquipment);
        GroupBlock(WDG_Gear, kWDGroupGear);
        GroupBlock(WDG_Attachments, kWDGroupAttachments);
    }ImGui::EndGroup();

    //Vehicles example: one toggle per vehicle type (WDVehicleGroup) with a color for friendly / enemy / unknown
    ImGui::BeginGroup();
    {
        for (int group = 0; group < kWDVehicleGroupCount; group++) {
            auto& style = Settings->Visuals.Vehicles.Categories[group];
            ImGui::PushID(group);
            ImGui::Checkbox(kWDVehicleGroupNames[group], &style.Enable);
            ImGui::SameLine();
            ImGui::ColorEdit4("##Friendly", style.FriendlyColor, ALPHA);
            ImGui::SameLine();
            ImGui::ColorEdit4("##Enemy", style.EnemyColor, ALPHA);
            ImGui::SameLine();
            ImGui::ColorEdit4("##Unknown", style.UnknownColor, ALPHA);
            ImGui::PopID();
        }
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

    }ImGui::EndGroup();
}
#endif
