#include "../GUI.h"
#include  <fstream>

#define  IMGUI_DEFINE_MATH_OPERATORS
#include "../../Imgui/imgui_internal.h"
#include "../main.h"
#include "../etc_element.h"

#if IsMW19
#include "../MenuKit.h"
using namespace kit;

char cfgname[30] = "NewSetting";

namespace {

const char* const kHitBoxes[]        = { "Head", "Chest", "Body", "Random" };
const char* const kPositions[]       = { "Top", "Bottom", "Left", "Right" };
const char* const kTopBottom[]       = { "Top", "Bottom" };
const char* const kBoxStyles[]       = { "Square", "Corner" };
const char* const kTargetShapes[]    = { "Circle", "Triangle" };
const char* const kOffscreenStyles[] = { "Circle", "Triangle" };
const char* const kLanguages[]       = { "English", "Portuguese" };

enum Page { P_Aimbot, P_Players, P_Radar, P_Settings, P_Configs, P_Count };

const char* const kSections[] = { "COMBAT", "VISUALS", "SYSTEM" };
const NavItem kPages[P_Count] = {
    { "Aimbot",   "Targeting, triggerbot, recoil and aim overlays", 0 },
    { "Players",  "Boxes, skeletons, labels and alerts",            1 },
    { "Radar",    "Mini radar and off-screen indicators",           1 },
    { "Settings", "Menu key, performance, game and fonts",          2 },
    { "Configs",  "Save, load and create presets",                  2 },
};

// Opens a panel for a player label (name / distance); caller adds extras and calls EndPanel()
template <typename T>
void BeginLabelPanel(const char* title, T& label, const std::vector<std::string>& fonts) {
    BeginPanel(title);
    ImGui::Checkbox("Enable", &label.Enable);
    if (label.Enable) {
        ComboRow("Position", &label.position, kPositions, IM_ARRAYSIZE(kPositions));
        ComboRow("Font", &label.FontStyle, fonts);
        ColorMode(label);
    }
}

void PageAimbot(SETTINGS* s) {
    auto& aim = s->Aimbot;
    BeginColumns();
    BeginPanel("Aimbot");
    {
        ImGui::Checkbox("Enable", &aim.Enable);
        KeyRow("Toggle Key", &aim.ToggleKey);
        KeyRow("Aim Key", &aim.HoldPrimaryKey);
        KeyRow("Secondary Key", &aim.HoldSecondaryKey);
        KeyRow("Switch To Head", &aim.SwitchToHeadKey);
        ComboRow("Hitbox", &aim.HitBox, kHitBoxes, IM_ARRAYSIZE(kHitBoxes));
        SliderF("Smoothing", &aim.Smooth, 1.f, 10.f);
        SliderI("Max Distance", &aim.MaxDistance, 1, 800, "%d m");
    }
    EndPanel();

    BeginPanel("Behavior");
    {
        ImGui::Checkbox("Visibility Check", &aim.VisibilityCheck);
        ImGui::Checkbox("Ignore Knocked", &aim.IgnoreKnocked);
        ImGui::Checkbox("Aim Lock", &aim.AimLock);
        ImGui::Checkbox("Keep Target", &aim.SaveTarget);
        if (aim.SaveTarget)
            SliderF("Target Switch Delay", &aim.TargetSwitchDelay, 0.f, 5.f, "%.1f s");
        ImGui::Checkbox("Dead Zone", &aim.DeadZone);
        if (aim.DeadZone)
            SliderF("Dead Zone Radius", &aim.DeadZoneRadius, 1.f, 20.f);
    }
    EndPanel();

    BeginPanel("Triggerbot");
    {
        ToggleKey("Enable", &aim.TriggerBot, &aim.TriggerKey);
        if (aim.TriggerBot) {
            SliderF("Delay", &aim.TriggerDelay, 0.f, 1.f, "%.2f s");
            SliderF("Trigger FOV", &aim.TriggerFOV, 1.f, 10.f);
            ToggleColor("Draw FOV", &aim.TriggerBotDrawFOV, aim.TriggerBotFOVColor);
            if (aim.TriggerBotDrawFOV)
                SliderF("Thickness", &aim.TriggerBotFOVThickness, 1.f, 5.f);
        }
    }
    EndPanel();

    NextColumn();
    BeginPanel("Field of View");
    {
        SliderF("Min FOV", &aim.MinFOV, 1.f, aim.MaxFOV, "%.0f");
        SliderF("Max FOV", &aim.MaxFOV, aim.MinFOV, 180.f, "%.0f");
        ImGui::Checkbox("Dynamic FOV", &aim.CustomFov);
        if (aim.CustomFov) {
            SliderF("Distance Coefficient", &aim.DistanceCoefficient, 0.f, 3.f, "%.2f");
            SliderF("Time Coefficient", &aim.TimeCoefficient, 0.f, 3.f, "%.2f");
        }
        SubHeader("Circle");
        ToggleColor("Draw FOV", &aim.DrawFov, aim.FovColor);
        if (aim.DrawFov)
            SliderF("Thickness", &aim.FovThickness, 1.f, 5.f);
    }
    EndPanel();

    BeginPanel("Target Indicator");
    {
        ToggleColor("Draw Target", &aim.DrawTarget, aim.TargetCollor);
        if (aim.DrawTarget) {
            ComboRow("Shape", &aim.TargetType, kTargetShapes, IM_ARRAYSIZE(kTargetShapes));
            SliderF("Thickness", &aim.TargetThickness, 1.f, 5.f);
        }
    }
    EndPanel();

    BeginPanel("Crosshair");
    {
        ToggleColor("Draw Crosshair", &aim.DrawCrossHair, aim.CrossHairColor);
        if (aim.DrawCrossHair)
            SliderF("Thickness", &aim.CrossHairThickness, 1.f, 5.f);
    }
    EndPanel();

    BeginPanel("Recoil Control");
    {
        ImGui::Checkbox("Enable", &aim.RecoilControl);
        if (aim.RecoilControl)
            SliderF("Strength", &aim.RecoilSlider, 0.1f, 1.f, "%.2f");
    }
    EndPanel();

    BeginPanel("Kill Switch");
    {
        ToggleKey("Enable", &aim.KillSwitch, &aim.KillSwitchKey);
        ImGui::Checkbox("Auto Kill Switch", &aim.AutoKillSwitch);
        if (aim.AutoKillSwitch)
            SliderF("Distance", &aim.KillSwitchDistance, 0.f, 150.f, "%.0f m");
    }
    EndPanel();
    EndColumns();
}

void PagePlayers(SETTINGS* s) {
    auto& p = s->Visuals.Players;
    BeginColumns();
    BeginPanel("Player ESP");
    {
        ImGui::Checkbox("Enable", &p.Enable);
        KeyRow("Toggle Key", &p.ToggleKey);
        ImGui::Checkbox("Team Check", &s->Configs.TeamCheck);
        SliderI("Max Distance", &p.MaxDistance, 1, 800, "%d m");
        SliderF("Min Font Size", &p.MinFontSize, 10.f, p.MaxFontSize, "%.0f px");
        SliderF("Max Font Size", &p.MaxFontSize, p.MinFontSize, 30.f, "%.0f px");
        ImGui::Checkbox("Fade With Distance", &p.DistanceTransparency);
        if (p.DistanceTransparency) {
            SliderF("Min Opacity", &p.MinTransparency, 0.1f, p.MaxTransparency, "%.2f");
            SliderF("Max Opacity", &p.MaxTransparency, p.MinTransparency, 1.f, "%.2f");
        }
    }
    EndPanel();

    BeginPanel("Box");
    {
        auto& box = p.Box;
        ImGui::Checkbox("Enable", &box.Enable);
        if (box.Enable) {
            ComboRow("Style", &box.Type, kBoxStyles, IM_ARRAYSIZE(kBoxStyles));
            SliderF("Width", &box.Width, 0.1f, 1.f, "%.2f");
            SliderF("Thickness", &box.Thickness, 1.f, 5.f);
            SliderI("Rounding", &box.rounding, 1, 20);
            ToggleColor("Filled", &box.Filled, box.FillColor);
            ColorMode(box);
        }
    }
    EndPanel();

    BeginPanel("Skeleton");
    {
        ImGui::Checkbox("Enable", &p.Skeleton.Enable);
        if (p.Skeleton.Enable) {
            SliderF("Thickness", &p.Skeleton.Thickness, 1.f, 5.f, "%.2f");
            ColorMode(p.Skeleton);
        }
    }
    EndPanel();

    BeginPanel("Head Circle");
    {
        auto& head = p.HeadCircle;
        ImGui::Checkbox("Enable", &head.Enable);
        if (head.Enable) {
            SliderF("Size", &head.Size, 1.f, 10.f);
            SliderF("Thickness", &head.Thickness, 1.f, 5.f);
            ImGui::Checkbox("Filled", &head.Filled);
            ColorMode(head);
        }
    }
    EndPanel();

    NextColumn();
    BeginLabelPanel("Name", p.NickName, s->InstalledFonts);
    EndPanel();

    BeginLabelPanel("Distance", p.Distance, s->InstalledFonts);
    if (p.Distance.Enable)
        ImGui::Checkbox("Merge With Name", &p.Distance.MergeNickName);
    EndPanel();

    BeginPanel("Health Bar");
    {
        auto& hp = p.Health;
        ImGui::Checkbox("Enable", &hp.Enable);
        if (hp.Enable) {
            ComboRow("Position", &hp.position, kPositions, IM_ARRAYSIZE(kPositions));
            SliderF("Thickness", &hp.Thickness, 1.f, 5.f);
            SliderI("Opacity", &hp.ColorAlpha, 0, 255);
            ImGui::Checkbox("Gradient", &hp.HealthGradient);
        }
    }
    EndPanel();

    BeginPanel("Snaplines");
    {
        auto& lines = p.Lines;
        ImGui::Checkbox("Enable", &lines.Enable);
        if (lines.Enable) {
            ComboRow("Origin", &lines.Position, kTopBottom, IM_ARRAYSIZE(kTopBottom));
            SliderF("Thickness", &lines.Thickness, 1.f, 5.f);
            ColorMode(lines);
        }
    }
    EndPanel();

    BeginPanel("Proximity Alert");
    {
        auto& prox = p.ProximityAlerts;
        ImGui::Checkbox("Enable", &prox.Enable);
        if (prox.Enable) {
            SliderF("Radius", &prox.ProximityRadius, 5.f, 200.f, "%.0f m");
            ImGui::Checkbox("Pulsing Border", &prox.PulsatingBorder);
            if (prox.PulsatingBorder)
                SliderF("Border Thickness", &prox.BorderThickness, 1.f, 5.f);
            ImGui::Checkbox("Warning Text", &prox.ProximityText);
            if (prox.ProximityText)
                SliderF("Text Size", &prox.TextSize, 10.f, 50.f, "%.0f px");
            ColorRow("Alert Color", prox.AlertColor);
            ColorRow("Text Color", prox.TextColor);
        }
    }
    EndPanel();
    EndColumns();
}

void PageRadar(SETTINGS* s) {
    BeginColumns();
    BeginPanel("Radar");
    {
        auto& r = s->Visuals.Radar;
        ToggleKey("Enable", &r.Enable, &r.ToggleKey);
        if (r.Enable) {
            ImGui::Checkbox("Show Bots", &r.ShowBots);
            SliderI("Size", &r.Size, 50, 500, "%d px");
            SliderI("Zoom", &r.Zoom, 1, 100);
            SliderI("Max Distance", &r.MaxDistance, 1, 800, "%d m");
            SliderI("Position X", &r.PositionX, 0, 3000);
            SliderI("Position Y", &r.PositionY, 0, 3000);
            SubHeader("Markers");
            SliderF("Min Size", &r.MinSize, 1.f, r.MaxSize);
            SliderF("Max Size", &r.MaxSize, r.MinSize, 6.f);
            SliderF("Min Opacity", &r.MinOpacity, 0.1f, r.MaxOpacity, "%.2f");
            SliderF("Max Opacity", &r.MaxOpacity, r.MinOpacity, 1.f, "%.2f");
            ColorMode(r);
        }
    }
    EndPanel();

    NextColumn();
    BeginPanel("Off-screen Indicators");
    {
        auto& o = s->Visuals.OutScreen;
        ToggleKey("Enable", &o.Enable, &o.ToggleKey);
        if (o.Enable) {
            ImGui::Checkbox("Show Bots", &o.ShowBots);
            ComboRow("Style", &o.Type, kOffscreenStyles, IM_ARRAYSIZE(kOffscreenStyles));
            SliderI("FOV", &o.Fov, 1, 180);
            SliderI("Max Distance", &o.MaxDistance, 1, 800, "%d m");
            SliderF("Thickness", &o.Thickness, 1.f, 5.f);
            SliderF("Min Size", &o.MinSize, 1.f, o.MaxSize, "%.0f px");
            SliderF("Max Size", &o.MaxSize, o.MinSize, 30.f, "%.0f px");
            SliderF("Min Opacity", &o.MinOpacity, 0.1f, o.MaxOpacity, "%.2f");
            SliderF("Max Opacity", &o.MaxOpacity, o.MinOpacity, 1.f, "%.2f");
            ColorMode(o);
        }
    }
    EndPanel();
    EndColumns();
}

void PageSettings(SETTINGS* s, bool* ShowMenu) {
    auto& c = s->Configs;
    BeginColumns();
    BeginPanel("Menu");
    {
        KeyRow("Menu Key", &c.MenuKey);
        ComboRow("Language", &c.Language, kLanguages, IM_ARRAYSIZE(kLanguages));
        if (ActionButton("##hide", "Hide Menu"))
            *ShowMenu = false;
    }
    EndPanel();

    BeginPanel("Performance");
    {
        ImGui::Checkbox("VSync", &c.VSync);
        SliderI("Render FPS", &c.RenderFPS, 30, 360);
        SliderI("Game FPS", &c.GameFPS, 30, 240);
    }
    EndPanel();

    NextColumn();
    FontsPanel(s, [s](int font) {
        s->Visuals.Players.NickName.FontStyle = font;
        s->Visuals.Players.Distance.FontStyle = font;
    });

    BeginPanel("Game");
    {
        ImGui::Checkbox("Disable Flash Effects", &c.DisableFlashEffects);
    }
    EndPanel();
    EndColumns();
}

} // namespace

