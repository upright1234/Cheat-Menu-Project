#include "../GUI.h"
#include  <fstream>

#define  IMGUI_DEFINE_MATH_OPERATORS
#include "../../Imgui/imgui_internal.h"
#include "../main.h"
#include "../etc_element.h"

#if IsWarDogs
#include "../MenuKit.h"
using namespace kit;

char cfgname[30] = "NewSetting";

namespace {

const char* const kHitBoxes[]        = { "Head", "Chest", "Body", "Random" };
const char* const kPositions[]       = { "Top", "Bottom", "Left", "Right" };
const char* const kTopBottom[]       = { "Top", "Bottom" };
const char* const kBoxStyles[]       = { "Square", "Corner" };
const char* const kTargetShapes[]    = { "Circle", "Triangle" };
const char* const kOffscreenStyles[] = { "Triangle", "Dot" };

enum Page { P_Aimbot, P_Weapon, P_Players, P_World, P_Vehicles, P_Radar, P_Settings, P_Configs, P_Count };

const char* const kSections[] = { "COMBAT", "VISUALS", "SYSTEM" };
const NavItem kPages[P_Count] = {
    { "Aimbot",   "Targeting, smoothing, FOV and aim overlays", 0 },
    { "Weapon",   "Recoil, sway, ADS and movement tweaks",      0 },
    { "Players",  "Boxes, skeletons, labels and alerts",        1 },
    { "World",    "Loot, containers, bodies and deployables",   1 },
    { "Vehicles", "Vehicle ESP by type and team",               1 },
    { "Radar",    "Mini radar and off-screen indicators",       1 },
    { "Settings", "Menu key, performance, alerts and fonts",    2 },
    { "Configs",  "Save, load and create presets",              2 },
};

void ToggleStrength(const char* label, bool* on, int* percent, int mn = 0, int mx = 100) {
    ImGui::PushID(label);
    ImGui::Checkbox(label, on);
    if (*on)
        SliderI("Strength", percent, mn, mx, "%d%%");
    ImGui::PopID();
}

// Opens a panel for a player label (name / weapon / distance); caller adds extras and calls EndPanel()
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
    BeginPanel("Profile", ImGui::GetContentRegionAvail().x);
    {
        ImGui::Checkbox("Per-weapon profiles", &s->Aim.ByWeapon);
        if (s->Aim.ByWeapon) {
            ImGui::SameLine(RightEdge() - kComboW - ImGui::CalcTextSize("Editing").x - 10.f);
            ImGui::PushStyleColor(ImGuiCol_Text, kColDim);
            Label("Editing");
            ImGui::PopStyleColor();
            ImGui::SameLine(RightEdge() - kComboW);
            ImGui::SetNextItemWidth(kComboW + 60.f);
            ImGui::Combo("##weapon", &s->selectedWeapon, kWDWeaponCategoryNames, kWDWeaponCategoryCount, kWDWeaponCategoryCount);
        } else {
            const char* note = "Editing the global profile";
            ImGui::SameLine(RightEdge() - ImGui::CalcTextSize(note).x);
            ImGui::PushStyleColor(ImGuiCol_Text, kColDim);
            Label(note);
            ImGui::PopStyleColor();
        }
    }
    EndPanel();

