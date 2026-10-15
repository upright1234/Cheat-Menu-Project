#include "../GUI.h"
#include  <fstream>

#define  IMGUI_DEFINE_MATH_OPERATORS
#include "../../Imgui/imgui_internal.h"
#include "../main.h"
#include "../etc_element.h"

#if IsPUBG
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
const char* const kVisChecks[]       = { "Disabled", "Default", "Raycast" };
const char* const kInterpolation[]   = { "Linear", "Bezier" };
const char* const kShowTypes[]       = { "By Type", "By Name" };
const char* const kAmmoShowTypes[]   = { "By Held Weapon", "By Type", "By Weapon Name", "By Name" };
const char* const kVehicleTypes[]    = { "Road", "Sailing" };
// aimbot[] order; kByWeapon is the original "Weapon" list for Aim.ByWeapon (0 = Disabled, n = aimbot[n - 1])
const char* const kWeaponTypes[]     = { "AR", "DMR", "LMG", "Melee", "Pistol", "Shotgun", "SMG", "Sniper", "Special", "Throw" };
const char* const kByWeapon[]        = { "Disabled", "AR", "DMR", "LMG", "Melee", "Pistol", "Shotgun", "SMG", "Sniper", "Special", "Throw" };
constexpr int kWeaponCount = IM_ARRAYSIZE(kWeaponTypes);

enum Page { P_Aimbot, P_Assist, P_Players, P_Labels, P_Loot, P_Radar, P_Settings, P_Configs, P_Count };

const char* const kSections[] = { "COMBAT", "VISUALS", "SYSTEM" };
const NavItem kPages[P_Count] = {
    { "Aimbot",   "Targeting, smoothing, FOV and aim overlays",  0 },
    { "Assist",   "Triggerbot, humanization and kill switch",    0 },
    { "Players",  "Boxes, skeletons, lines and alerts",          1 },
    { "Labels",   "Name, weapon, distance, status and speed",    1 },
    { "Loot",     "Items, vehicles and packages by category",    1 },
    { "Radar",    "Mini radar and off-screen indicators",        1 },
    { "Settings", "Menu key, performance and fonts",             2 },
    { "Configs",  "Save, load and create presets",               2 },
};

SETTINGS::AimStruct& CurrentAim(SETTINGS* s) {
    return s->Aim.ByWeapon > 0 ? s->aimbot[s->Aim.ByWeapon - 1] : s->Aim;
}

// Configuration by weapon, shared by both combat pages (same Aim.ByWeapon combo as the original menu)
void ProfilePanel(SETTINGS* s) {
    BeginPanel("Configuration By Weapon", ImGui::GetContentRegionAvail().x);
    ComboRow("Weapon", &s->Aim.ByWeapon, kByWeapon, IM_ARRAYSIZE(kByWeapon));
    Hint(s->Aim.ByWeapon > 0 ? "Settings below apply to the selected weapon."
                             : "Disabled: settings below apply to every weapon.");
    EndPanel();
}

bool AnyCodeSelected(const ItemSelector& item, const std::vector<int>& selected) {
    for (int code : item.codes)
        if (std::find(selected.begin(), selected.end(), code) != selected.end())
            return true;
    return false;
}

void SetItemSelected(const ItemSelector& item, std::vector<int>& selected, bool on) {
    for (int code : item.codes) {
        auto it = std::find(selected.begin(), selected.end(), code);
        if (on && it == selected.end()) selected.push_back(code);
        else if (!on && it != selected.end()) selected.erase(it);
    }
}

// Multi-select over an ItemSelector list (replaces ImGui::MultiComboVector, which has a fixed 250px width)
void ItemListRow(const char* label, std::vector<ItemSelector>& items, std::vector<int>& selected) {
    ImGui::PushID(label);
    Label(label);
    ImGui::SameLine(RightEdge() - kComboW);
    ImGui::SetNextItemWidth(kComboW + 60.f);

    int count = 0;
    const char* first = nullptr;
    for (const auto& item : items) {
        if (AnyCodeSelected(item, selected)) {
            if (!first) first = item.name.c_str();
            ++count;
        }
    }
    char preview[64];
    if (items.empty())   snprintf(preview, sizeof preview, "No items");
    else if (count == 0) snprintf(preview, sizeof preview, "None");
    else if (count == 1) snprintf(preview, sizeof preview, "%s", first);
    else                 snprintf(preview, sizeof preview, "%d selected", count);

    if (ImGui::BeginCombo("##v", preview)) {
        if (!items.empty()) {
            const float half = (ImGui::GetContentRegionAvail().x - 8.f) * 0.5f;
            if (ActionButton("##all", "Select All", ImVec2(half, 0.f)))
                for (const auto& item : items) SetItemSelected(item, selected, true);
            ImGui::SameLine(0.f, 8.f);
            if (ActionButton("##none", "Clear", ImVec2(half, 0.f)))
                for (const auto& item : items) SetItemSelected(item, selected, false);
        }
        for (int i = 0; i < (int)items.size(); ++i) {
            const bool on = AnyCodeSelected(items[i], selected);
            ImGui::PushID(i);
            if (ImGui::Selectable(items[i].name.c_str(), on, ImGuiSelectableFlags_DontClosePopups))
                SetItemSelected(items[i], selected, !on);
            ImGui::PopID();
        }
        ImGui::EndCombo();
    }
    ImGui::PopID();
}

