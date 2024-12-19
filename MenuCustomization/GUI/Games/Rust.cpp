#include "../GUI.h"
#include  <fstream>
#include "../nav_elements.h"
#include "../../Imgui/imgui_internal.h"

#if IsRust
float animation_ = 0.0f;

#define ALPHA (ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar)
#define NO_ALPHA (ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar)

char cfgname[30] = "NewSetting";
int tabs = 0;
float tab_alpha = 0.f;
float tab_add;
int active_tab = 0;

void UpdateComboItems(bool* values, std::vector<std::string> list, std::string& out) {
    out = "";
    std::vector<std::string> vec;
    for (int i = 0; i < list.size(); i++) {
        if (values[i]) {
            vec.push_back(list[i]);
        }
    }

    for (int i = 0; i < vec.size(); i++) {
        if (vec.size() == 1)
            out += vec.at(i);
        else if (!(i == vec.size() - 1)) {
            out += vec.at(i) + ",";
        }
        else
            out += vec.at(i);
    }
}

void MultiCombo(const char* label, bool* values, std::vector<std::string> list) {
    elements::multi_combo(label, values, list);
    bool current = values[0];
    if (elements::button(std::string("Select/Unselect All##" + list[0] + list[1]).c_str(), ImVec2(126.f, 26.f), current ? ("Select") : ("Unselect"))) {
        for (int i = 0; i < list.size(); i++) {
            values[i] = !current;
        }
    }

}