    SETTINGS::AimStruct& aim = s->Aim.ByWeapon ? s->aimbot[s->selectedWeapon] : s->Aim;
    ImGui::PushID(&aim); // keeps widget state separate per profile

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
        SliderI("Max Distance", &aim.MaxDistance, 0, 1000, "%d m");
    }
    EndPanel();

    BeginPanel("Behavior");
    {
        ImGui::Checkbox("Visibility Check", &aim.VisibilityCheck);
        ImGui::Checkbox("Predict Movement", &aim.Predict);
        ImGui::Checkbox("Memory Aim", &aim.MemoryAimbot);
        ImGui::Checkbox("Aim Lock", &aim.AimLock);
        SliderI("Lock FOV", &aim.LockFov, 1, 30);
        ImGui::Checkbox("Keep Target", &aim.SaveTarget);
        if (aim.SaveTarget)
            SliderF("Target Switch Delay", &aim.TargetSwitchDelay, 0.f, 5.f, "%.1f s");
        ImGui::Checkbox("Dead Zone", &aim.DeadZone);
        if (aim.DeadZone)
            SliderF("Dead Zone Radius", &aim.DeadZoneRadius, 1.f, 20.f);
    }
    EndPanel();

    BeginPanel("Heal Bot");
    {
        ImGui::Checkbox("Enable", &aim.HealBotEnable);
        if (aim.HealBotEnable) {
            ImGui::Checkbox("Lowest HP First", &aim.HealBotPrioritiseLowest);
            KeyRow("Heal Key", &aim.HealBotKey);
            Hint("Hold the heal key to target allies instead of enemies.");
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
        if (aim.DrawFov) {
            SliderF("Thickness", &aim.FovThickness, 1.f, 5.f);
            ToggleColor("Filled", &aim.FovFilled, aim.FovFillColor);
            ToggleColor("Color On Target", &aim.FovChangeTargetingColor, aim.FovTargetingColor);
        }
    }
    EndPanel();

    BeginPanel("Target Indicator");
    {
        ToggleColor("Draw Target", &aim.DrawTarget, aim.TargetCollor);
        if (aim.DrawTarget) {
            ComboRow("Shape", &aim.TargetType, kTargetShapes, IM_ARRAYSIZE(kTargetShapes));
            SliderF("Size", &aim.TargetSize, 1.f, 10.f);
            SliderF("Thickness", &aim.TargetThickness, 1.f, 5.f);
            ImGui::Checkbox("Filled", &aim.TargetFilled);
            if (aim.TargetFilled)
                SliderI("Fill Opacity", &aim.TargetFillOpacity, 0, 100);
            ImGui::Checkbox("Target Line", &aim.TargetLine);
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
    EndColumns();

    ImGui::PopID();
}

void PageWeapon(SETTINGS* s) {
    auto& c = s->Configs;
    BeginColumns();
    BeginPanel("Weapon Handling");
    {
        ToggleStrength("No Recoil", &c.NoRecoil, &c.NoRecoilPercent);
        ToggleStrength("No Sway", &c.NoSway, &c.NoSwayPercent);
        ToggleStrength("Instant ADS", &c.InstantADS, &c.InstantADSPercent);
        ToggleStrength("Infinite Focus", &c.InfiniteFocus, &c.InfiniteFocusPercent);
    }
    EndPanel();

    NextColumn();
    BeginPanel("Movement");
    {
        ToggleStrength("Instant Lean", &c.InstantLean, &c.InstantLeanPercent);
        ToggleStrength("Infinite Stamina", &c.InfiniteStamina, &c.InfiniteStaminaPercent);
        ToggleStrength("No Weight", &c.NoWeight, &c.NoWeightPercent, 1, 50);
    }
    EndPanel();

    BeginPanel("Zoom");
    {
        ImGui::Checkbox("Enable", &c.Zoom);
        if (c.Zoom)
            SliderF("Multiplier", &c.ZoomMultiplier, 1.f, 15.f, "%.1fx");
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
        SliderI("Max Distance", &p.MaxDistance, 0, 1000, "%d m");
        SliderF("Min Font Size", &p.MinFontSize, 6.f, p.MaxFontSize, "%.0f px");
        SliderF("Max Font Size", &p.MaxFontSize, p.MinFontSize, 32.f, "%.0f px");
        ImGui::Checkbox("Fade With Distance", &p.DistanceTransparency);
        if (p.DistanceTransparency) {
            SliderF("Min Opacity", &p.MinTransparency, 0.f, p.MaxTransparency, "%.2f");
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
            SliderF("Width", &box.Width, 0.2f, 1.f, "%.2f");
            SliderF("Thickness", &box.Thickness, 1.f, 5.f);
            SliderI("Rounding", &box.rounding, 0, 20);
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
            SliderF("Size", &head.Size, 1.f, 15.f);
            SliderF("Thickness", &head.Thickness, 1.f, 5.f);
            ImGui::Checkbox("Filled", &head.Filled);
            ColorMode(head);
        }
    }
    EndPanel();

    BeginPanel("Health Bar");
    {
        auto& hp = p.Health;
        ImGui::Checkbox("Enable", &hp.Enable);
        if (hp.Enable) {
            ComboRow("Position", &hp.position, kPositions, IM_ARRAYSIZE(kPositions));
            SliderF("Thickness", &hp.Thickness, 1.f, 10.f);
            SliderI("Opacity", &hp.ColorAlpha, 0, 255);
            ImGui::Checkbox("Gradient", &hp.HealthGradient);
        }
    }
    EndPanel();

    NextColumn();
    BeginLabelPanel("Name", p.NickName, s->InstalledFonts);
    EndPanel();

    BeginLabelPanel("Weapon", p.Weapon, s->InstalledFonts);
    EndPanel();

    BeginLabelPanel("Distance", p.Distance, s->InstalledFonts);
    if (p.Distance.Enable)
        ImGui::Checkbox("Merge With Name", &p.Distance.MergeNickName);
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
                SliderF("Border Thickness", &prox.BorderThickness, 1.f, 10.f);
            ImGui::Checkbox("Warning Text", &prox.ProximityText);
            if (prox.ProximityText)
                SliderF("Text Size", &prox.TextSize, 10.f, 40.f, "%.0f px");
            ColorRow("Alert Color", prox.AlertColor);
            ColorRow("Text Color", prox.TextColor);
        }
    }
    EndPanel();
    EndColumns();
}

