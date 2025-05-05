#include "../GUI.h"
#include  <fstream>
#include "../nav_elements.h"
#include "../../Imgui/imgui_internal.h"

#if IsFortnite
float animation_ = 0.0f;

#define ALPHA (ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar)
#define NO_ALPHA (ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar)

char cfgname[30] = "NewSetting";
int tabs = 0;
float tab_alpha = 0.f;
float tab_add;
int active_tab = 0;
void RenderMyMenu(bool* ShowMenu, SETTINGS* Settings, CConfig* g_pConfig) {

    const char* hitboxes[] = { ("Head"), ("Chest"), ("Body"), ("Random") };
    const char* predicts[] = { ("OFF"), ("Movement Only"), ("Bullet Drop Only"), ("Movement + Bullet Drop") };
    const char* boxes[] = { "Square", "Corner" };
    const char* rarities[] = { ("All"), ("Uncommon"), ("Rare"), ("Epic"), ("Legendary") };
    const char* LinesTypes[] = { ("Top"), ("Middle"), ("Bottom") };
    const char* AimTargetTypes[] = { ("Circle"), ("Triangle") };
    const char* WeaponTypes[] = { ("Shotgun"), ("SMG"), ("Pistol"), ("Bolt"), ("Sniper"), ("Rifle") , ("Others") }; 
    const char* OutScreen_items[] = { ("Triangle"), ("Dot") };

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

            if (elements::tab(icons, 3 == tabs, "E", ImVec2(50, 40))) tabs = 3;

            if (elements::tab(icons, 4 == tabs, "C", ImVec2(50, 40))) tabs = 4;

            if (elements::tab(icons, 5 == tabs, "D", ImVec2(50, 40))) tabs = 5;

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
                    elements::checkbox("Configuration By Weapon", &Settings->Aim.ByWeapon);
                    if (Settings->Aim.ByWeapon)
                    {
                        elements::combo("Weapon", &Settings->selectedWeapon, WeaponTypes, IM_ARRAYSIZE(WeaponTypes), IM_ARRAYSIZE(WeaponTypes));
                    }
                    if (Settings->Aim.ByWeapon)
                    {
                        elements::checkbox(("Enable"), &Settings->aimbot[Settings->selectedWeapon].Enable);
                        elements::sliderfloat(("Field Of View"), &Settings->aimbot[Settings->selectedWeapon].FOV, 1.0f, 30.0f, "%.1f");
                        elements::sliderfloat(("Smoothness"), &Settings->aimbot[Settings->selectedWeapon].Smooth, 1.0f, 30.0f, "%.1f");
                        elements::sliderint(("Max. Distance"), &Settings->aimbot[Settings->selectedWeapon].MaxDistance, 1, 1000, "%d");
                        elements::checkbox(("Save Target"), &Settings->aimbot[Settings->selectedWeapon].SaveTarget);
                        elements::checkbox(("Aim Lock"), &Settings->aimbot[Settings->selectedWeapon].AimLock);
                        elements::checkbox(("Visibility Check"), &Settings->aimbot[Settings->selectedWeapon].VisibilityCheck);
                        elements::checkbox(("Aim NPC"), &Settings->aimbot[Settings->selectedWeapon].NPC);
                        elements::checkbox(("Predict"), &Settings->aimbot[Settings->selectedWeapon].Predict);
                        elements::checkbox(("Weapon Only"), &Settings->aimbot[Settings->selectedWeapon].WeaponOnly);
                        elements::checkbox(("Ignore Downed"), &Settings->aimbot[Settings->selectedWeapon].IgnoreDowned);
                        elements::checkbox(("Humanized Smooth"), &Settings->aimbot[Settings->selectedWeapon].HumanizedSmooth);
                        elements::sliderfloat(("Humanized Smooth Percent"), &Settings->aimbot[Settings->selectedWeapon].HumanizedSmoothPercent, 1.0f, 10.0f, "%.1f");

                        elements::combo(("Hitbox"), &Settings->Aim.HitBox, hitboxes, IM_ARRAYSIZE(hitboxes));
                    }
                    else
                    {
                        elements::checkbox(("Enable"), &Settings->Aim.Enable);
                        elements::sliderfloat(("Field Of View"), &Settings->Aim.FOV, 1.0f, 30.0f, "%.1f");
                        elements::sliderfloat(("Smoothness"), &Settings->Aim.Smooth, 1.0f, 30.0f, "%.1f");
                        elements::sliderint(("Max. Distance"), &Settings->Aim.MaxDistance, 1, 1000, "%d");
                        elements::checkbox(("Save Target"), &Settings->Aim.SaveTarget);
                        elements::checkbox(("Aim Lock"), &Settings->Aim.AimLock);
                        elements::checkbox(("Visibility Check"), &Settings->Aim.VisibilityCheck);
                        elements::checkbox(("Aim NPC"), &Settings->Aim.NPC);
                        elements::checkbox(("Predict"), &Settings->Aim.Predict);
                        elements::checkbox(("Weapon Only"), &Settings->Aim.WeaponOnly);
                        elements::checkbox(("Ignore Downed"), &Settings->Aim.IgnoreDowned);
                        elements::checkbox(("Humanized Smooth"), &Settings->Aim.HumanizedSmooth);
                        elements::sliderfloat(("Humanized Smooth Percent"), &Settings->Aim.HumanizedSmoothPercent, 1.0f, 10.0f, "%.1f");

                        elements::combo(("Hitbox"), &Settings->Aim.HitBox, hitboxes, IM_ARRAYSIZE(hitboxes));
                    }
                }
                elements::endchild();
            }
            ImGui::EndGroup();

            ImGui::SameLine();

            ImGui::BeginGroup();
            {
                elements::beginchild("Aim #2", ImVec2(305, 433), true);
                {
                    elements::checkbox(("Draw FOV"), &Settings->Aim.DrawFov);
                    elements::checkbox(("Draw CrossHair"), &Settings->Aim.DrawCrossHair);
                    elements::checkbox(("Draw Target"), &Settings->Aim.DrawTarget);
                    elements::combo(("Draw Target Type"), &Settings->Aim.DrawTargetType, AimTargetTypes, IM_ARRAYSIZE(AimTargetTypes));
                    elements::sliderfloat(("Aim Draw Thickness"), &Settings->Aim.DrawThickness, 1.0f, 5.0f, "%.1f");

                    elements::checkbox("TriggerBot By Weapon", &Settings->Trigger.ByWeapon);
                    if (Settings->Trigger.ByWeapon)
                    {
                        elements::combo("Weapon##TriggerBot", &Settings->selectedWeapon, WeaponTypes, IM_ARRAYSIZE(WeaponTypes), IM_ARRAYSIZE(WeaponTypes));
                    }
                    if (Settings->Trigger.ByWeapon)
                    {
                        elements::checkbox(("TriggerBot"), &Settings->triggerbot[Settings->selectedWeapon].Enable);
                        elements::sliderint("Trigger Max Distance", &Settings->triggerbot[Settings->selectedWeapon].MaxDistance, 1, 1000, "%d");
                        elements::sliderint("Trigger Max Delay", &Settings->triggerbot[Settings->selectedWeapon].Delay, 1, 5000, "%d");
                    }
                    else
                    {
                        elements::checkbox(("TriggerBot"), &Settings->Trigger.Enable);
                        elements::sliderint("Trigger Max Distance", &Settings->Trigger.MaxDistance, 1, 1000, "%d");
                        elements::sliderint("Trigger Max Delay", &Settings->Trigger.Delay, 1, 5000, "%d");
                    }

                    
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
                    elements::checkbox(("Enable"), &Settings->Visuals.Players.Enable);
                    elements::checkbox(("Remove Downed"), &Settings->Visuals.Players.RemoveDowned);
                    elements::checkbox(("Head Circle"), &Settings->Visuals.Players.HeadCircle);
                    elements::checkbox(("Distance"), &Settings->Visuals.Players.Distance);
                    elements::checkbox(("NickName"), &Settings->Visuals.Players.NickName);
                    elements::checkbox(("Weapon"), &Settings->Visuals.Players.Weapon);
                    elements::checkbox(("Weapon Ammo"), &Settings->Visuals.Players.WeaponAmmo);
                    elements::checkbox(("Weapon Rarity"), &Settings->Visuals.Players.WeaponRarity);
                }
                elements::endchild();

            }
            ImGui::EndGroup();

            ImGui::SameLine();

            ImGui::BeginGroup();
            {
                elements::beginchild("Players #2", ImVec2(305, 433), true);
                {
                    elements::sliderint(("Max Distance"), &Settings->Visuals.Players.MaxDistance, 1, 1000, "%d");
                    elements::sliderfloat(("Font Size"), &Settings->Visuals.Players.FontSize, 10.0f, 25.0f, "%.1f");
                    elements::checkbox(("Box"), &Settings->Visuals.Players.Box);
                    elements::sliderfloat(("Box Thickness"), &Settings->Visuals.Players.BoxThickness, 1.0f, 5.0f, "%.1f");
                    elements::combo(("Box Type"), &Settings->Visuals.Players.BoxType, boxes, IM_ARRAYSIZE(boxes));
                    elements::checkbox(("Lines"), &Settings->Visuals.Players.Lines);
                    elements::sliderfloat(("Lines Thickness"), &Settings->Visuals.Players.LinesThickness, 1.0f, 5.0f, "%.1f");
                    elements::combo(("Lines Position"), &Settings->Visuals.Players.LinesPosition, LinesTypes, IM_ARRAYSIZE(LinesTypes));
                    elements::checkbox(("Skeleton"), &Settings->Visuals.Players.Skeleton);
                    elements::sliderfloat(("Skeleton Thickness"), &Settings->Visuals.Players.SkeletonThickness, 1.0f, 5.0f, "%.1f");
                }
                elements::endchild();

            }
            ImGui::EndGroup();

        }
        else if (active_tab == 2) {

            ImGui::BeginGroup();
            {
                elements::beginchild("Items #1", ImVec2(305, 433), true);
                {
                    elements::checkbox(("Enable Items"), &Settings->Visuals.World.Enable);
                    elements::checkbox(("Chest"), &Settings->Visuals.World.Chest);
                    elements::checkbox(("Vehicle"), &Settings->Visuals.World.Vehicle);
                    elements::checkbox(("Ammo"), &Settings->Visuals.World.Ammo);
                    elements::checkbox(("Weapon"), &Settings->Visuals.World.Weapon);
                    elements::checkbox(("Consumable"), &Settings->Visuals.World.Consumable);
                    elements::checkbox(("Material"), &Settings->Visuals.World.Material);
                    elements::checkbox(("Supply Drop"), &Settings->Visuals.World.SupplyDrop);
                }
                elements::endchild();

            }
            ImGui::EndGroup();

            ImGui::SameLine();

            ImGui::BeginGroup();
            {
                elements::beginchild("Items #2", ImVec2(305, 433), true);
                {
                    elements::sliderint(("Max. Distance"), &Settings->Visuals.World.MaxDistance, 0, 1000, "%d");
                    elements::sliderfloat(("Font Size"), &Settings->Visuals.World.FontSize, 10.f, 25.f, "%.1f");
                    elements::combo(("Weapon Rarity"), &Settings->Visuals.World.Rarity, rarities, IM_ARRAYSIZE(rarities), 5);
                }
                elements::endchild();

            }
            ImGui::EndGroup();

        }
        else if (active_tab == 3) {

            ImGui::BeginGroup();
            {
                elements::beginchild("Radar #1", ImVec2(305, 433), true);
                {
                    elements::checkbox(("Enable Radar"), &Settings->Visuals.Radar.Enable);
                    elements::checkbox(("Change Visible Enemies Color"), &Settings->Visuals.Radar.VisibleColor);
                    elements::checkbox(("Change Closest Enemies Color"), &Settings->Visuals.Radar.ClosestColor);
                    elements::checkbox("Enable OutScreen", &Settings->Visuals.Players.OutScreen);
                }
                elements::endchild();

            }
            ImGui::EndGroup();

            ImGui::SameLine();

            ImGui::BeginGroup();
            {
                elements::beginchild("Radar #2", ImVec2(305, 433), true);
                {
                    elements::sliderint(("Position Hor."), &Settings->Visuals.Radar.PositionX, 1, 3000);
                    elements::sliderint(("Position Vert."), &Settings->Visuals.Radar.PositionY, 1, 3000);
                    elements::sliderint(("Radar Zoom"), &Settings->Visuals.Radar.Zoom, 1, 100);
                    elements::sliderint(("Radar Size"), &Settings->Visuals.Radar.Size, 50, 500);
                    elements::sliderint(("Closest Distance"), &Settings->Visuals.Radar.ClosestDistance, 1, 200);
                    elements::combo("OutScreen Type", &Settings->Visuals.Players.OutScreenType, OutScreen_items, IM_ARRAYSIZE(OutScreen_items), IM_ARRAYSIZE(OutScreen_items));
                    elements::sliderint("OutScreen FOV", &Settings->Visuals.Players.OutScreenFov, 1, 100);
                    elements::sliderint("OutScreen Size", &Settings->Visuals.Players.OutScreenSize, 1, 20);
                    elements::sliderfloat("OutScreen Thickness", &Settings->Visuals.Players.OutScreenThickness, 1, 5);

                }
                elements::endchild();

            }
            ImGui::EndGroup();

        }
        else if (active_tab == 4) {

            ImGui::BeginGroup();
            {
                elements::beginchild("Aim #1", ImVec2(305, 433), true);
                {
                    ImGui::ColorEdit4(("Aim Fov"), (float*)&Settings->Colors.Aim.Fov, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

                    ImGui::ColorEdit4(("Aim Crosshair"), (float*)&Settings->Colors.Aim.Fov, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

                    ImGui::ColorEdit4(("Aim Target"), (float*)&Settings->Colors.Aim.Target, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                }
                elements::endchild();

            }
            ImGui::EndGroup();

            ImGui::SameLine();

            ImGui::BeginGroup();
            {
                elements::beginchild("Visuals #2", ImVec2(305, 433), true);
                {
                    ImGui::ColorEdit4(("Player Visible"), (float*)&Settings->Colors.Players.Visible, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

                    ImGui::ColorEdit4(("Player Non Visible"), (float*)&Settings->Colors.Players.NonVisible, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                    
                    ImGui::ColorEdit4(("Player NickName"), (float*)&Settings->Colors.Players.NickName, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                    
                    ImGui::ColorEdit4(("Player Weapon"), (float*)&Settings->Colors.Players.Weapon, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

                    ImGui::ColorEdit4(("Radar Visible"), (float*)&Settings->Colors.Radar.Visible, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

                    ImGui::ColorEdit4(("Radar Enemies"), (float*)&Settings->Colors.Radar.Enemies, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

                    ImGui::ColorEdit4(("Radar Closest"), (float*)&Settings->Colors.Radar.Closest, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                }
                elements::endchild();

            }
            ImGui::EndGroup();

        }
        else if (active_tab == 5) {

            ImGui::BeginGroup();
            {
                elements::beginchild("HotKeys", ImVec2(305, 433), true);
                {
                    elements::HotKey(("Cheat Menu Key"), &Settings->Keys.Menu, { 142, 29 });
                    elements::HotKey(("Trigger Key"), &Settings->Keys.Trigger, { 142, 29 });
                    elements::HotKey(("Aimbot Hold Primary"), &Settings->Keys.Aim.HoldPrimary, { 142, 29 });
                    elements::HotKey(("Aimbot Hold Secondary"), &Settings->Keys.Aim.HoldSecondary, { 142, 29 });
                    elements::HotKey(("Aimbot Switch To Head"), &Settings->Keys.Aim.SwitchToHead, { 142, 29 });
                    elements::HotKey(("Aimbot Toggle"), &Settings->Keys.Aim.Toggle, { 142, 29 });
                    elements::HotKey(("Visuals Toggle Players"), &Settings->Keys.Visuals.TogglePlayers, { 142, 29 });
                    elements::HotKey(("Visuals Toggle Items"), &Settings->Keys.Visuals.ToggleWorld, { 142, 29 });
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
                    elements::checkbox("Instant Switch", &Settings->Misc.InstantSwitch);
                    elements::checkbox("Aim While Jumping", &Settings->Misc.AimWhileJumping);
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