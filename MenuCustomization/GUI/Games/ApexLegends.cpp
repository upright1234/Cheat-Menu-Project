#include "../GUI.h"
#include  <fstream>
#include "../nav_elements.h"
#include "../../Imgui/imgui_internal.h"
#if IsApex
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
    const char* boxes[] = { "Square", "Corner" };
    const char* LinesTypes[] = { ("Top"), ("Middle"), ("Bottom") };
    const char* AimTargetTypes[] = { ("Circle"), ("Triangle") };
    const char* RarityTypes[] = { ("All"), ("Common"), ("Rare"), ("Epic"), ("Legendary") };
    const char* HealthTypes[] = { ("Horizontal"), ("Vertical") };
    const char* OutScreen_items[] = { ("Triangle"), ("Dot") };
    const char* GlowTypes[] = { ("Custom Color"), ("Single Color") };

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
                    elements::checkbox(("Enable Aim"), &Settings->Aimbot.Enable);
                    elements::checkbox(("Save Target"), &Settings->Aimbot.SaveTarget);
                    elements::checkbox(("Aim Lock"), &Settings->Aimbot.AimLock);
                    elements::checkbox(("Predict"), &Settings->Aimbot.Predict);
                    elements::checkbox(("Visibility Check"), &Settings->Aimbot.VisibilityCheck);
                    elements::checkbox(("Humanized Smooth"), &Settings->Aimbot.HumanizedSmooth);
                    elements::checkbox(("Recoil Control"), &Settings->Aimbot.RecoilControl);
                    elements::checkbox(("Ignore Knocked Players"), &Settings->Aimbot.IgnoreKnocked);
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
                    elements::combo(("Hitbox"), &Settings->Aimbot.HitBox, hitboxes, IM_ARRAYSIZE(hitboxes), 4);
                    elements::combo(("Draw Target Type"), &Settings->Aimbot.DrawTargetType, AimTargetTypes, IM_ARRAYSIZE(AimTargetTypes), 2);
                    elements::sliderint(("Recoil Slider"), &Settings->Aimbot.RecoilControlValue, 1, 100);
                    elements::sliderfloat(("FOV"), &Settings->Aimbot.FOV, 1.0f, 30.f);
                    elements::sliderfloat(("Smooth"), &Settings->Aimbot.Smooth, 1.f, 30.0f);
                    elements::sliderfloat(("Humanized Smooth##percent"), &Settings->Aimbot.HumanizedSmoothPercent, 1.f, 10.f);
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
                    elements::checkbox(("Ignore Knocked"), &Settings->Visuals.Players.IgnoreKnocked);
                    elements::checkbox(("Glow"), &Settings->Visuals.Players.Glow);
                    elements::checkbox(("Box"), &Settings->Visuals.Players.Box);
                    elements::checkbox(("Lines"), &Settings->Visuals.Players.Lines);
                    elements::checkbox(("Distance"), &Settings->Visuals.Players.Distance);
                    elements::checkbox(("NickName"), &Settings->Visuals.Players.NickName);
                    elements::checkbox(("Health"), &Settings->Visuals.Players.Health);
                    elements::checkbox(("Shield"), &Settings->Visuals.Players.Shield);
                    elements::checkbox(("Skeleton"), &Settings->Visuals.Players.Skeleton);
                    elements::checkbox(("Head Circle"), &Settings->Visuals.Players.HeadCircle);
                    elements::checkbox(("Weapon"), &Settings->Visuals.Players.HeadCircle);
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
                    elements::combo(("Glow Type"), &Settings->Visuals.Players.GlowType, GlowTypes, IM_ARRAYSIZE(GlowTypes), 2);
                    elements::combo(("Box Type"), &Settings->Visuals.Players.BoxType, boxes, IM_ARRAYSIZE(boxes), 2);
                    elements::sliderfloat(("Box Thickness"), &Settings->Visuals.Players.BoxThickness, 1.0f, 5.0f);
                    elements::combo(("Lines Position"), &Settings->Visuals.Players.LinesPosition, LinesTypes, IM_ARRAYSIZE(LinesTypes), 3);
                    elements::sliderfloat(("Lines Thickness"), &Settings->Visuals.Players.LinesThickness, 1.0f, 5.0f);
                    elements::combo(("Health Type"), &Settings->Visuals.Players.HealthType, HealthTypes, IM_ARRAYSIZE(HealthTypes), 2);
                    elements::sliderfloat(("Health Thickness"), &Settings->Visuals.Players.HealthThickness, 1.0f, 5.0f);
                    elements::combo(("Shield Type"), &Settings->Visuals.Players.ShieldType, HealthTypes, IM_ARRAYSIZE(HealthTypes), 2);
                    elements::sliderfloat(("Shield Thickness"), &Settings->Visuals.Players.ShieldThickness, 1.0f, 5.0f);
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
                    elements::checkbox(("Enable Items"), &Settings->Visuals.Items.Enable);
                    elements::checkbox(("Glow"), &Settings->Visuals.Items.Glow);
                    elements::checkbox(("Draw Text"), &Settings->Visuals.Items.Text);
                    elements::checkbox(("Deathbox"), &Settings->Visuals.Items.Deathbox);
                    elements::checkbox(("Vehicle"), &Settings->Visuals.Items.Vehicle);
                    elements::checkbox(("Weapon"), &Settings->Visuals.Items.Weapon);
                    elements::checkbox(("Ammo"), &Settings->Visuals.Items.Ammo);
                    elements::checkbox(("Attachments"), &Settings->Visuals.Items.Attachment);
                    elements::checkbox(("Grenades"), &Settings->Visuals.Items.Grenades);
                    elements::checkbox(("Medicine"), &Settings->Visuals.Items.Medicine);
                    elements::checkbox(("Helmet Shield"), &Settings->Visuals.Items.Helmet);
                    elements::checkbox(("Body Shield"), &Settings->Visuals.Items.Body);
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

                    elements::checkbox(("Knockdown Shield"), &Settings->Visuals.Items.Knockdown);
                    elements::combo(("Knockdown Rarity"), &Settings->Visuals.Items.KnockdownType, RarityTypes, IM_ARRAYSIZE(RarityTypes), 5);
                    elements::checkbox(("BackPack"), &Settings->Visuals.Items.Backpack);
                    elements::combo(("BackPack Rarity"), &Settings->Visuals.Items.BackpackType, RarityTypes, IM_ARRAYSIZE(RarityTypes), 5);
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
                    ImGui::ColorEdit4(("Aim Fov"), (float*)&Settings->Colors.Aimbot.Fov, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

                    ImGui::ColorEdit4(("Aim Crosshair"), (float*)&Settings->Colors.Aimbot.Fov, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

                    ImGui::ColorEdit4(("Aim Target"), (float*)&Settings->Colors.Aimbot.Target, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
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

                    ImGui::ColorEdit4(("Radar Visible"), (float*)&Settings->Colors.Radar.Visible, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

                    ImGui::ColorEdit4(("Radar Enemies"), (float*)&Settings->Colors.Radar.Enemies, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

                    ImGui::ColorEdit4(("Radar Closest"), (float*)&Settings->Colors.Radar.Closest, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                    
                    ImGui::ColorEdit4(("World Deathbox"), (float*)&Settings->Colors.Items.Deathbox, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

                    ImGui::ColorEdit4(("World Vehicle"), (float*)&Settings->Colors.Items.Vehicle, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

                    ImGui::ColorEdit4(("World Weapon"), (float*)&Settings->Colors.Items.Weapon, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
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
                    elements::HotKey(("Aimbot Hold Primary"), &Settings->Keys.Aimbot.HoldPrimary, { 142, 29 });
                    elements::HotKey(("Aimbot Hold Secondary"), &Settings->Keys.Aimbot.HoldSecondary, { 142, 29 });
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
                        std::string cfstring = cfgname;
                        std::string name = g_pConfig->pathname + "\\";
                        std::string cfgpath = name + cfstring;
                        std::ofstream outfile(cfgpath);
                        outfile << " " << std::endl;
                        outfile.close();
                        g_pConfig->ParseConfigs();
                        std::string cfstring2 = cfstring;
                        g_pConfig->InitPath((char*)cfstring2.c_str());
                        g_pConfig->Save();
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