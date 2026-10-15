#include "../GUI.h"
#include  <fstream>

#define  IMGUI_DEFINE_MATH_OPERATORS
#include "../../Imgui/imgui_internal.h"
#include "../main.h"
#include "../etc_element.h"

#if IsApex
#include "../MenuKit.h"
using namespace kit;

char cfgname[30] = "NewSetting";

namespace {

const char* const kWeapons[]       = { "AR", "DMR", "LMG", "Pistol", "Shotgun", "SMG", "Sniper" };
// Original "Weapon" list for Aim.ByWeapon: 0 = Disabled, n = aimbot[n - 1]
const char* const kByWeapon[]      = { "Disabled", "AR", "DMR", "LMG", "Pistol", "Shotgun", "SMG", "Sniper" };
const char* const kHitBoxes[]      = { "Head", "Chest", "Body", "Random" };
const char* const kPositions[]     = { "Top", "Bottom", "Left", "Right" };
const char* const kTopBottom[]     = { "Top", "Bottom" };
const char* const kBoxStyles[]     = { "Square", "Corner" };
const char* const kTargetShapes[]  = { "Circle", "Triangle" };
const char* const kSkyNadeModes[]  = { "Automatic", "Draw Only" };
const char* const kWeaponShow[]    = { "By Type", "By Name" };
const char* const kAttachShow[]    = { "By Type", "By Name", "Current Weapon" };
const char* const kAttachTypes[]   = { "Sights", "Barrel", "Magazine", "Stock", "Shotgun Bolt", "Hop-Up", "Laser" };
const char* const kGlowBody[] = {
    "None", "Pink", "Pink Visible Only", "Pulsing 1", "Pulsing Line Invisible Only", "Dark Pulsing Line",
    "Sharp Pulsing Visible Only", "Sharp Pulsing", "Pulsing Red Line", "Fast Pulsing Invisible", "Pulsing Up",
    "Solid Pulsing", "Solid Pulsing 2", "Bright", "Bright 2", "Light", "Light Solid", "Red Pulsing Visible Only",
    "Wave", "Shaded Visible", "Wireframe", "Wireframe Visible Only", "Black", "Black Visible Only",
};
const char* const kGlowOutline[] = {
    "None", "Bright", "Bright Invisible Only", "Dark", "Pink", "White", "Gold Flashing", "Gold", "Brown",
    "Wave", "Red", "Red Bright", "Heartbeat Visible Only", "Green Invisible Only", "Orange", "Visible Only", "Red 2",
};
constexpr int kWeaponCount = IM_ARRAYSIZE(kWeapons);

enum Page { P_Aimbot, P_Misc, P_Players, P_Labels, P_Loot, P_Radar, P_Settings, P_Configs, P_Count };

const char* const kSections[] = { "COMBAT", "VISUALS", "SYSTEM" };
const NavItem kPages[P_Count] = {
    { "Aimbot",   "Targeting, triggerbot, FOV and aim overlays", 0 },
    { "Misc",     "Movement, rapid fire and sky nade",           0 },
    { "Players",  "Glow, boxes, bars, lines and alerts",         1 },
    { "Labels",   "Name, weapon, distance and status text",      1 },
    { "Loot",     "Item ESP by category and item filters",       1 },
    { "Radar",    "Mini radar and off-screen indicators",        1 },
    { "Settings", "Menu key, performance and fonts",             2 },
    { "Configs",  "Save, load and create presets",               2 },
};

// ---------------------------------------------------------------------------
// Runtime item lists (ItemSelector, filled by the cheat); kit has no widget for these
// ---------------------------------------------------------------------------
bool ItemSelected(const ItemSelector& item, const std::vector<int>& selected) {
    for (int code : item.codes)
        if (std::find(selected.begin(), selected.end(), code) != selected.end()) return true;
    return false;
}

void SetItem(const ItemSelector& item, std::vector<int>& selected, bool on) {
    for (int code : item.codes) {
        auto it = std::find(selected.begin(), selected.end(), code);
        if (on && it == selected.end()) selected.push_back(code);
        else if (!on && it != selected.end()) selected.erase(it);
    }
}

void ItemFilterRow(const char* label, std::vector<ItemSelector>& list, std::vector<int>& selected) {
    ImGui::PushID(label);
    Label(label);
    ImGui::SameLine(RightEdge() - kComboW);
    ImGui::SetNextItemWidth(kComboW + 60.f);
    char preview[64] = "None";
    int count = 0, last = -1;
    for (int i = 0; i < (int)list.size(); ++i)
        if (ItemSelected(list[i], selected)) { ++count; last = i; }
    if (list.empty()) snprintf(preview, sizeof preview, "%s", "Loads in game");
    else if (count == (int)list.size()) snprintf(preview, sizeof preview, "%s", "All");
    else if (count == 1) snprintf(preview, sizeof preview, "%s", list[last].name.c_str());
    else if (count > 1) snprintf(preview, sizeof preview, "%d selected", count);
    if (ImGui::BeginCombo("##v", preview)) {
        if (list.empty()) {
            Hint("The item list is filled in once the game is running.");
        } else {
            if (ImGui::Selectable("Select All", false, ImGuiSelectableFlags_DontClosePopups))
                for (auto& item : list) SetItem(item, selected, true);
            if (ImGui::Selectable("Clear", false, ImGuiSelectableFlags_DontClosePopups))
                selected.clear();
            ImGui::Separator();
            for (int i = 0; i < (int)list.size(); ++i) {
                const bool on = ItemSelected(list[i], selected);
                ImGui::PushID(i);
                if (ImGui::Selectable(list[i].name.c_str(), on, ImGuiSelectableFlags_DontClosePopups))
                    SetItem(list[i], selected, !on);
                ImGui::PopID();
            }
        }
        ImGui::EndCombo();
    }
    ImGui::PopID();
}

// Rarity coloring + item filter shared by every list-based loot category
template <typename T>
void LootFilter(T& cat) {
    ImGui::Checkbox("Rarity Colors", &cat.rarity_color);
    ItemFilterRow("Items", cat.List, cat.Selected);
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

// Health / shield bar
template <typename T>
void BarPanel(const char* title, T& bar) {
    BeginPanel(title);
    ImGui::Checkbox("Enable", &bar.Enable);
    if (bar.Enable) {
        ComboRow("Position", &bar.position, kPositions, IM_ARRAYSIZE(kPositions));
        SliderF("Thickness", &bar.Thickness, 1.f, 10.f);
        SliderI("Opacity", &bar.ColorAlpha, 0, 255);
        ImGui::Checkbox("Gradient", &bar.Gradient);
    }
    EndPanel();
}

// Aim.ByWeapon: 0 = global profile, 1..7 = aimbot[ByWeapon - 1]
SETTINGS::AimStruct& CurrentProfile(SETTINGS* s) {
    return s->Aim.ByWeapon > 0 ? s->aimbot[s->Aim.ByWeapon - 1] : s->Aim;
}

void PageAimbot(SETTINGS* s) {
    // Same Aim.ByWeapon combo as the original menu
    BeginPanel("Configuration By Weapon", ImGui::GetContentRegionAvail().x);
    ComboRow("Weapon", &s->Aim.ByWeapon, kByWeapon, IM_ARRAYSIZE(kByWeapon));
    Hint(s->Aim.ByWeapon > 0 ? "Settings below apply to the selected weapon."
                             : "Disabled: settings below apply to every weapon.");
    EndPanel();

    SETTINGS::AimStruct& aim = CurrentProfile(s);
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
        ImGui::Checkbox("Visibility Check", &aim.VisibilityCheck);
        ImGui::Checkbox("Ignore Knocked", &aim.IgnoreKnocked);
        ImGui::Checkbox("Memory Aim", &aim.MemoryAim);
        ImGui::Checkbox("Predict Velocity", &aim.Predict[0]);
        ImGui::Checkbox("Predict Bullet Drop", &aim.Predict[1]);
        ImGui::Checkbox("Aim Lock", &aim.AimLock);
        ImGui::Checkbox("Keep Target", &aim.SaveTarget);
        if (aim.SaveTarget)
            SliderF("Target Switch Delay", &aim.TargetSwitchDelay, 0.f, 5.f, "%.1f s");
        ImGui::Checkbox("Dead Zone", &aim.DeadZone);
        if (aim.DeadZone)
            SliderF("Dead Zone Radius", &aim.DeadZoneRadius, 1.f, 20.f);
        SliderI("Max Spectators", &aim.MaxSpectators, 0, 20);
    }
    EndPanel();

    BeginPanel("Recoil");
    {
        ImGui::Checkbox("Recoil Control", &aim.RecoilControl);
        if (aim.RecoilControl)
            SliderI("Strength", &aim.RecoilControlValue, 1, 100, "%d%%");
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

    BeginPanel("Triggerbot");
    {
        ToggleKey("Enable", &aim.TriggerEnable, &aim.TriggerKey);
        if (aim.TriggerEnable) {
            ImGui::Checkbox("Require Key", &aim.TriggerByKey);
            ImGui::Checkbox("Only While Aiming", &aim.TriggerWhileADS);
            ImGui::Checkbox("Ignore Knocked", &aim.TriggerIgnoreKnocked);
            SliderI("Delay", &aim.TriggerDelay, 1, 2000, "%d ms");
            SliderI("Max Distance", &aim.TriggerMaxDistance, 1, 1000, "%d m");
        }
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

void PageMisc(SETTINGS* s) {
    auto& c = s->Configs;
    BeginColumns();
    BeginPanel("Movement");
    {
        ToggleKey("Bunny Hop", &c.Bhop, &c.BhopKey);
        KeyRow("Jump Key", &c.JumpKey);
        ImGui::Checkbox("Super Glide", &c.SuperGlide);
        ImGui::Checkbox("Tap Strafe", &c.TapStrafe);
        ImGui::Checkbox("Auto Grapple", &c.AutoGrapple);
        ToggleKey("Fake Duck", &c.FakeDuck, &c.FakeDuckKey);
    }
    EndPanel();

    NextColumn();
    BeginPanel("Rapid Fire");
    {
        ToggleKey("Enable", &c.RapidFire, &c.RapidFireKey);
        if (c.RapidFire)
            SliderI("Delay", &c.RapidFireDelay, 5, 200, "%d ms");
    }
    EndPanel();

    // Grenades aren't a weapon profile, so Sky Nade always edits the global profile
    BeginPanel("Sky Nade");
    {
        auto& nade = s->Aim.SkyNade;
        ToggleKey("Enable", &nade.Enable, &nade.Key);
        if (nade.Enable) {
            ComboRow("Mode", &nade.Mode, kSkyNadeModes, IM_ARRAYSIZE(kSkyNadeModes));
            ImGui::Checkbox("Visibility Check", &nade.VisibilityCheck);
            ToggleColor("Show Indicator", &nade.ShowIndicator, nade.IndicatorColor);
            if (nade.ShowIndicator)
                SliderF("Font Size", &nade.FontSize, 10.f, 30.f, "%.0f px");
        }
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
        ImGui::Checkbox("Ignore Knocked", &p.IgnoreKnocked);
        SliderI("Max Distance", &p.MaxDistance, 0, 800, "%d m");
        SliderF("Min Font Size", &p.MinFontSize, 10.f, p.MaxFontSize, "%.0f px");
        SliderF("Max Font Size", &p.MaxFontSize, p.MinFontSize, 30.f, "%.0f px");
        ImGui::Checkbox("Fade With Distance", &p.DistanceTransparency);
        if (p.DistanceTransparency) {
            SliderF("Min Opacity", &p.MinTransparency, 0.f, p.MaxTransparency, "%.2f");
            SliderF("Max Opacity", &p.MaxTransparency, p.MinTransparency, 1.f, "%.2f");
        }
    }
    EndPanel();

    BeginPanel("Glow");
    {
        auto& glow = p.Glow;
        ImGui::Checkbox("Enable", &glow.Enable);
        if (glow.Enable) {
            ComboRow("Body Style", &glow.BodyStyle, kGlowBody, IM_ARRAYSIZE(kGlowBody));
            ComboRow("Outline Style", &glow.OutlineStyle, kGlowOutline, IM_ARRAYSIZE(kGlowOutline));
            SliderI("Radius", &glow.Radius, 1, 200);
            ColorRow("Visible", glow.VisibleColor, kColorFlagsNoAlpha);
            ColorRow("Not Visible", glow.NonVisibleColor, kColorFlagsNoAlpha);
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

    BarPanel("Health Bar", p.Health);

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

    BarPanel("Shield Bar", p.Shield);

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

    BeginPanel("Prediction Lines");
    {
        auto& pl = p.PredictionLines;
        ImGui::Checkbox("Enable", &pl.Enable);
        if (pl.Enable) {
            SliderF("Prediction Time", &pl.PredictionTime, 0.5f, 5.f, "%.1f s");
            SliderF("Length", &pl.Length, 10.f, 200.f, "%.0f");
            SliderF("Thickness", &pl.Thickness, 1.f, 10.f);
            SliderF("Min Velocity", &pl.MinVelocity, 1.f, 20.f);
            ImGui::Checkbox("End Point", &pl.ShowEndPoint);
            if (pl.ShowEndPoint)
                SliderF("End Point Size", &pl.EndPointSize, 1.f, 10.f);
            ColorMode(pl);
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
                SliderF("Border Thickness", &prox.BorderThickness, 1.f, 20.f);
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

    BeginLabelPanel("Velocity", p.Velocity, fonts);
    EndPanel();
    EndColumns();
}

void PageLoot(SETTINGS* s) {
    auto& it = s->Visuals.Items;
    BeginColumns();
    BeginPanel("Loot ESP");
    {
        ImGui::Checkbox("Enable", &it.Enable);
        KeyRow("Toggle Key", &it.ToggleKey);
        ImGui::Checkbox("Text Labels", &it.Text);
        ImGui::Checkbox("Item Glow", &it.Glow);
        SliderI("Max Distance", &it.MaxDistance, 0, 800, "%d m");
        SliderF("Min Font Size", &it.MinFontSize, 10.f, it.MaxFontSize, "%.0f px");
        SliderF("Max Font Size", &it.MaxFontSize, it.MinFontSize, 30.f, "%.0f px");
        ComboRow("Font", &it.FontStyle, s->InstalledFonts);
        ImGui::Checkbox("Fade With Distance", &it.DistanceTransparency);
        if (it.DistanceTransparency) {
            SliderF("Min Opacity", &it.MinTransparency, 0.f, it.MaxTransparency, "%.2f");
            SliderF("Max Opacity", &it.MaxTransparency, it.MinTransparency, 1.f, "%.2f");
        }
    }
    EndPanel();

    NextColumn();
    BeginPanel("Categories");
    {
        Hint("Color is used when Rarity Colors is off.");
        ToggleColor("Weapons", &it.Weapon.Enable, it.Weapon.Color);
        ToggleColor("Ammo", &it.Ammo.Enable, it.Ammo.Color);
        ToggleColor("Attachments", &it.Attachment.Enable, it.Attachment.Color);
        ToggleColor("Grenades", &it.Grenades.Enable, it.Grenades.Color);
        ToggleColor("Medicine", &it.Medicine.Enable, it.Medicine.Color);
        ToggleColor("Helmets", &it.Helmet.Enable, it.Helmet.Color);
        ToggleColor("Body Shields", &it.Body.Enable, it.Body.Color);
        ToggleColor("Knockdown Shields", &it.Knockdown.Enable, it.Knockdown.Color);
        ToggleColor("Backpacks", &it.Backpack.Enable, it.Backpack.Color);
        ToggleColor("Death Boxes", &it.Deathbox.Enable, it.Deathbox.Color);
    }
    EndPanel();
    EndColumns();

    static const char* const kFilterCats[] = {
        "Weapons", "Ammo", "Attachments", "Grenades", "Medicine", "Helmets", "Body Shields", "Knockdown Shields", "Backpacks",
    };
    static int cat = 0;
    BeginPanel("Category Filter", ImGui::GetContentRegionAvail().x);
    {
        ComboRow("Category", &cat, kFilterCats, IM_ARRAYSIZE(kFilterCats));
        ImGui::PushID(cat);
        switch (cat) {
        case 0: {
            auto& w = it.Weapon;
            ImGui::Checkbox("Rarity Colors", &w.rarity_color);
            ComboRow("Show", &w.ShowType, kWeaponShow, IM_ARRAYSIZE(kWeaponShow));
            if (w.ShowType == 0) MultiComboRow("Types", w.Types, kWeapons, kWeaponCount);
            else ItemFilterRow("Items", w.List, w.Selected);
            break;
        }
        case 1: LootFilter(it.Ammo); break;
        case 2: {
            auto& a = it.Attachment;
            ImGui::Checkbox("Rarity Colors", &a.rarity_color);
            ComboRow("Show", &a.ShowType, kAttachShow, IM_ARRAYSIZE(kAttachShow));
            if (a.ShowType == 0) MultiComboRow("Types", a.Types, kAttachTypes, IM_ARRAYSIZE(kAttachTypes));
            else if (a.ShowType == 1) ItemFilterRow("Items", a.List, a.Selected);
            else Hint("Shows attachments that fit the weapon you are holding.");
            break;
        }
        case 3: LootFilter(it.Grenades); break;
        case 4: LootFilter(it.Medicine); break;
        case 5: LootFilter(it.Helmet); break;
        case 6: LootFilter(it.Body); break;
        case 7: LootFilter(it.Knockdown); break;
        case 8: LootFilter(it.Backpack); break;
        }
        ImGui::PopID();
    }
    EndPanel();
}

void PageRadar(SETTINGS* s) {
    BeginColumns();
    BeginPanel("Radar");
    {
        auto& r = s->Visuals.Radar;
        ToggleKey("Enable", &r.Enable, &r.ToggleKey);
        if (r.Enable) {
            SliderI("Size", &r.Size, 50, 500, "%d px");
            SliderI("Zoom", &r.Zoom, 1, 10);
            SliderI("Max Distance", &r.MaxDistance, 0, 800, "%d m");
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
            ComboRow("Style", &o.Type, kTargetShapes, IM_ARRAYSIZE(kTargetShapes));
            SliderI("FOV", &o.Fov, 1, 180);
            SliderI("Max Distance", &o.MaxDistance, 0, 800, "%d m");
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
        ImGui::Checkbox("Feature Descriptions", &c.FeaturesDefinition);
        if (ActionButton("##hide", "Hide Menu"))
            *ShowMenu = false;
    }
    EndPanel();

    BeginPanel("Performance");
    {
        ImGui::Checkbox("VSync", &c.VSync);
        SliderI("Render FPS", &c.RenderFPS, 30, 360);
        SliderI("Game FPS", &c.GameFPS, 30, 1000);
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

    SETTINGS::AimStruct& aim = CurrentProfile(Settings);

    const Chip chips[] = {
        { "AIM",   &aim.Enable },
        { "ESP",   &Settings->Visuals.Players.Enable },
        { "LOOT",  &Settings->Visuals.Items.Enable },
        { "RADAR", &Settings->Visuals.Radar.Enable },
    };

    ShellDesc desc;
    desc.brand = "APEX LEGENDS";
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
    case P_Misc:     PageMisc(Settings); break;
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