void PageWorld(SETTINGS* s) {
    auto& world = s->Visuals.WorldEsp;
    BeginColumns();
    BeginPanel("World ESP");
    {
        ImGui::Checkbox("Enable", &world.Enable);
        KeyRow("Toggle Key", &world.ToggleKey);
        SliderI("Max Distance", &world.MaxDistance, 0, 500, "%d m");
        SliderF("Min Font Size", &world.MinFontSize, 6.f, world.MaxFontSize, "%.0f px");
        SliderF("Max Font Size", &world.MaxFontSize, world.MinFontSize, 32.f, "%.0f px");
        ComboRow("Font", &world.FontStyle, s->InstalledFonts);
        Hint("Max Distance applies to ammo, containers, bodies and deployables. Item groups have their own range below.");
    }
    EndPanel();

    NextColumn();
    BeginPanel("Sources");
    {
        ImGui::Checkbox("Dropped Items", &world.ItemsEnable);
        ImGui::Checkbox("World Spawns", &world.LootablesEnable);
        ToggleColor("Containers", &world.ContainersEnable, world.ColorContainer);
        ToggleColor("Bodies", &world.BodiesEnable, world.ColorBody);
        ToggleColor("Deployables", &world.DeployablesEnable, world.ColorDeployable);
        auto& ammo = world.Categories.Style[WDC_Ammo];
        ToggleColor(kWDCategoryNames[WDC_Ammo], &ammo.Enable, ammo.Color);
    }
    EndPanel();
    EndColumns();

    // WDC_Other is never drawn by the cheat, so it is not listed
    static int group = WDG_Weapons;
    const WDCategory* items = kWDGroupWeapons;
    int count = IM_ARRAYSIZE(kWDGroupWeapons);
    switch (group) {
    case WDG_Equipment:   items = kWDGroupEquipment;   count = IM_ARRAYSIZE(kWDGroupEquipment);   break;
    case WDG_Gear:        items = kWDGroupGear;        count = IM_ARRAYSIZE(kWDGroupGear);        break;
    case WDG_Attachments: items = kWDGroupAttachments; count = IM_ARRAYSIZE(kWDGroupAttachments); break;
    }

    BeginPanel("Item Categories", ImGui::GetContentRegionAvail().x);
    {
        Segmented("##groups", &group, kWDItemGroupNames, kWDItemGroupCount);
        ImGui::PushID(group);
        SliderI("Group Max Distance", &world.GroupDistance[group], 0, 400, "%d m");

        const float half = (ImGui::GetContentRegionAvail().x - 8.f) * 0.5f;
        if (ActionButton("##all_on", "Enable All", ImVec2(half, 0.f)))
            for (int k = 0; k < count; ++k) world.Categories.Style[items[k]].Enable = true;
        ImGui::SameLine(0.f, 8.f);
        if (ActionButton("##all_off", "Disable All", ImVec2(half, 0.f)))
            for (int k = 0; k < count; ++k) world.Categories.Style[items[k]].Enable = false;
        ImGui::Dummy(ImVec2(0.f, 2.f));

        const float colGap = 28.f;
        const float startX = ImGui::GetCursorPosX();
        const float cellW = (ImGui::GetContentRegionAvail().x - colGap) * 0.5f;
        for (int k = 0; k < count; ++k) {
            const float x = startX + (k % 2) * (cellW + colGap);
            if (k % 2)
                ImGui::SameLine(x);
            auto& style = world.Categories.Style[items[k]];
            ImGui::PushID(k);
            ImGui::Checkbox(kWDCategoryNames[items[k]], &style.Enable);
            Swatch("##c", style.Color, x + cellW);
            ImGui::PopID();
        }
        ImGui::PopID();
    }
    EndPanel();
}