// Loot category with only a name list (Package, Medicine, ...)
template <typename T>
void SimpleCategory(const char* title, T& cat) {
    ImGui::PushID(title);
    ToggleColor(title, &cat.Enable, cat.Color);
    if (cat.Enable)
        ItemListRow("Items", cat.List, cat.Selected);
    ImGui::PopID();
}

// Ammo / Attachment: by held weapon, by weapon type, by weapon name or by item name
template <typename T>
void WeaponLinkedCategory(const char* title, T& cat, std::vector<ItemSelector>& weaponList) {
    ImGui::PushID(title);
    ToggleColor(title, &cat.Enable, cat.Color);
    if (cat.Enable) {
        ComboRow("Show", &cat.ShowType, kAmmoShowTypes, IM_ARRAYSIZE(kAmmoShowTypes));
        switch (cat.ShowType) {
        case 1: MultiComboRow("Weapon Types", cat.Types, kWeaponTypes, kWeaponCount); break;
        case 2: ItemListRow("Weapons", weaponList, cat.SelectedWeapon); break;
        case 3: ItemListRow("Items", cat.List, cat.Selected); break;
        }
    }
    ImGui::PopID();
}

// Opens a panel for a player label; caller adds extras and calls EndPanel()
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
    ProfilePanel(s);
    SETTINGS::AimStruct& aim = CurrentAim(s);
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
        SliderI("Max Distance", &aim.MaxDistance, 1, 1000, "%d m");
    }
    EndPanel();

    BeginPanel("Behavior");
    {
        ComboRow("Visibility Check", &aim.VisibilityCheck, kVisChecks, IM_ARRAYSIZE(kVisChecks));
        ImGui::Checkbox("Predict Movement", &aim.Predict);
        ImGui::Checkbox("Recoil Control", &aim.RecoilControl);
        ImGui::Checkbox("Ignore Knocked", &aim.IgnoreKnocked);
        ImGui::Checkbox("Ignore Driving", &aim.IgnoreDriving);
        ImGui::Checkbox("Ignore Bots", &aim.IgnoreBots);
        ImGui::Checkbox("Aim Lock", &aim.AimLock);
        ImGui::Checkbox("Keep Target", &aim.SaveTarget);
        if (aim.SaveTarget)
            SliderF("Target Switch Delay", &aim.TargetSwitchDelay, 0.f, 5.f, "%.1f s");
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
        ImGui::Checkbox("Dead Zone", &aim.DeadZone);
        if (aim.DeadZone)
            SliderF("Dead Zone Radius", &aim.DeadZoneRadius, 1.f, 20.f);
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
    EndColumns();

    ImGui::PopID();
}

void PageAssist(SETTINGS* s) {
    ProfilePanel(s);
    SETTINGS::AimStruct& aim = CurrentAim(s);
    ImGui::PushID(&aim);

    BeginColumns();
    BeginPanel("Triggerbot");
    {
        ImGui::Checkbox("Enable", &aim.TriggerBot);
        if (aim.TriggerBot) {
            KeyRow("Trigger Key", &aim.TriggerKey);
            SliderF("Delay", &aim.TriggerDelay, 0.f, 1.f, "%.2f s");
            SliderF("Trigger FOV", &aim.TriggerFOV, 1.f, 10.f);
            ToggleColor("Draw FOV", &aim.TriggerBotDrawFOV, aim.TriggerBotFOVColor);
            if (aim.TriggerBotDrawFOV)
                SliderF("Thickness", &aim.TriggerBotFOVThickness, 1.f, 5.f);
        }
    }
    EndPanel();

    BeginPanel("Smooth Interpolation");
    {
        ImGui::Checkbox("Enable", &aim.SmoothInterpolation);
        if (aim.SmoothInterpolation) {
            ComboRow("Curve", &aim.InterpolationType, kInterpolation, IM_ARRAYSIZE(kInterpolation));
            SliderF("Speed", &aim.InterpolationSpeed, 0.1f, 1.f, "%.2f");
        }
    }
    EndPanel();

    NextColumn();
    BeginPanel("Humanization");
    {
        ImGui::Checkbox("Enable", &aim.Humanization);
        if (aim.Humanization) {
            SliderF("Reaction Time", &aim.ReactionTime, 0.05f, 0.5f, "%.2f s");
            SliderF("Random Offset", &aim.RandomOffset, 0.1f, 2.f, "%.1f");
        }
    }
    EndPanel();

    BeginPanel("Safety");
    {
        ImGui::Checkbox("Kill Switch", &aim.KillSwitch);
        KeyRow("Kill Switch Key", &aim.KillSwitchKey);
        ImGui::Checkbox("Auto Kill Switch", &aim.AutoKillSwitch);
        if (aim.AutoKillSwitch)
            SliderF("Trigger Distance", &aim.KillSwitchDistance, 0.f, 150.f, "%.0f m");
        SliderI("Max Spectators", &aim.MaxSpectators, 0, 20);
        Hint("Aim pauses while more players than this are spectating you.");
    }
    EndPanel();
    EndColumns();

    ImGui::PopID();
}

