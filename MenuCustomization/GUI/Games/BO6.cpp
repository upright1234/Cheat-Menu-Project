#include "../GUI.h"
#include  <fstream>

#define  IMGUI_DEFINE_MATH_OPERATORS
#include "../../Imgui/imgui_internal.h"
#include "../main.h"
#include "../etc_element.h"

#if IsBO6
float animation_ = 0.0f;

#define ALPHA (ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar)
#define NO_ALPHA (ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar)

char cfgname[30] = "NewSetting";
int selectedItem = 0;
bool firstTime = true;
void RenderMyMenu(bool* ShowMenu, SETTINGS* Settings, CConfig* g_pConfig) {


    if (!ShowMenu)
        return;
    
    const char* ColorTypes[] = {(("Static")), (("By Team")), (("By Visible"))};
    const char* hitboxes[] = { ("Head"), ("Chest"), ("Body"), ("Random") };
    const char* boxes[] = { "Square", "Corner" };
    const char* LinesTypes[] = { ("Top"), ("Middle"), ("Bottom") };
    const char* HealthTypes[] = { ("Horizontal"), ("Vertical") };
    const char* AimTargetTypes[] = { ("Circle"), ("Triangle") };
    const char* rarityOptions[] = { "Grey", "Green", "Blue", "Purple", "Gold" };

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
        Settings->Visuals.Players.KDA.FontStyle = selectedFontForAll;
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
                        ImGui::Text("Switch To Head");  ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(0, (ImGui::GetStyle().ItemSpacing.y * 4.3) - 6)); ImGui::Keybind("Switch To Head", &Settings->Aimbot.SwitchToHeadKey);
                        ImGui::Text("Hold Key");  ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(0, (ImGui::GetStyle().ItemSpacing.y * 4.3) - 6)); ImGui::Keybind("HoldPrimary", &Settings->Aimbot.HoldPrimaryKey);
                        ImGui::Text("Alternative Hold Key");  ImGui::SetCursorPos(ImGui::GetCursorPos() - ImVec2(0, (ImGui::GetStyle().ItemSpacing.y * 4.3) - 6)); ImGui::Keybind("Alternative Hold", &Settings->Aimbot.HoldSecondaryKey);
                    }ImGui::EndGroup();

                }e_elements::end_child();
                ImGui::SetCursorPos(ImVec2(540, 30));
                e_elements::begin_child("Settings", ImVec2(335, 500));
                {
                    ImGui::SetCursorPos(ImVec2(10, 35));

                    ImGui::BeginGroup();
                    {

                        ImGui::Checkbox("Custom FOV", &Settings->Aimbot.CustomFov);
                        if (Settings->Aimbot.CustomFov) {
                            ImGui::SliderFloat("Min. FOV", &Settings->Aimbot.MinFOV, 1, Settings->Aimbot.MaxFOV);
                            ImGui::SliderFloat("Max. FOV", &Settings->Aimbot.MaxFOV, Settings->Aimbot.MinFOV, 30.f);
                            ImGui::SliderFloat("Time Coefficient", &Settings->Aimbot.TimeCoefficient, 0.1f, 3.f);
                            ImGui::SliderFloat("Distance Coefficient", &Settings->Aimbot.DistanceCoefficient, 0.1f, 3.f);
                        }
                        else {
                            ImGui::SliderFloat("FOV", &Settings->Aimbot.MinFOV, 1, 180.f);
                        }
                        ImGui::SliderFloat("Smooth", &Settings->Aimbot.Smooth, 1, 30.f);
                        ImGui::Combo("HitBox", &Settings->Aimbot.HitBox, hitboxes, IM_ARRAYSIZE(hitboxes), IM_ARRAYSIZE(hitboxes));
                        ImGui::SliderInt("Max. Distance", &Settings->Aimbot.MaxDistance, 1, 1000);


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

                        ImGui::BinderBox("0", "Enable", &Settings->Aimbot.Enable, &Settings->Aimbot.ToggleKey);
                        ImGui::Checkbox("Save Target", &Settings->Aimbot.SaveTarget);
                        ImGui::Checkbox("Visibility Check", &Settings->Aimbot.VisibilityCheck);
                        ImGui::Checkbox("Predict", &Settings->Aimbot.Predict);
                        ImGui::Checkbox("Recoil Control", &Settings->Aimbot.RecoilControl);
                        ImGui::Checkbox("AimLock (Risky)", &Settings->Aimbot.AimLock);
                        ImGui::Checkbox("Ignore Knocked", &Settings->Aimbot.IgnoreKnocked);
                        ImGui::SliderFloat("Target Switch Delay", &Settings->Aimbot.TargetSwitchDelay, 0, 5, "%.1f");
                    
                    }ImGui::EndGroup();

                }e_elements::end_child();
                ImGui::SetCursorPos(ImVec2(540, 30));
                e_elements::begin_child("Draw", ImVec2(335, 500));
                {
                    ImGui::SetCursorPos(ImVec2(10, 35));

                    ImGui::BeginGroup();
                    {
                        ImGui::CollorBox("##color", "Draw Fov", &Settings->Aimbot.DrawFov, Settings->Aimbot.FovColor, ALPHA);
                        ImGui::CollorBox("##color2", "Draw CrossHair", &Settings->Aimbot.DrawCrossHair, Settings->Aimbot.CrossHairColor, ALPHA);
                        ImGui::CollorBox("##color3", "Draw Target", &Settings->Aimbot.DrawTarget, Settings->Aimbot.TargetCollor, ALPHA);
                        ImGui::Combo("Target Type", &Settings->Aimbot.TargetType, AimTargetTypes, IM_ARRAYSIZE(AimTargetTypes), IM_ARRAYSIZE(AimTargetTypes));
                        ImGui::SliderFloat("Fov Thickness", &Settings->Aimbot.FovThickness, 1, 5);
                        ImGui::SliderFloat("CrossHair Thickness", &Settings->Aimbot.CrossHairThickness, 1, 5);
                        ImGui::SliderFloat("Target Thickness", &Settings->Aimbot.TargetThickness, 1, 5);
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