void PageVehicles(SETTINGS* s) {
    auto& v = s->Visuals.Vehicles;
    BeginColumns();
    BeginPanel("Vehicle ESP");
    {
        ImGui::Checkbox("Enable", &v.ShowVehicles);
        ImGui::Checkbox("Friendly Vehicles", &v.ShowFriendlyVehicles);
        ImGui::Checkbox("Enemy Vehicles", &v.ShowEnemyVehicles);
        ImGui::Checkbox("Occupants", &v.ShowOccupants);
        ImGui::Checkbox("Health", &v.ShowHealth);
        if (v.ShowHealth)
            ImGui::Checkbox("Hull Bar Only", &v.HealthBarOnly);
        SliderI("Max Distance", &v.MaxDistance, 0, 3000, "%d m");
    }
    EndPanel();

    NextColumn();
    BeginPanel("Vehicle Types");
    {
        Hint("Colors: friendly / enemy / unknown");
        const float gap = 8.f;
        const char* const tips[] = { "Friendly", "Enemy", "Unknown" };
        for (int g = 0; g < kWDVehicleGroupCount; ++g) {
            auto& style = v.Categories[g];
            float* cols[] = { style.FriendlyColor, style.EnemyColor, style.UnknownColor };
            ImGui::PushID(g);
            ImGui::Checkbox(kWDVehicleGroupNames[g], &style.Enable);
            for (int c = 0; c < 3; ++c) {
                ImGui::PushID(c);
                Swatch("##c", cols[c], RightEdge() - (2 - c) * (kSwatch + gap), kColorFlags, c == 0 ? 1.f : 0.f);
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("%s", tips[c]);
                ImGui::PopID();
            }
            ImGui::PopID();
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
            SliderI("Size", &r.Size, 50, 500, "%d px");
            SliderI("Zoom", &r.Zoom, 1, 100);
            SliderI("Max Distance", &r.MaxDistance, 0, 1000, "%d m");
            SliderI("Position X", &r.PositionX, 0, 3000);
            SliderI("Position Y", &r.PositionY, 0, 3000);
            SubHeader("Markers");
            SliderF("Min Size", &r.MinSize, 1.f, r.MaxSize);
            SliderF("Max Size", &r.MaxSize, r.MinSize, 10.f);
            SliderF("Min Opacity", &r.MinOpacity, 0.f, r.MaxOpacity, "%.2f");
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
            ComboRow("Style", &o.Type, kOffscreenStyles, IM_ARRAYSIZE(kOffscreenStyles));
            SliderI("FOV", &o.Fov, 1, 100);
            SliderI("Max Distance", &o.MaxDistance, 0, 1000, "%d m");
            SliderF("Thickness", &o.Thickness, 1.f, 5.f);
            SliderF("Min Size", &o.MinSize, 1.f, o.MaxSize, "%.0f px");
            SliderF("Max Size", &o.MaxSize, o.MinSize, 60.f, "%.0f px");
            SliderF("Min Opacity", &o.MinOpacity, 0.f, o.MaxOpacity, "%.2f");
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
        if (ActionButton("##hide", "Hide Menu"))
            *ShowMenu = false;
    }
    EndPanel();

    BeginPanel("Performance");
    {
        ImGui::Checkbox("VSync", &c.VSync);
        SliderI("Render FPS", &c.RenderFPS, 30, 1000);
        SliderI("Game FPS", &c.GameFPS, 30, 1000);
    }
    EndPanel();

    BeginPanel("Alerts");
    {
        ImGui::Checkbox("Spectator Count", &c.SpectatorCount);
        ImGui::Checkbox("Admin Spectator Alert", &c.AdminSpectatorAlert);
    }
    EndPanel();

    NextColumn();
    FontsPanel(s, [s](int font) {
        s->Visuals.Players.NickName.FontStyle = font;
        s->Visuals.Players.Weapon.FontStyle = font;
        s->Visuals.Players.Distance.FontStyle = font;
        s->Visuals.WorldEsp.FontStyle = font;
    });
    EndColumns();
}

} // namespace

void RenderMyMenu(bool* ShowMenu, SETTINGS* Settings, CConfig* g_pConfig) {
    MENU_LOG_FRAME(ShowMenu, Settings, g_pConfig);
    if (!ShowMenu || !Settings)
        return;

    if (Settings->selectedWeapon < 0 || Settings->selectedWeapon >= kWDWeaponCategoryCount)
        Settings->selectedWeapon = 0;

    SETTINGS::AimStruct& aim = Settings->Aim.ByWeapon ? Settings->aimbot[Settings->selectedWeapon] : Settings->Aim;

    const Chip chips[] = {
        { "AIM",   &aim.Enable },
        { "ESP",   &Settings->Visuals.Players.Enable },
        { "LOOT",  &Settings->Visuals.WorldEsp.Enable },
        { "RADAR", &Settings->Visuals.Radar.Enable },
    };

    ShellDesc desc;
    desc.brand = "WAR DOGS";
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
    case P_Weapon:   PageWeapon(Settings); break;
    case P_Players:  PagePlayers(Settings); break;
    case P_World:    PageWorld(Settings); break;
    case P_Vehicles: PageVehicles(Settings); break;
    case P_Radar:    PageRadar(Settings); break;
    case P_Settings: PageSettings(Settings, ShowMenu); break;
    case P_Configs:  ConfigsPage(Settings, g_pConfig, cfgname, (int)sizeof cfgname); break;
    }
    EndShell();
}
#endif