void PagePlayers(SETTINGS* s) {
    auto& p = s->Visuals.Players;
    BeginColumns();
    BeginPanel("Player ESP");
    {
        ImGui::Checkbox("Enable", &p.Enable);
        KeyRow("Toggle Key", &p.ToggleKey);
        SliderI("Max Distance", &p.MaxDistance, 1, 1000, "%d m");
        SliderF("Min Font Size", &p.MinFontSize, 10.f, p.MaxFontSize, "%.0f px");
        SliderF("Max Font Size", &p.MaxFontSize, p.MinFontSize, 30.f, "%.0f px");
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

    NextColumn();
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

    BeginPanel("Prediction Lines");
    {
        auto& pred = p.PredictionLines;
        ImGui::Checkbox("Enable", &pred.Enable);
        if (pred.Enable) {
            SliderF("Look Ahead", &pred.PredictionTime, 0.5f, 5.f, "%.1f s");
            SliderF("Length", &pred.Length, 10.f, 200.f, "%.0f");
            SliderF("Thickness", &pred.Thickness, 1.f, 5.f);
            SliderF("Min Speed", &pred.MinVelocity, 1.f, 20.f, "%.0f");
            ImGui::Checkbox("End Point", &pred.ShowEndPoint);
            if (pred.ShowEndPoint)
                SliderF("End Point Size", &pred.EndPointSize, 1.f, 10.f);
            ColorMode(pred);
        }
    }
    EndPanel();

    BeginPanel("Proximity Alert");
    {
        auto& prox = p.ProximityAlerts;
        ImGui::Checkbox("Enable", &prox.Enable);
        if (prox.Enable) {
            SliderF("Radius", &prox.ProximityRadius, 10.f, 200.f, "%.0f m");
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

void PageLabels(SETTINGS* s) {
    auto& p = s->Visuals.Players;
    const auto& fonts = s->InstalledFonts;
    BeginColumns();
    BeginLabelPanel("Name", p.NickName, fonts);
    EndPanel();

    BeginLabelPanel("Weapon", p.Weapon, fonts);
    EndPanel();

    BeginLabelPanel("Distance", p.Distance, fonts);
    if (p.Distance.Enable)
        ImGui::Checkbox("Merge With Name", &p.Distance.MergeNickName);
    EndPanel();

    NextColumn();
    BeginLabelPanel("Status", p.Status, fonts);
    EndPanel();

    BeginLabelPanel("Speed", p.Velocity, fonts);
    EndPanel();
    EndColumns();
}

void PageLoot(SETTINGS* s) {
    auto& it = s->Visuals.Items;
    BeginColumns();
    BeginPanel("Item ESP");
    {
        ImGui::Checkbox("Enable", &it.Enable);
        KeyRow("Toggle Key", &it.ToggleKey);
        SliderI("Max Distance", &it.MaxDistance, 1, 1000, "%d m");
        SliderF("Min Font Size", &it.MinFontSize, 10.f, it.MaxFontSize, "%.0f px");
        SliderF("Max Font Size", &it.MaxFontSize, it.MinFontSize, 30.f, "%.0f px");
        ComboRow("Font", &it.FontStyle, s->InstalledFonts);
    }
    EndPanel();

    BeginPanel("Weapons & Vehicles");
    {
        ImGui::PushID("weapon");
        ToggleColor("Weapons", &it.Weapon.Enable, it.Weapon.Color);
        if (it.Weapon.Enable) {
            ComboRow("Show", &it.Weapon.ShowType, kShowTypes, IM_ARRAYSIZE(kShowTypes));
            if (it.Weapon.ShowType == 0) MultiComboRow("Types", it.Weapon.Types, kWeaponTypes, kWeaponCount);
            else                         ItemListRow("Items", it.Weapon.List, it.Weapon.Selected);
        }
        ImGui::PopID();

        ImGui::PushID("vehicle");
        ToggleColor("Vehicles", &it.Vehicle.Enable, it.Vehicle.Color);
        if (it.Vehicle.Enable) {
            ComboRow("Show", &it.Vehicle.ShowType, kShowTypes, IM_ARRAYSIZE(kShowTypes));
            if (it.Vehicle.ShowType == 0) MultiComboRow("Types", it.Vehicle.Types, kVehicleTypes, IM_ARRAYSIZE(kVehicleTypes));
            else                          ItemListRow("Items", it.Vehicle.List, it.Vehicle.Selected);
        }
        ImGui::PopID();
    }
    EndPanel();

    NextColumn();
    BeginPanel("Ammo & Attachments");
    {
        WeaponLinkedCategory("Ammo", it.Ammo, it.Weapon.List);
        WeaponLinkedCategory("Attachments", it.Attachment, it.Weapon.List);
    }
    EndPanel();

    BeginPanel("Supplies");
    {
        SimpleCategory("Medicine", it.Medicine);
        SimpleCategory("Throwables", it.Throwables);
        SimpleCategory("Equipment", it.Equipable);
        SimpleCategory("Other Gear", it.OtherEquipable);
        SimpleCategory("Packages", it.Package);
    }
    EndPanel();

    BeginPanel("Rarity Colors");
    {
        auto& r = s->Rarity;
        ImGui::Checkbox("Color By Rarity", &r.Enable);
        if (r.Enable) {
            ColorRow("Common", r.CommonColor);
            ColorRow("Rare", r.RareColor);
            ColorRow("Epic", r.EpicColor);
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
            SliderI("Max Distance", &r.MaxDistance, 1, 1000, "%d m");
            SliderI("Position X", &r.PositionX, 0, 3000);
            SliderI("Position Y", &r.PositionY, 0, 3000);
            SubHeader("Markers");
            SliderF("Min Size", &r.MinSize, 1.f, r.MaxSize);
            SliderF("Max Size", &r.MaxSize, r.MinSize, 6.f);
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
            SliderI("FOV", &o.Fov, 1, 180);
            SliderI("Max Distance", &o.MaxDistance, 1, 1000, "%d m");
            SliderF("Thickness", &o.Thickness, 1.f, 5.f);
            SliderF("Min Size", &o.MinSize, 1.f, o.MaxSize, "%.0f px");
            SliderF("Max Size", &o.MaxSize, o.MinSize, 30.f, "%.0f px");
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
        SliderI("Render FPS", &c.RenderFPS, 30, 360);
        SliderI("Game FPS", &c.GameFPS, 30, 240);
        Hint("Game FPS should match your in-game frame rate.");
    }
    EndPanel();

    BeginPanel("Alerts");
    {
        ImGui::Checkbox("Spectator Count", &c.SpectatorCount);
    }
    EndPanel();

    NextColumn();
    FontsPanel(s, [s](int font) {
        auto& p = s->Visuals.Players;
        p.NickName.FontStyle = font;
        p.Weapon.FontStyle = font;
        p.Distance.FontStyle = font;
        p.Status.FontStyle = font;
        p.Velocity.FontStyle = font;
        s->Visuals.Items.FontStyle = font;
    });
    EndColumns();
}

} // namespace

void RenderMyMenu(bool* ShowMenu, SETTINGS* Settings, CConfig* g_pConfig) {
    MENU_LOG_FRAME(ShowMenu, Settings, g_pConfig);
    if (!ShowMenu || !Settings)
        return;

    if (Settings->Aim.ByWeapon < 0 || Settings->Aim.ByWeapon > kWeaponCount)
        Settings->Aim.ByWeapon = 0;

    SETTINGS::AimStruct& aim = CurrentAim(Settings);

    const Chip chips[] = {
        { "AIM",   &aim.Enable },
        { "ESP",   &Settings->Visuals.Players.Enable },
        { "LOOT",  &Settings->Visuals.Items.Enable },
        { "RADAR", &Settings->Visuals.Radar.Enable },
    };

    ShellDesc desc;
    desc.brand = "PUBG";
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
    case P_Assist:   PageAssist(Settings); break;
    case P_Players:  PagePlayers(Settings); break;
    case P_Labels:   PageLabels(Settings); break;
    case P_Loot:     PageLoot(Settings); break;
    case P_Radar:    PageRadar(Settings); break;
    case P_Settings: PageSettings(Settings, ShowMenu); break;
    case P_Configs:  ConfigsPage(Settings, g_pConfig, cfgname, (int)sizeof cfgname); break;
    }
    EndShell();
}
#endif