void RenderMyMenu(bool* ShowMenu, SETTINGS* Settings, CConfig* g_pConfig) {
    MENU_LOG_FRAME(ShowMenu, Settings, g_pConfig);
    if (!ShowMenu || !Settings)
        return;

    const Chip chips[] = {
        { "AIM",     &Settings->Aimbot.Enable },
        { "TRIGGER", &Settings->Aimbot.TriggerBot },
        { "ESP",     &Settings->Visuals.Players.Enable },
        { "RADAR",   &Settings->Visuals.Radar.Enable },
    };

    ShellDesc desc;
    desc.brand = "MODERN WARFARE";
    desc.sections = kSections;
    desc.pages = kPages;
    desc.pageCount = P_Count;
    desc.chips = chips;
    desc.chipCount = IM_ARRAYSIZE(chips);
    desc.menuKey = Settings->Configs.MenuKey;

    static int page = P_Aimbot;
    BeginShell(desc, page, ShowMenu);
    switch (page) {
    case P_Aimbot:   PageAimbot(Settings); break;
    case P_Players:  PagePlayers(Settings); break;
    case P_Radar:    PageRadar(Settings); break;
    case P_Settings: PageSettings(Settings, ShowMenu); break;
    case P_Configs:  ConfigsPage(Settings, g_pConfig, cfgname, (int)sizeof cfgname); break;
    }
    EndShell();
}
#endif