void RenderMyMenu(bool* ShowMenu, SETTINGS* Settings, CConfig* g_pConfig) {

    const char* hitboxes[] = { ("Head"), ("Chest"), ("Body"), ("Random") };
    const char* predicts[] = { ("OFF"), ("Movement Only"), ("Bullet Drop Only"), ("Movement + Bullet Drop") };
    const char* boxes[] = { "Square", "Corner" };
    const char* rarities[] = { ("All"), ("Uncommon"), ("Rare"), ("Epic"), ("Legendary") };
    const char* LinesTypes[] = { ("Top"), ("Middle"), ("Bottom") };
    const char* AimTargetTypes[] = { ("Circle"), ("Triangle") };
    const char* HealthTypes[] = { ("Horizontal"), ("Vertical") };
    const char* OutScreen_items[] = { ("Triangle"), ("Dot") };
    const char* FontTypes[] = { ("Burbank"), ("Verdana") };

    const char* tab_name = "";
    const char* tab_icon = "";

    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGuiStyle& s = ImGui::GetStyle();
    ImGui::Begin("Menu", ShowMenu, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBringToFrontOnFocus);
    {
        const auto& p = ImGui::GetWindowPos();
        ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(0 + p.x, +p.y), ImVec2(750 + p.x, 470 + p.y), ImGui::GetColorU32(ImVec4(ImColor(15, 15, 15, 255))));

        ImGui::GetWindowDrawList()->AddLine(ImVec2(85 + p.x, 0 + p.y), ImVec2(85 + p.x, 470 + p.y), ImGui::GetColorU32(ImVec4(ImColor(20, 20, 20, 255))), 2.f);
        ImGui::GetWindowDrawList()->AddLine(ImVec2(13 + p.x, 73 + p.y), ImVec2(85 - 10 + p.x, 73 + p.y), ImGui::GetColorU32(ImVec4(ImColor(20, 20, 20, 255))), 2.f);

        ImGui::GetForegroundDrawList()->AddImageRounded(logo, ImVec2(p.x + 20, p.y + 20), ImVec2(p.x + 120, p.y + 170), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255) /*color*/, 0 /*rounding*/);

        ImGui::SetCursorPos(ImVec2(18, 90));

        ImGui::BeginGroup();
        {

            if (elements::tab(icons, 0 == tabs, "A", ImVec2(50, 40))) tabs = 0;

            if (elements::tab(icons, 1 == tabs, "B##1", ImVec2(50, 40))) tabs = 1;

            if (elements::tab(icons, 2 == tabs, "B##2", ImVec2(50, 40))) tabs = 2;

            if (elements::tab(icons, 3 == tabs, "B##3", ImVec2(50, 40))) tabs = 3;

            if (elements::tab(icons, 4 == tabs, "E", ImVec2(50, 40))) tabs = 4;

            if (elements::tab(icons, 5 == tabs, "C", ImVec2(50, 40))) tabs = 5;

            if (elements::tab(icons, 6 == tabs, "D", ImVec2(50, 40))) tabs = 6;

        }
        ImGui::EndGroup();

        tab_alpha = ImClamp(tab_alpha + (7.f * ImGui::GetIO().DeltaTime * (tabs == active_tab ? 1.f : -1.f)), 0.f, 1.f);
        tab_add = ImClamp(tab_add + (std::round(350.f) * ImGui::GetIO().DeltaTime * (tabs == active_tab ? 1.f : -1.f)), 0.f, 1.f);

        if (tab_alpha == 0.f && tab_add == 0.f)
            active_tab = tabs;

        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tab_alpha * s.Alpha);

        ImGui::SetCursorPos(ImVec2(144 - tab_alpha * 40, 18));

        if (active_tab == 0) {

            ImGui::BeginGroup();
            {
                elements::beginchild("Aim #1", ImVec2(305, 433), true);
                {
                    elements::checkbox(("Enable"), &Settings->Aimbot.Enable);
                    elements::checkbox(("Save Target"), &Settings->Aimbot.SaveTarget);
                    elements::checkbox(("Ignore Wounded"), &Settings->Aimbot.IgnoreWounded);
                    elements::checkbox(("Aim Lock"), &Settings->Aimbot.AimLock);
                    elements::checkbox(("Visibility Check"), &Settings->Aimbot.VisibilityCheck);
                    elements::checkbox(("Draw FOV"), &Settings->Aimbot.DrawFov);
                    elements::checkbox(("Draw CrossHair"), &Settings->Aimbot.DrawCrossHair);
                    elements::checkbox(("Draw Target"), &Settings->Aimbot.DrawTarget);
                }
                elements::endchild();
            }
            ImGui::EndGroup();

            ImGui::SameLine();

            ImGui::BeginGroup();
            {
                elements::beginchild("Aim #2", ImVec2(305, 433), true);
                {
                    elements::sliderint(("Max. Distance##Aim"), &Settings->Aimbot.MaxDistance, 0, 1000);
                    elements::sliderfloat(("Aim Draw Thickness"), &Settings->Aimbot.DrawThickness, 1.0f, 5.0f);
                    elements::combo(("Predict"), &Settings->Aimbot.Predict, predicts, IM_ARRAYSIZE(predicts));
                    elements::combo(("Hitbox"), &Settings->Aimbot.HitBox, hitboxes, IM_ARRAYSIZE(hitboxes), 4);
                    elements::combo(("Draw Target Type"), &Settings->Aimbot.DrawTargetType, AimTargetTypes, IM_ARRAYSIZE(AimTargetTypes), 2);
                    elements::sliderint(("FOV"), &Settings->Aimbot.FOV, 1, 200);
                    elements::sliderfloat(("Min. Smooth"), &Settings->Aimbot.min_time, 0.1f, 1.f);
                    elements::sliderfloat(("Max. Smooth"), &Settings->Aimbot.max_time, 0.1f, 1.f);
                }
                elements::endchild();

            }
            ImGui::EndGroup();
        }
        else if (active_tab == 1) {

            ImGui::BeginGroup();
            {
                elements::beginchild("Players #1", ImVec2(305, 433), true);
                {
                    elements::checkbox(("Enable Players"), &Settings->Visuals.Players.Enable);
                    elements::checkbox(("Remove Sleeping"), &Settings->Visuals.Players.RemoveSleeping);
                    elements::checkbox(("Team Check"), &Settings->Visuals.Players.TeamCheck);
                    elements::checkbox(("Head Circle"), &Settings->Visuals.Players.HeadCircle);
                    elements::checkbox(("Nickname"), &Settings->Visuals.Players.NickName);
                    elements::checkbox(("Distance"), &Settings->Visuals.Players.Distance);
                    elements::checkbox(("Weapon"), &Settings->Visuals.Players.Weapon);
                    elements::checkbox(("Chams"), &Settings->Visuals.Players.Chams);
                    elements::checkbox(("HotBar"), &Settings->Visuals.Players.HotBar);
                    
                }
                elements::endchild();

            }
            ImGui::EndGroup();

            ImGui::SameLine();

            ImGui::BeginGroup();
            {
                elements::beginchild("Players #2", ImVec2(305, 433), true);
                {
                    elements::sliderint(("Max. Distance##ESP"), &Settings->Visuals.Players.MaxDistance, 0, 1000);
                    elements::sliderfloat(("Font Size"), &Settings->Visuals.Players.FontSize, 10.0f, 25.0f);
                    elements::sliderfloat(("Skeleton Thickness"), &Settings->Visuals.Players.SkeletonThickness, 1.0f, 5.0f);
                    elements::combo(("Box Type"), &Settings->Visuals.Players.BoxType, boxes, IM_ARRAYSIZE(boxes), 2);
                    elements::sliderfloat(("Box Thickness"), &Settings->Visuals.Players.BoxThickness, 1.0f, 5.0f);
                    elements::combo(("Lines Position"), &Settings->Visuals.Players.LinesPosition, LinesTypes, IM_ARRAYSIZE(LinesTypes), 3);
                    elements::sliderfloat(("Lines Thickness"), &Settings->Visuals.Players.LinesThickness, 1.0f, 5.0f);
                    if (Settings->Visuals.Players.HotBar) {
                        elements::sliderfloat("HotBar Font Size", &Settings->Visuals.Players.HotBarFontSize, 10.0f, 25.0f, "%.1f");
                        elements::sliderint(("HotBar Pos X"), &Settings->Visuals.Players.HotBarX, 1, 3000);
                        elements::sliderint(("HotBar Pos Y"), &Settings->Visuals.Players.HotBarY, 1, 3000);
                    }
                }
                elements::endchild();

            }
            ImGui::EndGroup();

        }
        else if (active_tab == 2) {

            ImGui::BeginGroup();
            {
                elements::beginchild("NPCs #1", ImVec2(305, 433), true);
                {
                    elements::checkbox(("Enable NPCs"), &Settings->Visuals.NPC.Enable);
                    elements::checkbox(("Head Circle"), &Settings->Visuals.NPC.HeadCircle);
                    elements::checkbox(("Distance"), &Settings->Visuals.NPC.Distance);
                }
                elements::endchild();

            }
            ImGui::EndGroup();

            ImGui::SameLine();

            ImGui::BeginGroup();
            {
                elements::beginchild("NPCs #2", ImVec2(305, 433), true);
                {
                    elements::sliderint(("Max. Distance##ESP"), &Settings->Visuals.NPC.MaxDistance, 0, 1000);
                    elements::sliderfloat(("Font Size"), &Settings->Visuals.NPC.FontSize, 10.0f, 25.0f);
                    elements::sliderfloat(("Skeleton Thickness"), &Settings->Visuals.NPC.SkeletonThickness, 1.0f, 5.0f);
                    elements::combo(("Box Type"), &Settings->Visuals.NPC.BoxType, boxes, IM_ARRAYSIZE(boxes), 2);
                    elements::sliderfloat(("Box Thickness"), &Settings->Visuals.NPC.BoxThickness, 1.0f, 5.0f);
                    elements::combo(("Lines Position"), &Settings->Visuals.NPC.LinesPosition, LinesTypes, IM_ARRAYSIZE(LinesTypes), 3);
                    elements::sliderfloat(("Lines Thickness"), &Settings->Visuals.NPC.LinesThickness, 1.0f, 5.0f);
                }
                elements::endchild();

            }
            ImGui::EndGroup();

        }
        else if (active_tab == 3) {

            ImGui::BeginGroup();
            {
                elements::beginchild("Items #1", ImVec2(305, 433), true);
                {
                    elements::checkbox(("Corpse"), &Settings->Visuals.Items.Corpse);
                    elements::checkbox(("Hemp"), &Settings->Visuals.Items.Hemp);
                    elements::checkbox(("World Drop"), &Settings->Visuals.Items.WorldDrop); 
                    if (Settings->Visuals.Items.WorldDrop) {
                        std::vector<std::string> world_list = { std::string("Weapon"), std::string("Construction"), std::string("Items")
                            , std::string("Resources"), std::string("Attire"), std::string("Tool"), std::string("Medical"), std::string("Food")
                        , std::string("Ammunition") , std::string("Traps") , std::string("Misc") , std::string("Common") , std::string("Component") , std::string("Search") };
                        MultiCombo("", Settings->Visuals.Items.WorldType, world_list);
                    }
                    elements::checkbox(("AirDrop"), &Settings->Visuals.Items.AirDrop);
                    elements::checkbox(("Cupboard"), &Settings->Visuals.Items.Cupboard);
                    elements::checkbox(("Ore"), &Settings->Visuals.Items.Ore);
                }
                elements::endchild();

            }
            ImGui::EndGroup();

            ImGui::SameLine();

            ImGui::BeginGroup();
            {
                elements::beginchild("Items #2", ImVec2(305, 433), true);
                {
                    elements::sliderint(("Max. Distance"), &Settings->Visuals.Items.MaxDistance, 0, 100);
                    elements::sliderfloat(("Font Size"), &Settings->Visuals.Items.FontSize, 0, 100);
                    if (Settings->Visuals.Items.Ore) {
                        std::vector<std::string> ore_list = { std::string(("Stone")), std::string(("Metal")), std::string(("Sulfur")) };
                        MultiCombo(("Ore Type"), Settings->Visuals.Items.OreType, ore_list);
                    }
                    elements::checkbox(("Traps"), &Settings->Visuals.Items.Traps);
                    if (Settings->Visuals.Items.Traps) {
                        std::vector<std::string> trap_list = { std::string(("SAM Site")), std::string(("Auto Turret")), std::string(("Gun Trap")), std::string(("Flame Turret")) };
                        MultiCombo(("Trap Type"), Settings->Visuals.Items.TrapsType, trap_list);
                    }
                    elements::checkbox(("Vehicles"), &Settings->Visuals.Items.Vehicles);
                    if (Settings->Visuals.Items.Vehicles) {
                        std::vector<std::string> vehicles_list = { std::string(("Minicopter")), std::string(("Car")), std::string(("Transport Heli")), std::string(("Patrol Heli")), std::string(("Horse")), std::string(("Boat")), std::string(("RHIB")), std::string(("Bradley")) };
                        MultiCombo(("Vehicle Type"), Settings->Visuals.Items.VehiclesType, vehicles_list);
                    }
                    elements::checkbox(("Animals"), &Settings->Visuals.Items.Animals);
                    if (Settings->Visuals.Items.Animals) {
                        std::vector<std::string> animals_list = { std::string(("Wolf")), std::string(("Bear")), std::string(("Stag")), std::string(("Chicken")), std::string(("Boar")) };
                        MultiCombo(("Animal Type"), Settings->Visuals.Items.AnimalsType, animals_list);
                    }
                    elements::checkbox(("Crate"), &Settings->Visuals.Items.Crate);
                    if (Settings->Visuals.Items.Crate) {
                        std::vector<std::string> crate_list = { std::string(("Small Stash")), std::string(("Normal")), std::string(("Elite")), std::string(("Food")), std::string(("Medical")) };
                        MultiCombo(("Crate Type"), Settings->Visuals.Items.CrateType, crate_list);
                    }
                    elements::checkbox(("Barrel"), &Settings->Visuals.Items.Barrel);
                    if (Settings->Visuals.Items.Barrel) {
                        std::vector<std::string> barrel_list = { std::string(("Loot Barrel 1")), std::string(("Loot Barrel 2")), std::string(("Oil Barrel")) };
                        MultiCombo(("Barrel Type"), Settings->Visuals.Items.BarrelType, barrel_list);
                    }
                }
                elements::endchild();

            }
            ImGui::EndGroup();

        }
        else if (active_tab == 4) {

            ImGui::BeginGroup();
            {
                elements::beginchild("Radar #1", ImVec2(305, 433), true);
                {
                    elements::checkbox(("Enable Radar"), &Settings->Visuals.Radar.Enable);
                    elements::checkbox(("Change Visible Enemies Color"), &Settings->Visuals.Radar.VisibleColor);
                    elements::checkbox("Enable OutScreen", &Settings->Visuals.Players.OutScreen);
                    elements::sliderint(("Position Hor."), &Settings->Visuals.Radar.PositionX, 1, 3000);
                    elements::sliderint(("Position Vert."), &Settings->Visuals.Radar.PositionY, 1, 3000);
                    elements::sliderint(("Radar Zoom"), &Settings->Visuals.Radar.Zoom, 1, 100);
                    elements::sliderint(("Radar Size"), &Settings->Visuals.Radar.Size, 50, 500);
                    elements::combo("OutScreen Type", &Settings->Visuals.Players.OutScreenType, OutScreen_items, IM_ARRAYSIZE(OutScreen_items), IM_ARRAYSIZE(OutScreen_items));
                    elements::sliderint("OutScreen FOV", &Settings->Visuals.Players.OutScreenFov, 1, 100);
                    elements::sliderint("OutScreen Size", &Settings->Visuals.Players.OutScreenSize, 1, 20);
                    elements::sliderfloat("OutScreen Thickness", &Settings->Visuals.Players.OutScreenThickness, 1, 5);
                }
                elements::endchild();

            }
            ImGui::EndGroup();

            ImGui::SameLine();

            ImGui::BeginGroup();
            {
                elements::beginchild("Misc", ImVec2(305, 433), true);
                {
                    elements::checkbox(("Silent"), &Settings->Misc.Silent);
                    elements::sliderint(("Silent Hit Chance"), &Settings->Misc.SilentHitChance, 1, 100);
                    elements::sliderint(("Silent HS Chance"), &Settings->Misc.SilentHeadShotChance, 1, 100);
                    elements::checkbox(("Attack On Mountable"), &Settings->Misc.AttackOnMountable);
                    elements::checkbox(("Recoil Control"), &Settings->Misc.RecoilControl);
                    elements::sliderint(("Recoil Scale"), &Settings->Misc.RecoilScale, 1, 100);
                    elements::checkbox(("SpreadControl"), &Settings->Misc.SpreadControl);
                    elements::checkbox(("SwayControl"), &Settings->Misc.SwayControl);
                    elements::checkbox(("BulletSpeed"), &Settings->Misc.BulletSpeed);
                    elements::checkbox(("FullAuto"), &Settings->Misc.FullAuto);
                    elements::checkbox(("SuperBow"), &Settings->Misc.SuperBow);
                    elements::checkbox(("InstantEoka"), &Settings->Misc.InstantEoka);
                    elements::checkbox(("ShootAir"), &Settings->Misc.ShootAir);
                    elements::checkbox(("Extended Melee"), &Settings->Misc.ExtendedMelee);
                    elements::checkbox(("Gravity Multiplier"), &Settings->Misc.GravityMultiplier);
                    elements::checkbox(("No Fall"), &Settings->Misc.NoFall);
                    elements::checkbox(("Spiderman"), &Settings->Misc.Spiderman);
                    elements::checkbox(("High Jump"), &Settings->Misc.HighJump);
                    elements::checkbox(("Admin Flag"), &Settings->Misc.AdminFlag);
                    elements::checkbox(("Third Person"), &Settings->Misc.ThirdPerson);
                    elements::checkbox(("Bright Night"), &Settings->Misc.nightSky);
                    elements::checkbox(("Time Change"), &Settings->Misc.night_mode);
                    if (Settings->Misc.night_mode)
                    {
                        elements::sliderfloat(("Admin Time"), &Settings->Misc.time, 0.f, 24.f, "%.1f");
                    }
                    elements::checkbox(("FOV Changer"), &Settings->Misc.FovChanger);
                    if (Settings->Misc.FovChanger)
                    {
                        elements::sliderfloat(("FOV Slider"), &Settings->Misc.FovChangerValue, 10.f, 180.f, "%.1f");
                    }
                    elements::checkbox(("Zoom FOV"), &Settings->Misc.ZoomFov);
                    if (Settings->Misc.ZoomFov)
                    {
                        elements::sliderfloat(("Zoom Slider"), &Settings->Misc.ZoomFovValue, 50.f, 300.f, "%.1f");
                    }
                }
                elements::endchild();

            }
            ImGui::EndGroup();

        }
        else if (active_tab == 5) {

            ImGui::BeginGroup();
            {
                elements::beginchild("Colors #1", ImVec2(305, 433), true);
                {
                    ImGui::ColorEdit4(("Aim Fov"), (float*)&Settings->Colors.Aimbot.Fov, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

                    ImGui::ColorEdit4(("Aim Crosshair"), (float*)&Settings->Colors.Aimbot.Fov, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

                    ImGui::ColorEdit4(("Aim Target"), (float*)&Settings->Colors.Aimbot.Target, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
               
                    ImGui::ColorEdit4(("Player Visible"), (float*)&Settings->Colors.Players.Visible, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

                    ImGui::ColorEdit4(("Player Non Visible"), (float*)&Settings->Colors.Players.NonVisible, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

                    ImGui::ColorEdit4(("Player NickName"), (float*)&Settings->Colors.Players.NickName, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

                    ImGui::ColorEdit4(("Radar Visible"), (float*)&Settings->Colors.Radar.Visible, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

                    ImGui::ColorEdit4(("Radar Enemies"), (float*)&Settings->Colors.Radar.Enemies, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

                    ImGui::ColorEdit4(("NPC Visible"), (float*)&Settings->Colors.NPC.Visible, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

                    ImGui::ColorEdit4(("NPC Non Visible"), (float*)&Settings->Colors.NPC.NonVisible, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

                    ImGui::ColorEdit4(("NPC Distance"), (float*)&Settings->Colors.NPC.Distance, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

                }
                elements::endchild();

            }
            ImGui::EndGroup();

            ImGui::SameLine();

            ImGui::BeginGroup();
            {
                elements::beginchild("Colors #2", ImVec2(305, 433), true);
                {
                    ImGui::ColorEdit4(("Ore (items)"), Settings->Colors.Items.Ore, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                    ImGui::ColorEdit4(("Hemp (items)"), Settings->Colors.Items.Hemp, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                    ImGui::ColorEdit4(("AirDrop (items)"), Settings->Colors.Items.AirDrop, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                    ImGui::ColorEdit4(("World Drop (items)"), Settings->Colors.Items.WorldDrop, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                    ImGui::ColorEdit4(("Traps (items)"), Settings->Colors.Items.Traps, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                    ImGui::ColorEdit4(("Vehicles (items)"), Settings->Colors.Items.Vehicles, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                    ImGui::ColorEdit4(("Animals (items)"), Settings->Colors.Items.Animals, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                    ImGui::ColorEdit4(("Cupboard (items)"), Settings->Colors.Items.Cupboard, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                    ImGui::ColorEdit4(("Crate (items)"), Settings->Colors.Items.Crate, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                    ImGui::ColorEdit4(("Barrel (items)"), Settings->Colors.Items.Barrel, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                }
                elements::endchild();

            }
            ImGui::EndGroup();

        }
        else if (active_tab == 6) {

            ImGui::BeginGroup();
            {
                elements::beginchild("HotKeys", ImVec2(305, 433), true);
                {
                    elements::HotKey(("Cheat Menu Key"), &Settings->Keys.Menu, { 142, 29 });
                    elements::HotKey(("Zoom Hold"), &Settings->Keys.ZoomFovHold, { 142, 29 });
                    elements::HotKey(("Admin Flag Key"), &Settings->Keys.AdminFlag, { 142, 29 });
                    elements::HotKey(("Admin Flag Hold"), &Settings->Keys.AdminFlagHold, { 142, 29 });

                    elements::HotKey(("Aimbot Hold Primary"), &Settings->Keys.Aimbot.HoldPrimary, { 142, 29 });
                    elements::HotKey(("Aimbot Hold Secondary"), &Settings->Keys.Aimbot.HoldSecondary, { 142, 29 });
                    elements::HotKey(("Aimbot Hold NPC"), &Settings->Keys.Aimbot.HoldNPC, { 142, 29 });
                    elements::HotKey(("Aimbot Switch To Head"), &Settings->Keys.Aimbot.SwitchToHead, { 142, 29 });
                    elements::HotKey(("Aimbot Toggle"), &Settings->Keys.Aimbot.Toggle, { 142, 29 });
                    elements::HotKey(("Visuals Toggle Players"), &Settings->Keys.Visuals.TogglePlayers, { 142, 29 });
                    elements::HotKey(("Visuals Toggle Items"), &Settings->Keys.Visuals.ToggleItems, { 142, 29 });
                    elements::HotKey(("Visuals Toggle Radar"), &Settings->Keys.Visuals.ToggleRadar, { 142, 29 });
                }
                elements::endchild();

            }
            ImGui::EndGroup();

            ImGui::SameLine();

            ImGui::BeginGroup();
            {
                elements::beginchild("Configs", ImVec2(305, 433), true);
                {
                    elements::combo(("Render Font Style"), &Settings->Configs.FontStyle, FontTypes, IM_ARRAYSIZE(FontTypes));

                    if (elements::combo(("Select Your Config"), &Settings->iConfig, g_pConfig->szConfigs.c_str(), g_pConfig->szConfigs.size())) {
                        if (Settings->oldiConfig != Settings->iConfig) {
                            g_pConfig->InitPath(&g_pConfig->vecConfigs[Settings->oldiConfig][0u]);
                            g_pConfig->Save();
                            Settings->oldiConfig = Settings->iConfig;
                            g_pConfig->InitPath(&g_pConfig->vecConfigs[Settings->iConfig][0u]);
                            g_pConfig->Read();
                        }
                    }

                    elements::input_text(("Config Name"), cfgname, 30);
                    if (elements::button(("New Config"), ImVec2(ImGui::CalcItemWidth() - 20, 33), false)) {
                        g_pConfig->CreateConfig(cfgname);
                    }
                }
                elements::endchild();

            }
            ImGui::EndGroup();

        }

        ImGui::PopStyleVar();

    }
    ImGui::End();

}
#endif