#include "../GUI.h"
#include  <fstream>

#define  IMGUI_DEFINE_MATH_OPERATORS
#include "../../Imgui/imgui_internal.h"
#include "../main.h"
#include "../etc_element.h"

#if IsPUBG
float animation_ = 0.0f;

#define ALPHA (ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar)
#define NO_ALPHA (ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar)

char cfgname[30] = "NewSetting";
int selectedItem = 0;
bool firstTime = true;
void RenderMyMenu(bool* ShowMenu, SETTINGS* Settings, CConfig* g_pConfig) {

    //example force disable option
    Settings->Aim.SmoothInterpolation = false;

    if (!ShowMenu)
        return;
    
    const char* ColorTypes[] = {(("Static")), (("By Team")), (("By Visible"))};
    const char* hitboxes[] = { ("Head"), ("Chest"), ("Body"), ("Random") };
    const char* boxes[] = { "Square", "Corner" };
    const char* LinesTypes[] = { ("Top"), ("Middle"), ("Bottom") };
    const char* HealthTypes[] = { ("Horizontal"), ("Vertical") };
    const char* AimTargetTypes[] = { ("Circle"), ("Triangle") };
    const char* FontTypes[] = { ("Fortnite"), ("Verdana") };
    const char* ItemShowTypes[] = { ("By Type"), ("By Name") };
    const char* ItemAttachShowTypes[] = { ("By Holding Weapon"), ("By Type"), ("By Weapon Name"), ("By Name") };
    const char* WeaponTypes[] = { "Disabled",("AR")
                                , ("DMR")
                                    , ("LMG")
                                , ("Melee")
                                , ("Pistol")
                                , ("Shotgun")
                                , ("SMG")
                                    , ("Sniper")
                                , ("Special")
                                , ("Throw") };
    std::vector<std::string> weaponTypes = { ("AR")
                                , ("DMR")
                                    , ("LMG")
                                , ("Melee")
                                , ("Pistol")
                                , ("Shotgun")
                                , ("SMG")
                                    , ("Sniper")
                                , ("Special")
                                , ("Throw") };

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

    CustomStyleColor();
    if (firstTime) {
        ImGui::SetNextWindowPos({ (float)300, (float)300 });
        firstTime = false;
    }
    ImGui::SetNextWindowSize(ImVec2(890, 620));
    ImGui::Begin("Fortnite", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);
    {
        const auto& p = ImGui::GetWindowPos();

        if (users)
            ImGui::GetForegroundDrawList()->AddImageRounded(users, ImVec2(p.x + 10, p.y + 50), ImVec2(p.x + 170, p.y + 130), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255) /*color*/, 0 /*rounding*/);

        ImGui::SetCursorPos(ImVec2(10, 160));
        ImGui::BeginGroup();
        {

            if (ImGui::Tabs(0 == tabs, "F", "Aimbot", ImVec2(90, 40))) {
                sub_tabs = 0;
                tabs = 0;
            }

            if (ImGui::Tabs(1 == tabs, "A", "Visuals", ImVec2(90, 40))) {
                sub_tabs = 0;
                tabs = 1;
            }

            if (ImGui::Tabs(2 == tabs, "N", "World", ImVec2(90, 40)))
            {
                sub_tabs = 0;
                tabs = 2;
            }

            if (ImGui::Tabs(3 == tabs, "D", "Config", ImVec2(90, 40))) {
                sub_tabs = 0;
                tabs = 3;
            }


        }
        ImGui::EndGroup();
        if (0 == tabs) {
            ImGui::SetCursorPos(ImVec2(200, 15));
            ImGui::BeginGroup();
            {

                if (ImGui::SubTab(0 == sub_tabs, "Aimbot")) {
                    sub_tabs = 0;
                }

                if (ImGui::SubTab(1 == sub_tabs, "Aimbot 2")) {
                    sub_tabs = 1;
                }

            }ImGui::EndGroup();
            if (1 == sub_tabs) {
                ImGui::SetCursorPos(ImVec2(200, 30));
                e_elements::begin_child("Misc", ImVec2(335, 500));
                {
                    ImGui::SetCursorPos(ImVec2(10, 35));

                    ImGui::BeginGroup();
                    {
                        ImGui::Checkbox("Configuration By Weapon", &Settings->Aim.ByWeapon);
                        if (Settings->Aim.ByWeapon) {
                            ImGui::Combo("Weapon", &Settings->selectedWeapon, WeaponTypes, IM_ARRAYSIZE(WeaponTypes), IM_ARRAYSIZE(WeaponTypes));
                        }
                        ImGui::Text("Switch To Head");  ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(0, (ImGui::GetStyle().ItemSpacing.y * 4.3) - 6)); ImGui::Keybind("Switch To Head", &Settings->Aim.SwitchToHeadKey);
                        ImGui::Text("Hold Key");  ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(0, (ImGui::GetStyle().ItemSpacing.y * 4.3) - 6)); ImGui::Keybind("HoldPrimary", &Settings->Aim.HoldPrimaryKey);
                        ImGui::Text("Alternative Hold Key");  ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(0, (ImGui::GetStyle().ItemSpacing.y * 4.3) - 6)); ImGui::Keybind("Alternative Hold", &Settings->Aim.HoldSecondaryKey);
                    }ImGui::EndGroup();

                }e_elements::end_child();
                ImGui::SetCursorPos(ImVec2(540, 30));
                e_elements::begin_child("Settings", ImVec2(335, 500));
                {
                    ImGui::SetCursorPos(ImVec2(10, 35));

                    ImGui::BeginGroup();
                    {
                        if (Settings->Aim.ByWeapon) {
                            ImGui::Checkbox("Custom FOV", &Settings->aimbot[Settings->selectedWeapon].CustomFov);
                            if (Settings->aimbot[Settings->selectedWeapon].CustomFov) {
                                ImGui::SliderFloat("Min. FOV", &Settings->aimbot[Settings->selectedWeapon].MinFOV, 1, Settings->aimbot[Settings->selectedWeapon].MaxFOV);
                                ImGui::SliderFloat("Max. FOV", &Settings->aimbot[Settings->selectedWeapon].MaxFOV, Settings->aimbot[Settings->selectedWeapon].MinFOV, 180.f);
                                ImGui::SliderFloat("Time Coefficient", &Settings->aimbot[Settings->selectedWeapon].TimeCoefficient, 0.1f, 3.f);
                                ImGui::SliderFloat("Distance Coefficient", &Settings->aimbot[Settings->selectedWeapon].DistanceCoefficient, 0.1f, 3.f);
                            }
                            else {
                                ImGui::SliderFloat("FOV", &Settings->aimbot[Settings->selectedWeapon].MinFOV, 1, 180.f);
                            }
                            ImGui::SliderFloat("Smooth", &Settings->aimbot[Settings->selectedWeapon].Smooth, 1, 10.f);
                            ImGui::Combo("HitBox", &Settings->aimbot[Settings->selectedWeapon].HitBox, hitboxes, IM_ARRAYSIZE(hitboxes), IM_ARRAYSIZE(hitboxes));
                            ImGui::Checkbox("Humanized Smooth", &Settings->aimbot[Settings->selectedWeapon].HumanizedSmooth);
                            if (Settings->aimbot[Settings->selectedWeapon].HumanizedSmooth) {
                                ImGui::SliderFloat("Humanized Smooth Percent", &Settings->aimbot[Settings->selectedWeapon].HumanizedSmoothPercent, 1, 10.f);
                            }
                            ImGui::SliderInt("Max. Distance", &Settings->aimbot[Settings->selectedWeapon].MaxDistance, 1, 1000);
                        }
                        else {

                            ImGui::Checkbox("Custom FOV", &Settings->Aim.CustomFov);
                            if (Settings->Aim.CustomFov) {
                                ImGui::SliderFloat("Min. FOV", &Settings->Aim.MinFOV, 1, Settings->Aim.MaxFOV);
                                ImGui::SliderFloat("Max. FOV", &Settings->Aim.MaxFOV, Settings->Aim.MinFOV, 30.f);
                                ImGui::SliderFloat("Time Coefficient", &Settings->aimbot[Settings->selectedWeapon].TimeCoefficient, 0.1f, 3.f);
                                ImGui::SliderFloat("Distance Coefficient", &Settings->aimbot[Settings->selectedWeapon].DistanceCoefficient, 0.1f, 3.f);
                            }
                            else {
                                ImGui::SliderFloat("FOV", &Settings->Aim.MinFOV, 1, 180.f);
                            }
                            ImGui::SliderFloat("Smooth", &Settings->Aim.Smooth, 1, 30.f);
                            ImGui::Combo("HitBox", &Settings->Aim.HitBox, hitboxes, IM_ARRAYSIZE(hitboxes), IM_ARRAYSIZE(hitboxes));
                            ImGui::Checkbox("Humanized Smooth", &Settings->Aim.HumanizedSmooth);
                            if (Settings->Aim.HumanizedSmooth) {
                                ImGui::SliderFloat("Humanized Smooth Percent", &Settings->Aim.HumanizedSmoothPercent, 1, 10.f);
                            }
                            ImGui::SliderInt("Max. Distance", &Settings->Aim.MaxDistance, 1, 1000);
                        }

                    }ImGui::EndGroup();

                }e_elements::end_child();
            }
            else if (0 == sub_tabs) {
                ImGui::SetCursorPos(ImVec2(200, 30));
                e_elements::begin_child("General", ImVec2(335, 500));
                {
                    ImGui::SetCursorPos(ImVec2(10, 35));

                    ImGui::BeginGroup();
                    {
                        ImGui::Checkbox("Configuration By Weapon", &Settings->Aim.ByWeapon);
                        ImGui::SliderInt("Game FPS", &Settings->Configs.GameFPS, 30, 1000);
                        if (Settings->Aim.ByWeapon) {
                            ImGui::BinderBox("0", "Enable", &Settings->aimbot[Settings->selectedWeapon].Enable, &Settings->Aim.ToggleKey);
                            ImGui::Checkbox("Save Target", &Settings->aimbot[Settings->selectedWeapon].SaveTarget);
                            ImGui::Checkbox("Visibility Check", &Settings->aimbot[Settings->selectedWeapon].VisibilityCheck);
                            ImGui::Checkbox("Predict", &Settings->aimbot[Settings->selectedWeapon].Predict);
                            ImGui::Checkbox("Recoil Control", &Settings->aimbot[Settings->selectedWeapon].RecoilControl);
                            ImGui::Checkbox("AimLock (Risky)", &Settings->aimbot[Settings->selectedWeapon].AimLock);
                            ImGui::Checkbox("Ignore Knocked", &Settings->aimbot[Settings->selectedWeapon].IgnoreKnocked);
                            ImGui::SliderFloat("Target Switch Delay", &Settings->aimbot[Settings->selectedWeapon].TargetSwitchDelay, 0, 5, "%.1f");
                        }
                        else {
                            ImGui::BinderBox("0", "Enable", &Settings->Aim.Enable, &Settings->Aim.ToggleKey);
                            ImGui::Checkbox("Save Target", &Settings->Aim.SaveTarget);
                            ImGui::Checkbox("Visibility Check", &Settings->Aim.VisibilityCheck);
                            ImGui::Checkbox("Predict", &Settings->Aim.Predict);
                            ImGui::Checkbox("Recoil Control", &Settings->Aim.RecoilControl);
                            ImGui::Checkbox("AimLock (Risky)", &Settings->Aim.AimLock);
                            ImGui::Checkbox("Ignore Knocked", &Settings->Aim.IgnoreKnocked);
                            ImGui::SliderFloat("Target Switch Delay", &Settings->Aim.TargetSwitchDelay, 0, 5, "%.1f");
                        }
                    }ImGui::EndGroup();

                }e_elements::end_child();
                ImGui::SetCursorPos(ImVec2(540, 30));
                e_elements::begin_child("Draw", ImVec2(335, 500));
                {
                    ImGui::SetCursorPos(ImVec2(10, 35));

                    ImGui::BeginGroup();
                    {
                        ImGui::CollorBox("##color", "Draw Fov", &Settings->Aim.DrawFov, Settings->Aim.FovColor, ALPHA);
                        ImGui::CollorBox("##color2", "Draw CrossHair", &Settings->Aim.DrawCrossHair, Settings->Aim.CrossHairColor, ALPHA);
                        ImGui::CollorBox("##color3", "Draw Target", &Settings->Aim.DrawTarget, Settings->Aim.TargetCollor, ALPHA);
                        ImGui::Combo("Target Type", &Settings->Aim.TargetType, AimTargetTypes, IM_ARRAYSIZE(AimTargetTypes), IM_ARRAYSIZE(AimTargetTypes));
                        ImGui::SliderFloat("Fov Thickness", &Settings->Aim.FovThickness, 1, 5);
                        ImGui::SliderFloat("CrossHair Thickness", &Settings->Aim.CrossHairThickness, 1, 5);
                        ImGui::SliderFloat("Target Thickness", &Settings->Aim.TargetThickness, 1, 5);
                    }ImGui::EndGroup();

                }e_elements::end_child();
            }
        }
        else if (1 == tabs) {
            ImGui::SetCursorPos(ImVec2(200, 15));
            ImGui::BeginGroup();
            {

                if (ImGui::SubTab(0 == sub_tabs, "Players")) {
                    sub_tabs = 0;
                }
                if (ImGui::SubTab(2 == sub_tabs, "Players 2")) {
                    sub_tabs = 2;
                }
                if (ImGui::SubTab(1 == sub_tabs, "Out Screen")) {
                    sub_tabs = 1;
                }
            }ImGui::EndGroup();

            if (1 == sub_tabs) {
                ImGui::SetCursorPos(ImVec2(200, 30));
                e_elements::begin_child("Radar", ImVec2(335, 500));
                {
                    ImGui::SetCursorPos(ImVec2(10, 35));

                    ImGui::BeginGroup();
                    {
                        ImGui::BinderBox("0", "Enable", &Settings->Visuals.Radar.Enable, &Settings->Visuals.Radar.ToggleKey);
                        if (Settings->Visuals.Radar.Enable) {
                            ImGui::Combo("Color Type", &Settings->Visuals.Radar.ColorType, "Static\0By Team\0By Visibility\0\0", 3);
                            if (Settings->Visuals.Radar.ColorType == 2) {
                                ImGui::Text("Visible Color");
                                ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(-294, (ImGui::GetStyle().ItemSpacing.y * 3.5) - 6));

                                ImGui::ColorEdit4("Visible Color", Settings->Visuals.Radar.VisibleColor, ALPHA);
                                ImGui::Text("Non Visible Color");
                                ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(-294, (ImGui::GetStyle().ItemSpacing.y * 3.5) - 6));

                                ImGui::ColorEdit4("Non Visible Color", Settings->Visuals.Radar.NonVisibleColor, ALPHA);
                            }
                            else if (Settings->Visuals.Radar.ColorType == 1) {
                                ImGui::SliderInt("Color Alpha", &Settings->Visuals.Radar.TeamColorAlpha, 0, 255);
                            }
                            else {
                                ImGui::Text("Static Color");
                                ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(-294, (ImGui::GetStyle().ItemSpacing.y * 3.5) - 6));

                                ImGui::ColorEdit4("Static Color", Settings->Visuals.Radar.StaticColor, ALPHA);
                            }
                            ImGui::SliderInt("Horizontal Pos", &Settings->Visuals.Radar.PositionX, 1, 3000);
                            ImGui::SliderInt("Vertical Pos", &Settings->Visuals.Radar.PositionY, 1, 3000);
                            ImGui::SliderInt("Zoom", &Settings->Visuals.Radar.Zoom, 1, 100);
                            ImGui::SliderInt("Size", &Settings->Visuals.Radar.Size, 50, 500);
                            ImGui::SliderInt("Max. Distance", &Settings->Visuals.Radar.MaxDistance, 0, 1000);
                        }
                    }ImGui::EndGroup();

                }e_elements::end_child();
                ImGui::SetCursorPos(ImVec2(540, 30));
                e_elements::begin_child("Out Screen", ImVec2(335, 500));
                {
                    ImGui::SetCursorPos(ImVec2(10, 35));

                    ImGui::BeginGroup();
                    {
                        ImGui::Checkbox("Enable OutScreen", &Settings->Visuals.OutScreen.Enable);
                        if (Settings->Visuals.OutScreen.Enable) {
                            const char* OutScreen_items[] = { ("Triangle"), ("Dot") };
                            ImGui::Combo("Type", &Settings->Visuals.OutScreen.Type, OutScreen_items, IM_ARRAYSIZE(OutScreen_items), IM_ARRAYSIZE(OutScreen_items));
                            ImGui::Combo("Color Type", &Settings->Visuals.OutScreen.ColorType, "Static\0By Team\0By Visibility\0\0", 3);
                            if (Settings->Visuals.OutScreen.ColorType == 2) {
                                ImGui::Text("Visible Color");
                                ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(-294, (ImGui::GetStyle().ItemSpacing.y * 3.5) - 6));

                                ImGui::ColorEdit4("Visible Color##OutVis", Settings->Visuals.OutScreen.VisibleColor, ALPHA);
                                ImGui::Text("Non Visible Color");
                                ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(-294, (ImGui::GetStyle().ItemSpacing.y * 3.5) - 6));

                                ImGui::ColorEdit4("Non Visible Color##OutNonVis", Settings->Visuals.OutScreen.NonVisibleColor, ALPHA);
                            }
                            else if (Settings->Visuals.OutScreen.ColorType == 1) {
                                ImGui::SliderInt("Team Color Alpha##OutTeam", &Settings->Visuals.OutScreen.TeamColorAlpha, 0, 255);
                            }
                            else {
                                ImGui::Text("Static Color");
                                ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(-294, (ImGui::GetStyle().ItemSpacing.y * 3.5) - 6));

                                ImGui::ColorEdit4("Static Color##OutStatic", Settings->Visuals.OutScreen.StaticColor, ALPHA);
                            }
                            ImGui::Keybind("Toggle Key", &Settings->Visuals.OutScreen.ToggleKey);
                            ImGui::SliderInt("FOV", &Settings->Visuals.OutScreen.Fov, 0, 180);
                            ImGui::SliderFloat("Thickness", &Settings->Visuals.OutScreen.Thickness, 1, 5);
                            ImGui::SliderInt("Max. Distance", &Settings->Visuals.OutScreen.MaxDistance, 0, 1000);
                        }
                    }ImGui::EndGroup();

                }e_elements::end_child();
            }
            else if (2 == sub_tabs) {
                ImGui::SetCursorPos(ImVec2(200, 10));

                e_elements::begin_child("Features", ImVec2(335, 590));
                {
                    ImGui::SetCursorPos(ImVec2(10, 25));

                    ImGui::BeginGroup();
                    {
                        ImGui::Checkbox("Head Circle", &Settings->Visuals.Players.HeadCircle.Enable);
                        if (Settings->Visuals.Players.HeadCircle.Enable) {
                            ImGui::SliderFloat("Size##HeadCircle", &Settings->Visuals.Players.HeadCircle.Size, 1.0f, 10.0f);
                            ImGui::Combo("Color Type##HeadCircle", &Settings->Visuals.Players.HeadCircle.ColorType, "Static\0By Team\0By Visibility\0\0", 3);
                            if (Settings->Visuals.Players.HeadCircle.ColorType == 2) {
                                ImGui::Text("Visible Color");
                                ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(-294, (ImGui::GetStyle().ItemSpacing.y * 3.5) - 6));

                                ImGui::ColorEdit4("Visible Color##HeadVis", Settings->Visuals.Players.HeadCircle.VisibleColor, ALPHA);
                                ImGui::Text("Non Visible Color");
                                ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(-294, (ImGui::GetStyle().ItemSpacing.y * 3.5) - 6));

                                ImGui::ColorEdit4("Non Visible Color##HeadNonVis", Settings->Visuals.Players.HeadCircle.NonVisibleColor, ALPHA);
                            }
                            else if (Settings->Visuals.Players.HeadCircle.ColorType == 1) {
                                ImGui::SliderInt("Team Color Alpha##HeadTeam", &Settings->Visuals.Players.HeadCircle.TeamColorAlpha, 0, 255);
                            }
                        }
                        ImGui::Separator();
                        ImGui::CollorBox("##color1", "NickName", &Settings->Visuals.Players.NickName.Enable, Settings->Visuals.Players.NickName.StaticColor, ALPHA);
                        if (Settings->Visuals.Players.NickName.Enable) {
                            ImGui::Combo("Position##NickName", &Settings->Visuals.Players.NickName.position, LinesTypes, IM_ARRAYSIZE(LinesTypes), IM_ARRAYSIZE(LinesTypes));
                            ImGui::Combo("Color Type##NickName", &Settings->Visuals.Players.NickName.ColorType, "Static\0By Team\0By Visibility\0\0", 3);
                            if (Settings->Visuals.Players.NickName.ColorType == 2) {
                                ImGui::Text("Visible Color");
                                ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(-294, (ImGui::GetStyle().ItemSpacing.y * 3.5) - 6));

                                ImGui::ColorEdit4("Visible Color##NameVis", Settings->Visuals.Players.NickName.VisibleColor, ALPHA);
                                ImGui::Text("Non Visible Color");
                                ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(-294, (ImGui::GetStyle().ItemSpacing.y * 3.5) - 6));

                                ImGui::ColorEdit4("Non Visible Color##NameNonVis", Settings->Visuals.Players.NickName.NonVisibleColor, ALPHA);
                            }
                            else if (Settings->Visuals.Players.NickName.ColorType == 1) {
                                ImGui::SliderInt("Team Color Alpha##NameTeam", &Settings->Visuals.Players.NickName.TeamColorAlpha, 0, 255);
                            }
                        }
                    }ImGui::EndGroup();

                }e_elements::end_child();

                ImGui::SetCursorPos(ImVec2(540, 10));
                e_elements::begin_child("Features", "Features##2", ImVec2(335, 590));
                {
                    ImGui::SetCursorPos(ImVec2(10, 15));

                    ImGui::BeginGroup();
                    {

                        ImGui::CollorBox("##color2", "Distance", &Settings->Visuals.Players.Distance.Enable, Settings->Visuals.Players.Distance.StaticColor, ALPHA);
                        if (Settings->Visuals.Players.Distance.Enable) {
                            ImGui::Checkbox("Merge With NickName", &Settings->Visuals.Players.Distance.MergeNickName);
                            ImGui::Combo("Position##Distance", &Settings->Visuals.Players.Distance.position, LinesTypes, IM_ARRAYSIZE(LinesTypes), IM_ARRAYSIZE(LinesTypes));
                            ImGui::Combo("Color Type##Distance", &Settings->Visuals.Players.Distance.ColorType, "Static\0By Team\0By Visibility\0\0", 3);
                            if (Settings->Visuals.Players.Distance.ColorType == 2) {
                                ImGui::Text("Visible Color");
                                ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(-294, (ImGui::GetStyle().ItemSpacing.y * 3.5) - 6));

                                ImGui::ColorEdit4("Visible Color##DistVis", Settings->Visuals.Players.Distance.VisibleColor, ALPHA);
                                ImGui::Text("Non Visible Color");
                                ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(-294, (ImGui::GetStyle().ItemSpacing.y * 3.5) - 6));

                                ImGui::ColorEdit4("Non Visible Color##DistNonVis", Settings->Visuals.Players.Distance.NonVisibleColor, ALPHA);
                            }
                            else if (Settings->Visuals.Players.Distance.ColorType == 1) {
                                ImGui::SliderInt("Team Color Alpha##DistTeam", &Settings->Visuals.Players.Distance.TeamColorAlpha, 0, 255);
                            }
                        }
                        ImGui::Separator();
                        ImGui::CollorBox("##color3", "Weapon", &Settings->Visuals.Players.Weapon.Enable, Settings->Visuals.Players.Weapon.StaticColor, ALPHA);
                        if (Settings->Visuals.Players.Weapon.Enable) {
                            ImGui::Combo("Position##Weapon", &Settings->Visuals.Players.Weapon.position, LinesTypes, IM_ARRAYSIZE(LinesTypes), IM_ARRAYSIZE(LinesTypes));
                            ImGui::Combo("Color Type##Weapon", &Settings->Visuals.Players.Weapon.ColorType, "Static\0By Team\0By Visibility\0\0", 3);
                            if (Settings->Visuals.Players.Weapon.ColorType == 2) {
                                ImGui::Text("Visible Color");
                                ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(-294, (ImGui::GetStyle().ItemSpacing.y * 3.5) - 6));

                                ImGui::ColorEdit4("Visible Color##WeapVis", Settings->Visuals.Players.Weapon.VisibleColor, ALPHA);
                                ImGui::Text("Non Visible Color");
                                ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(-294, (ImGui::GetStyle().ItemSpacing.y * 3.5) - 6));

                                ImGui::ColorEdit4("Non Visible Color##WeapNonVis", Settings->Visuals.Players.Weapon.NonVisibleColor, ALPHA);
                            }
                            else if (Settings->Visuals.Players.Weapon.ColorType == 1) {
                                ImGui::SliderInt("Team Color Alpha##WeapTeam", &Settings->Visuals.Players.Weapon.TeamColorAlpha, 0, 255);
                            }
                        }

                    }ImGui::EndGroup();

                }e_elements::end_child();
            }
            else if (0 == sub_tabs) {
                ImGui::SetCursorPos(ImVec2(200, 10));
                e_elements::begin_child("General", ImVec2(335, 240));
                {
                    ImGui::SetCursorPos(ImVec2(10, 35));

                    ImGui::BeginGroup();
                    {
                        ImGui::BinderBox("0", "Enable", &Settings->Visuals.Players.Enable, &Settings->Visuals.Players.ToggleKey);
                        ImGui::SliderInt("Max. Distance##ESP", &Settings->Visuals.Players.MaxDistance, 0, 1000);
                        ImGui::SliderFloat("Min. Font Size", &Settings->Visuals.Players.MinFontSize, 10.0f, Settings->Visuals.Players.MaxFontSize, "%.1f");
                        ImGui::SliderFloat("Max. Font Size", &Settings->Visuals.Players.MaxFontSize, Settings->Visuals.Players.MinFontSize, 30.0f, "%.1f");

                    }ImGui::EndGroup();

                }e_elements::end_child();


                ImGui::SetCursorPos(ImVec2(200, 200));

                e_elements::begin_child("Features", ImVec2(335, 370));
                {
                    ImGui::SetCursorPos(ImVec2(10, 35));

                    ImGui::BeginGroup();
                    {
                        ImGui::Checkbox("Box", &Settings->Visuals.Players.Box.Enable);
                        if (Settings->Visuals.Players.Box.Enable) {
                            ImGui::Combo("Type##Box", &Settings->Visuals.Players.Box.Type, boxes, IM_ARRAYSIZE(boxes), IM_ARRAYSIZE(boxes));
                            ImGui::SliderFloat("Thickness##Box", &Settings->Visuals.Players.Box.Thickness, 1.0f, 5.0f, "%.1f");
                            ImGui::Checkbox("Filled##Box", &Settings->Visuals.Players.Box.Filled);
                            if (Settings->Visuals.Players.Box.Filled) {
                                ImGui::Text("Fill Color");
                                ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(-294, (ImGui::GetStyle().ItemSpacing.y * 3.5) - 6));

                                ImGui::ColorEdit4("Fill Color##Box", Settings->Visuals.Players.Box.FillColor, ALPHA);
                            }
                            ImGui::Combo("Color Type##Box", &Settings->Visuals.Players.Box.ColorType, "Static\0By Team\0By Visibility\0\0", 3);
                            if (Settings->Visuals.Players.Box.ColorType == 2) {
                                ImGui::Text("Visible Color");
                                ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(-294, (ImGui::GetStyle().ItemSpacing.y * 3.5) - 6));

                                ImGui::ColorEdit4("Visible Color##BoxVis", Settings->Visuals.Players.Box.VisibleColor, ALPHA);
                                ImGui::Text("Non Visible Color");
                                ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(-294, (ImGui::GetStyle().ItemSpacing.y * 3.5) - 6));

                                ImGui::ColorEdit4("Non Visible Color##BoxNonVis", Settings->Visuals.Players.Box.NonVisibleColor, ALPHA);
                            }
                            else if (Settings->Visuals.Players.Box.ColorType == 1) {
                                ImGui::SliderInt("Color Alpha##BoxTeam", &Settings->Visuals.Players.Box.TeamColorAlpha, 0, 255);
                            }
                        }
                    }ImGui::EndGroup();

                }e_elements::end_child();

                ImGui::SetCursorPos(ImVec2(540, 10));
                e_elements::begin_child("Features", "Features##2", ImVec2(335, 590));
                {
                    ImGui::SetCursorPos(ImVec2(10, 35));

                    ImGui::BeginGroup();
                    {
                        ImGui::Checkbox("Health", &Settings->Visuals.Players.Health.Enable);
                        if (Settings->Visuals.Players.Health.Enable)
                        {
                            ImGui::Combo("Position##Health", &Settings->Visuals.Players.Health.position, HealthTypes, IM_ARRAYSIZE(HealthTypes), 2);
                            ImGui::SliderFloat("Thickness##Health", &Settings->Visuals.Players.Health.Thickness, 1.0f, 5.0f, "%.1f");
                            ImGui::SliderInt("Color Alpha##Health", &Settings->Visuals.Players.Health.ColorAlpha, 0, 255);
                        }
                        ImGui::Separator();
                        ImGui::Checkbox("Lines", &Settings->Visuals.Players.Lines.Enable);
                        if (Settings->Visuals.Players.Lines.Enable) {
                            ImGui::Combo("Position##Lines", &Settings->Visuals.Players.Lines.Position, LinesTypes, IM_ARRAYSIZE(LinesTypes), IM_ARRAYSIZE(LinesTypes));
                            ImGui::Combo("Color Type##Lines", &Settings->Visuals.Players.Lines.ColorType, "Static\0By Team\0By Visibility\0\0", 3);
                            if (Settings->Visuals.Players.Lines.ColorType == 2) {
                                ImGui::Text("Visible Color");
                                ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(-294, (ImGui::GetStyle().ItemSpacing.y * 3.5) - 6));

                                ImGui::ColorEdit4("Visible Color##LinesVis", Settings->Visuals.Players.Lines.VisibleColor, ALPHA);
                                ImGui::Text("Non Visible Color");
                                ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(-294, (ImGui::GetStyle().ItemSpacing.y * 3.5) - 6));

                                ImGui::ColorEdit4("Non Visible Color##LinesNonVis", Settings->Visuals.Players.Lines.NonVisibleColor, ALPHA);
                            }
                            else if (Settings->Visuals.Players.Lines.ColorType == 1) {
                                ImGui::SliderInt("Color Alpha##LinesTeam", &Settings->Visuals.Players.Lines.TeamColorAlpha, 0, 255);
                            }
                        }
                        ImGui::Separator();
                        ImGui::Checkbox("Skeleton", &Settings->Visuals.Players.Skeleton.Enable);
                        if (Settings->Visuals.Players.Skeleton.Enable) {
                            ImGui::SliderFloat("Thickness##Skeleton", &Settings->Visuals.Players.Skeleton.Thickness, 1.0f, 5.0f, "%.1f");
                            ImGui::Combo("Color Type##Skeleton", &Settings->Visuals.Players.Skeleton.ColorType, "Static\0By Team\0By Visibility\0\0", 3);
                            if (Settings->Visuals.Players.Skeleton.ColorType == 2) {
                                ImGui::Text("Visible Color");
                                ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(-294, (ImGui::GetStyle().ItemSpacing.y * 3.5) - 6));

                                ImGui::ColorEdit4("Visible Color##SkelVis", Settings->Visuals.Players.Skeleton.VisibleColor, ALPHA);
                                ImGui::Text("Non Visible Color");
                                ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(-294, (ImGui::GetStyle().ItemSpacing.y * 3.5) - 6));

                                ImGui::ColorEdit4("Non Visible Color##SkelNonVis", Settings->Visuals.Players.Skeleton.NonVisibleColor, ALPHA);
                            }
                            else if (Settings->Visuals.Players.Skeleton.ColorType == 1) {
                                ImGui::SliderInt("Color Alpha##SkelTeam", &Settings->Visuals.Players.Skeleton.TeamColorAlpha, 0, 255);
                            }
                        }

                    }ImGui::EndGroup();

                }e_elements::end_child();
            }
        }
        else if (2 == tabs)
        {
            ImGui::SetCursorPos(ImVec2(200, 15));
            ImGui::BeginGroup();
            {
                if (ImGui::SubTab(0 == sub_tabs, "Config"))
                {
                    sub_tabs = 0;
                }
                if (ImGui::SubTab(1 == sub_tabs, "Weapons"))
                {
                    sub_tabs = 1;
                }
                if (ImGui::SubTab(2 == sub_tabs, "World"))
                {
                    sub_tabs = 2;
                }
            }
            ImGui::EndGroup();

            if (0 == sub_tabs) {
                ImGui::SetCursorPos(ImVec2(200, 20));
                ImGuiWindow* window = ImGui::GetCurrentWindow();
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 11));
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
                e_elements::begin_child(window->GetID("World##1"), ImVec2(335, 600));
                {
                    ImGui::SetCursorPos(ImVec2(10, 35));

                    ImGui::BeginGroup();
                    {
                        ImGui::BinderBox("0", "Enable", &Settings->Visuals.Items.Enable, &Settings->Visuals.Items.ToggleKey);
                        ImGui::SliderInt("Max. Distance", &Settings->Visuals.Items.MaxDistance, 0, 1000);
                        ImGui::SliderFloat("Min. Font Size", &Settings->Visuals.Players.MinFontSize, 10.0f, Settings->Visuals.Players.MaxFontSize, "%.1f");
                        ImGui::SliderFloat("Max. Font Size", &Settings->Visuals.Players.MaxFontSize, Settings->Visuals.Players.MinFontSize, 30.0f, "%.1f");
                    }
                    ImGui::EndGroup();
                }
                e_elements::end_child();
            }
            if (1 == sub_tabs) {
                ImGui::SetCursorPos(ImVec2(200, 20));
                ImGuiWindow* window = ImGui::GetCurrentWindow();
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 11));
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
                e_elements::begin_child(window->GetID("World##1"), ImVec2(335, 600));
                {
                    ImGui::SetCursorPos(ImVec2(10, 35));

                    ImGui::BeginGroup();
                    {
                        ImGui::CollorBox("##collorWeapon", "Weapon", &Settings->Visuals.Items.Weapon.Enable, Settings->Visuals.Items.Weapon.Color, ALPHA);
                        if (Settings->Visuals.Items.Weapon.Enable) {
                            ImGui::Combo("Type##Weapon", &Settings->Visuals.Items.Weapon.ShowType, ItemShowTypes, IM_ARRAYSIZE(ItemShowTypes), IM_ARRAYSIZE(ItemShowTypes));
                            if (Settings->Visuals.Items.Weapon.ShowType == 0) {
                                ImGui::MultiCombo("Types##Weapon", Settings->Visuals.Items.Weapon.Types, weaponTypes);
                            }
                            else  if (Settings->Visuals.Items.Weapon.ShowType == 1) {

                                ImGui::MultiComboVector("Types##Weapon", Settings->Visuals.Items.Weapon.List, Settings->Visuals.Items.Weapon.Selected);
                            }
                        }


                    }
                    ImGui::EndGroup();
                }
                e_elements::end_child();
                ImGui::SetCursorPos(ImVec2(540, 20));
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 11));
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
                e_elements::begin_child(window->GetID("World##2"), ImVec2(335, 600));
                {
                    ImGui::SetCursorPos(ImVec2(10, 35));

                    ImGui::BeginGroup();
                    {
                        ImGui::CollorBox("##collorAmmo", "Ammo", &Settings->Visuals.Items.Ammo.Enable, Settings->Visuals.Items.Ammo.Color, ALPHA);
                        if (Settings->Visuals.Items.Ammo.Enable) {
                            ImGui::Combo("Type##Ammo", &Settings->Visuals.Items.Ammo.ShowType, ItemAttachShowTypes, IM_ARRAYSIZE(ItemAttachShowTypes), IM_ARRAYSIZE(ItemAttachShowTypes));
                            if (Settings->Visuals.Items.Ammo.ShowType == 1) {
                                ImGui::MultiCombo("Types##Ammo", Settings->Visuals.Items.Ammo.Types, weaponTypes);
                            }
                            else if (Settings->Visuals.Items.Ammo.ShowType == 2) {
                                ImGui::MultiComboVector("Types##Ammo", Settings->Visuals.Items.Weapon.List, Settings->Visuals.Items.Ammo.SelectedWeapon);
                            }
                            else if (Settings->Visuals.Items.Ammo.ShowType == 3) {
                                ImGui::MultiComboVector("Types##Ammo", Settings->Visuals.Items.Ammo.List, Settings->Visuals.Items.Ammo.Selected);
                            }
                        }
                        ImGui::Separator();
                        ImGui::CollorBox("##collorAttachment", "Attachment", &Settings->Visuals.Items.Attachment.Enable, Settings->Visuals.Items.Attachment.Color, ALPHA);
                        if (Settings->Visuals.Items.Attachment.Enable) {
                            ImGui::Combo("Type##Attachment", &Settings->Visuals.Items.Attachment.ShowType, ItemAttachShowTypes, IM_ARRAYSIZE(ItemAttachShowTypes), IM_ARRAYSIZE(ItemAttachShowTypes));
                            if (Settings->Visuals.Items.Attachment.ShowType == 1) {
                                ImGui::MultiCombo("Types##Attachment", Settings->Visuals.Items.Attachment.Types, weaponTypes);
                            }
                            else if (Settings->Visuals.Items.Attachment.ShowType == 2) {
                                ImGui::MultiComboVector("Types##Attachment", Settings->Visuals.Items.Weapon.List, Settings->Visuals.Items.Attachment.SelectedWeapon);
                            }
                            else if (Settings->Visuals.Items.Attachment.ShowType == 3) {
                                ImGui::MultiComboVector("Types##Attachment", Settings->Visuals.Items.Attachment.List, Settings->Visuals.Items.Attachment.Selected);
                            }
                        }
                    }
                    ImGui::EndGroup();
                }
                e_elements::end_child();
            }
            if (2 == sub_tabs) {
                ImGui::SetCursorPos(ImVec2(200, 20));
                ImGuiWindow* window = ImGui::GetCurrentWindow();
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 11));
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
                e_elements::begin_child(window->GetID("World##1"), ImVec2(335, 600));
                {
                    ImGui::SetCursorPos(ImVec2(10, 35));

                    ImGui::BeginGroup();
                    {
                        ImGui::CollorBox("##collorVehicle", "Vehicle", &Settings->Visuals.Items.Vehicle.Enable, Settings->Visuals.Items.Vehicle.Color, ALPHA);
                        if (Settings->Visuals.Items.Vehicle.Enable) {
                            ImGui::Combo("Type##Vehicle", &Settings->Visuals.Items.Vehicle.ShowType, ItemShowTypes, IM_ARRAYSIZE(ItemShowTypes), IM_ARRAYSIZE(ItemShowTypes));
                            if (Settings->Visuals.Items.Vehicle.ShowType == 0) {
                                std::vector<std::string> VehicleTypes = { std::string("Road")
                                       , std::string("Sailing") };
                                ImGui::MultiCombo("Types##Vehicle", Settings->Visuals.Items.Vehicle.Types, VehicleTypes);
                            }
                            else if (Settings->Visuals.Items.Vehicle.ShowType == 1) {
                                ImGui::MultiComboVector("Types##Vehicle", Settings->Visuals.Items.Vehicle.List, Settings->Visuals.Items.Vehicle.Selected);
                            }
                        }
                        ImGui::Separator();
                        ImGui::CollorBox("##collorPackage", "Package", &Settings->Visuals.Items.Package.Enable, Settings->Visuals.Items.Package.Color, ALPHA);
                        if (Settings->Visuals.Items.Package.Enable) {
                            ImGui::MultiComboVector("Type##Package", Settings->Visuals.Items.Package.List, Settings->Visuals.Items.Package.Selected);
                        }

                        ImGui::Separator();
                        ImGui::CollorBox("##collorOther", "Other Items", &Settings->Visuals.Items.OtherEquipable.Enable, Settings->Visuals.Items.OtherEquipable.Color, ALPHA);
                        if (Settings->Visuals.Items.OtherEquipable.Enable) {
                            ImGui::MultiComboVector("Types##Other", Settings->Visuals.Items.OtherEquipable.List, Settings->Visuals.Items.OtherEquipable.Selected);
                        }
                    }
                    ImGui::EndGroup();
                }
                e_elements::end_child();
                ImGui::SetCursorPos(ImVec2(540, 20));
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 11));
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 16));
                e_elements::begin_child(window->GetID("World##2"), ImVec2(335, 600));
                {
                    ImGui::SetCursorPos(ImVec2(10, 35));

                    ImGui::BeginGroup();
                    {
                        ImGui::CollorBox("##collorMedicine", "Medicine", &Settings->Visuals.Items.Medicine.Enable, Settings->Visuals.Items.Medicine.Color, ALPHA);
                        if (Settings->Visuals.Items.Medicine.Enable) {
                            ImGui::MultiComboVector("Types##Medicine", Settings->Visuals.Items.Medicine.List, Settings->Visuals.Items.Medicine.Selected);
                        }
                        ImGui::Separator();
                        ImGui::CollorBox("##collorEquipable", "Equipable", &Settings->Visuals.Items.Equipable.Enable, Settings->Visuals.Items.Equipable.Color, ALPHA);
                        if (Settings->Visuals.Items.Equipable.Enable) {
                            ImGui::MultiComboVector("Types##Equipable", Settings->Visuals.Items.Equipable.List, Settings->Visuals.Items.Equipable.Selected);
                        }
                        ImGui::Separator();
                        ImGui::CollorBox("##collorThrowables", "Throwables", &Settings->Visuals.Items.Throwables.Enable, Settings->Visuals.Items.Throwables.Color, ALPHA);
                        if (Settings->Visuals.Items.Throwables.Enable) {
                            ImGui::MultiComboVector("Types##Throwables", Settings->Visuals.Items.Throwables.List, Settings->Visuals.Items.Throwables.Selected);
                        }
                    }
                    ImGui::EndGroup();
                }
                e_elements::end_child();
            }
        }

        else if (3 == tabs) {
            ImGui::SetCursorPos(ImVec2(200, 15));
            ImGui::BeginGroup();
            {

                if (ImGui::SubTab(0 == sub_tabs, "Config")) {
                    sub_tabs = 0;
                }
            }ImGui::EndGroup();

            ImGui::SetCursorPos(ImVec2(200, 30));
            ImGuiWindow* window = ImGui::GetCurrentWindow();
            e_elements::begin_child("Config", ImVec2(535, 580));
            {
                ImGui::SetCursorPos(ImVec2(10, 45));

                ImGui::BeginGroup();
                {
                    ImGui::SliderInt("Render FPS", &Settings->Configs.RenderFPS, 30, 1000);
                    ImGui::Checkbox("Spectator Count", &Settings->Configs.SpectatorCount);
                    ImGui::Combo("Render Font Style", &Settings->Configs.FontStyle, FontTypes, IM_ARRAYSIZE(FontTypes), IM_ARRAYSIZE(FontTypes));

                    ImGui::Text("Menu Key");
                    ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(0, (ImGui::GetStyle().ItemSpacing.y * 5) - 6));
                    ImGui::Keybind("Menu", &Settings->Configs.MenuKey);


                    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(0, (ImGui::GetStyle().ItemSpacing.y * 2.5) - 6));

                    if (ImGui::Combo("Select Your Config", &Settings->iConfig, g_pConfig->szConfigs.c_str())) {
                        if (Settings->oldiConfig != Settings->iConfig) {
                            g_pConfig->InitPath(&g_pConfig->vecConfigs[Settings->oldiConfig][0u]);
                            g_pConfig->Save();
                            Settings->oldiConfig = Settings->iConfig;
                            g_pConfig->InitPath(&g_pConfig->vecConfigs[Settings->iConfig][0u]);
                            g_pConfig->Read();
                        }
                    }


                    ImGui::Text("New Config:");
                    ImGui::InputText("Config Name", cfgname, 30);
                    if (ImGui::Button("Add Config")) {
                        g_pConfig->CreateConfig(cfgname);
                    }

                }ImGui::EndGroup();

            }e_elements::end_child();
        }
    } ImGui::End();
}
#endif