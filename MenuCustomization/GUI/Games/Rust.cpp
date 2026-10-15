#include "../GUI.h"
#include  <fstream>

#define  IMGUI_DEFINE_MATH_OPERATORS
#include "../../Imgui/imgui_internal.h"
#include "../main.h"
#include "../etc_element.h"

#if IsRust
#include "../MenuKit.h"
using namespace kit;

char cfgname[30] = "NewSetting";

namespace {

const char* const kHitBoxes[]      = { "Head", "Chest", "Body", "Nearest" };
const char* const kPositions[]     = { "Top", "Bottom", "Left", "Right" };
const char* const kTopBottom[]     = { "Top", "Bottom" };
const char* const kBoxStyles[]     = { "Square", "Corner" };
const char* const kTargetShapes[]  = { "Circle", "Triangle" };
const char* const kRadarMarkers[]  = { "Dot", "Arrow" };
const char* const kChamsMaterials[] = { "Red", "White (through walls)", "Mirror (through walls)", "Gray (through walls)", "Bright Sky Blue" };
// Rust item categories 0-13, used by Items.WorldDrop.Selected
const char* const kDropCategories[] = {
    "Weapon", "Construction", "Items", "Resources", "Attire", "Tool", "Medical",
    "Food", "Ammunition", "Traps", "Misc", "All", "Common", "Component",
};

enum Page { P_Aimbot, P_Weapon, P_Players, P_Labels, P_NPC, P_Loot, P_Radar, P_World, P_Settings, P_Configs, P_Count };

const char* const kSections[] = { "COMBAT", "VISUALS", "SYSTEM" };
const NavItem kPages[P_Count] = {
    { "Aimbot",   "Targeting, smoothing, FOV and aim overlays", 0 },
    { "Weapon",   "Triggerbot, weapon mods and movement",       0 },
    { "Players",  "Boxes, skeletons, lines and alerts",         1 },
    { "Labels",   "Player text and hotbar",                     1 },
    { "NPCs",     "Scientists and other NPC ESP",               1 },
    { "Loot",     "Drops, resources, crates and deployables",   1 },
    { "Radar",    "Mini radar and off-screen indicators",       1 },
    { "World",    "Camera, environment and chams",              1 },
    { "Settings", "Menu key, performance and fonts",            2 },
    { "Configs",  "Save, load and create presets",              2 },
};

// ---------------------------------------------------------------------------
// Local list widgets (kit has no ItemSelector / index-vector multi-select)
// ---------------------------------------------------------------------------
bool Contains(const std::vector<int>& v, int x) { return std::find(v.begin(), v.end(), x) != v.end(); }

bool ItemSelected(const ItemSelector& item, const std::vector<int>& selected) {
    for (int code : item.codes)
        if (Contains(selected, code)) return true;
    return false;
}

void SetItem(const ItemSelector& item, std::vector<int>& selected, bool on) {
    for (int code : item.codes) {
        auto it = std::find(selected.begin(), selected.end(), code);
        if (on && it == selected.end()) selected.push_back(code);
        else if (!on && it != selected.end()) selected.erase(it);
    }
}

// Multi-select over a runtime item list (filled by the cheat); stores every code of each picked item
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

// Multi-select that stores picked indices in a vector<int>
void IndexMultiRow(const char* label, std::vector<int>& selected, const char* const items[], int count) {
    ImGui::PushID(label);
    Label(label);
    ImGui::SameLine(RightEdge() - kComboW);
    ImGui::SetNextItemWidth(kComboW + 60.f);
    char preview[64] = "None";
    int n = 0, last = -1;
    for (int i = 0; i < count; ++i)
        if (Contains(selected, i)) { ++n; last = i; }
    if (n == 1) snprintf(preview, sizeof preview, "%s", items[last]);
    else if (n > 1) snprintf(preview, sizeof preview, "%d selected", n);
    if (ImGui::BeginCombo("##v", preview)) {
        for (int i = 0; i < count; ++i) {
            const bool on = Contains(selected, i);
            if (ImGui::Selectable(items[i], on, ImGuiSelectableFlags_DontClosePopups)) {
                if (on) selected.erase(std::find(selected.begin(), selected.end(), i));
                else selected.push_back(i);
            }
        }
        ImGui::EndCombo();
    }
    ImGui::PopID();
}

// One loot category: toggle + color, then its item filter when enabled
template <typename T>
void LootCategory(const char* name, T& cat) {
    ImGui::PushID(&cat);
    ToggleColor(name, &cat.Enable, cat.Color);
    if (cat.Enable)
        ItemFilterRow("Show", cat.List, cat.Selected);
    ImGui::PopID();
}

// Opens a panel for a player label (name / weapon / distance ...); caller adds extras and calls EndPanel()
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

template <typename T>
void BoxPanel(T& box) {
    BeginPanel("Box");
    ImGui::Checkbox("Enable", &box.Enable);
    if (box.Enable) {
        ComboRow("Style", &box.Type, kBoxStyles, IM_ARRAYSIZE(kBoxStyles));
        SliderF("Thickness", &box.Thickness, 1.f, 5.f);
        SliderI("Rounding", &box.rounding, 0, 20);
        ToggleColor("Filled", &box.Filled, box.FillColor);
        ColorMode(box);
    }
    EndPanel();
}

template <typename T>
void SkeletonPanel(T& sk) {
    BeginPanel("Skeleton");
    ImGui::Checkbox("Enable", &sk.Enable);
    if (sk.Enable) {
        SliderF("Thickness", &sk.Thickness, 1.f, 5.f, "%.2f");
        ColorMode(sk);
    }
    EndPanel();
}

template <typename T>
void HeadCirclePanel(T& head) {
    BeginPanel("Head Circle");
    ImGui::Checkbox("Enable", &head.Enable);
    if (head.Enable) {
        SliderF("Size", &head.Size, 1.f, 15.f);
        SliderF("Thickness", &head.Thickness, 1.f, 5.f);
        ImGui::Checkbox("Filled", &head.Filled);
        ColorMode(head);
    }
    EndPanel();
}

template <typename T>
void LinesPanel(T& lines) {
    BeginPanel("Snaplines");
    ImGui::Checkbox("Enable", &lines.Enable);
    if (lines.Enable) {
        ComboRow("Origin", &lines.Position, kTopBottom, IM_ARRAYSIZE(kTopBottom));
        SliderF("Thickness", &lines.Thickness, 1.f, 5.f);
        ColorMode(lines);
    }
    EndPanel();
}

// Shared distance / font size / fade block of the Players, NPC and Loot ESP panels
template <typename T>
void RangeAndFade(T& e, int maxDistance) {
    SliderI("Max Distance", &e.MaxDistance, 1, maxDistance, "%d m");
    SliderF("Min Font Size", &e.MinFontSize, 8.f, e.MaxFontSize, "%.0f px");
    SliderF("Max Font Size", &e.MaxFontSize, e.MinFontSize, 32.f, "%.0f px");
    ImGui::Checkbox("Fade With Distance", &e.DistanceTransparency);
    if (e.DistanceTransparency) {
        SliderF("Min Opacity", &e.MinTransparency, 0.f, e.MaxTransparency, "%.2f");
        SliderF("Max Opacity", &e.MaxTransparency, e.MinTransparency, 1.f, "%.2f");
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
        SliderF("Smoothing", &aim.Smooth, 1.f, 20.f);
        SliderI("Max Distance", &aim.MaxDistance, 1, 1000, "%d m");
    }
    EndPanel();

    BeginPanel("Behavior");
    {
        ImGui::Checkbox("Silent Aim", &s->Configs.SilentAim);
        ImGui::Checkbox("Memory Aim", &aim.MemoryAim);
        ImGui::Checkbox("Visibility Check", &aim.VisibilityCheck);
        ImGui::Checkbox("Target NPCs", &aim.AimNPC);
        ImGui::Checkbox("Ignore Wounded", &aim.IgnoreWounded);
        ImGui::Checkbox("Ignore Sleeping", &aim.IgnoreSleeping);
        ImGui::Checkbox("Predict Movement", &aim.Predict);
        ImGui::Checkbox("Aim Lock", &aim.AimLock);
        if (aim.AimLock)
            SliderI("Lock FOV", &aim.LockFov, 1, 30);
        ImGui::Checkbox("Keep Target", &aim.SaveTarget);
        if (aim.SaveTarget)
            SliderF("Target Switch Delay", &aim.TargetSwitchDelay, 0.f, 5.f, "%.1f s");
        ImGui::Checkbox("Dead Zone", &aim.DeadZone);
        if (aim.DeadZone)
            SliderF("Dead Zone Radius", &aim.DeadZoneRadius, 1.f, 20.f);
    }
    EndPanel();

    BeginPanel("Crosshair");
    {
        ToggleColor("Draw Crosshair", &aim.DrawCrossHair, aim.CrossHairColor);
        if (aim.DrawCrossHair)
            SliderF("Thickness", &aim.CrossHairThickness, 1.f, 5.f);
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
            SliderF("Size", &aim.TargetSize, 1.f, 20.f);
            SliderF("Thickness", &aim.TargetThickness, 1.f, 5.f);
            ImGui::Checkbox("Filled", &aim.TargetFilled);
            if (aim.TargetFilled)
                SliderI("Fill Opacity", &aim.TargetFillOpacity, 0, 255);
            ImGui::Checkbox("Target Line", &aim.TargetLine);
        }
    }
    EndPanel();
    EndColumns();
}

void PageWeapon(SETTINGS* s) {
    auto& aim = s->Aimbot;
    auto& c = s->Configs;
    BeginColumns();
    BeginPanel("Triggerbot");
    {
        ToggleKey("Enable", &aim.TriggerEnable, &aim.TriggerKey);
        if (aim.TriggerEnable) {
            ImGui::Checkbox("Require Key", &aim.TriggerByKey);
            ImGui::Checkbox("Only While Aiming", &aim.TriggerWhileADS);
            ImGui::Checkbox("Ignore Wounded", &aim.TriggerIgnoreKnocked);
            SliderI("Delay", &aim.TriggerDelay, 0, 250, "%d ms");
            SliderI("Max Distance", &aim.TriggerMaxDistance, 1, 500, "%d m");
            SliderF("FOV", &aim.TriggerFOV, 1.f, 30.f);
            ToggleColor("Draw FOV", &aim.TriggerDrawFOV, aim.TriggerFOVColor);
            if (aim.TriggerDrawFOV)
                SliderF("FOV Thickness", &aim.TriggerFOVThickness, 1.f, 5.f);
        }
    }
    EndPanel();

    NextColumn();
    BeginPanel("Weapon Mods");
    {
        ImGui::Checkbox("Recoil Control", &c.RecoilControl);
        if (c.RecoilControl)
            SliderI("Recoil Scale", &c.RecoilScale, 0, 100, "%d%%");
        ImGui::Checkbox("No Sway", &c.NoSway);
        ImGui::Checkbox("No Spread", &c.NoSpread);
        ImGui::Checkbox("Full Auto", &c.FullAuto);
        ImGui::Checkbox("Thick Bullet", &c.ThickBullet);
        ImGui::Checkbox("Super Bow", &c.SuperBow);
        ImGui::Checkbox("Instant Eoka", &c.InstantEoka);
        ImGui::Checkbox("Extended Melee", &c.ExtendedMelee);
        ImGui::Checkbox("Attack On Mountables", &c.AttackOnMountable);
    }
    EndPanel();

    BeginPanel("Movement");
    {
        ImGui::Checkbox("Spider-Man", &c.SpiderMan);
        ImGui::Checkbox("No Fall Damage", &c.NoFall);
        ImGui::Checkbox("Omni Sprint", &c.OmniSprint);
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
        ImGui::Checkbox("Team Check", &p.TeamCheck);
        ImGui::Checkbox("Hide Sleeping", &p.RemoveSleeping);
        ImGui::Checkbox("Hide Wounded", &p.RemoveWounded);
        RangeAndFade(p, 1000);
    }
    EndPanel();

    BoxPanel(p.Box);
    HeadCirclePanel(p.HeadCircle);

    NextColumn();
    SkeletonPanel(p.Skeleton);
    LinesPanel(p.Lines);

    BeginPanel("Prediction Lines");
    {
        auto& pl = p.PredictionLines;
        ImGui::Checkbox("Enable", &pl.Enable);
        if (pl.Enable) {
            SliderF("Prediction Time", &pl.PredictionTime, 0.5f, 5.f, "%.1f s");
            SliderF("Length", &pl.Length, 5.f, 200.f, "%.0f");
            SliderF("Thickness", &pl.Thickness, 1.f, 5.f);
            SliderF("Min Velocity", &pl.MinVelocity, 0.f, 20.f);
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
            SliderF("Radius", &prox.ProximityRadius, 5.f, 150.f, "%.0f m");
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

    BeginPanel("Hotbar");
    {
        auto& hb = p.HotBar;
        ImGui::Checkbox("Enable", &hb.Enable);
        if (hb.Enable) {
            ImGui::Checkbox("Show Clothing", &hb.Clothing);
            ComboRow("Font", &hb.FontStyle, fonts);
            SliderF("Text Size", &hb.TextSize, 10.f, 40.f, "%.0f px");
            SliderI("Position X", &hb.PosX, 0, 3000);
            SliderI("Position Y", &hb.PosY, 0, 3000);
        }
    }
    EndPanel();
    EndColumns();
}

void PageNPC(SETTINGS* s) {
    auto& n = s->Visuals.NPC;
    const auto& fonts = s->InstalledFonts;
    BeginColumns();
    BeginPanel("NPC ESP");
    {
        ImGui::Checkbox("Enable", &n.Enable);
        KeyRow("Toggle Key", &n.ToggleKey);
        ItemFilterRow("Show", n.List, n.Selected);
        RangeAndFade(n, 1000);
    }
    EndPanel();

    BoxPanel(n.Box);
    LinesPanel(n.Lines);

    NextColumn();
    BeginLabelPanel("Name", n.NickName, fonts);
    EndPanel();

    BeginLabelPanel("Distance", n.Distance, fonts);
    if (n.Distance.Enable)
        ImGui::Checkbox("Merge With Name", &n.Distance.MergeNickName);
    EndPanel();

    SkeletonPanel(n.Skeleton);
    HeadCirclePanel(n.HeadCircle);
    EndColumns();
}

void PageLoot(SETTINGS* s) {
    auto& it = s->Visuals.Items;
    BeginColumns();
    BeginPanel("Loot ESP");
    {
        ImGui::Checkbox("Enable", &it.Enable);
        KeyRow("Toggle Key", &it.ToggleKey);
        ComboRow("Font", &it.FontStyle, s->InstalledFonts);
        RangeAndFade(it, 500);
    }
    EndPanel();

    BeginPanel("Containers");
    {
        LootCategory("Crates", it.Crates);
        LootCategory("Barrels", it.Barrels);
        LootCategory("Corpses", it.Corpse);
    }
    EndPanel();

    NextColumn();
    BeginPanel("Resources");
    {
        ImGui::PushID(&it.WorldDrop);
        ToggleColor("Dropped Items", &it.WorldDrop.Enable, it.WorldDrop.Color);
        if (it.WorldDrop.Enable)
            IndexMultiRow("Categories", it.WorldDrop.Selected, kDropCategories, IM_ARRAYSIZE(kDropCategories));
        ImGui::PopID();
        LootCategory("Ores", it.Ores);
        LootCategory("Animals", it.Animals);
        LootCategory("Food", it.Food);
        LootCategory("Collectibles", it.Collectibles);
    }
    EndPanel();

    BeginPanel("Deployables");
    {
        LootCategory("Vehicles", it.Vehicles);
        LootCategory("Traps", it.Traps);
        LootCategory("Misc", it.Misc);
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
            ComboRow("Marker", &r.Type, kRadarMarkers, IM_ARRAYSIZE(kRadarMarkers));
            SliderI("Size", &r.Size, 100, 400, "%d px");
            SliderI("Zoom", &r.Zoom, 1, 50);
            SliderI("Max Distance", &r.MaxDistance, 1, 500, "%d m");
            SliderF("Position X", &s->radarPos.x, 0.f, 3000.f, "%.0f");
            SliderF("Position Y", &s->radarPos.y, 0.f, 3000.f, "%.0f");
            SubHeader("Markers");
            SliderF("Min Size", &r.MinSize, 1.f, r.MaxSize);
            SliderF("Max Size", &r.MaxSize, r.MinSize, 20.f);
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
            SliderI("FOV", &o.Fov, 1, 90);
            SliderI("Max Distance", &o.MaxDistance, 1, 500, "%d m");
            SliderF("Thickness", &o.Thickness, 1.f, 5.f);
            SliderF("Min Size", &o.MinSize, 1.f, o.MaxSize, "%.0f px");
            SliderF("Max Size", &o.MaxSize, o.MinSize, 40.f, "%.0f px");
            SliderF("Min Opacity", &o.MinOpacity, 0.f, o.MaxOpacity, "%.2f");
            SliderF("Max Opacity", &o.MaxOpacity, o.MinOpacity, 1.f, "%.2f");
            ColorMode(o);
        }
    }
    EndPanel();
    EndColumns();
}

void PageWorld(SETTINGS* s) {
    auto& c = s->Configs;
    BeginColumns();
    BeginPanel("Camera");
    {
        ImGui::Checkbox("FOV Changer", &c.FovChanger);
        if (c.FovChanger)
            SliderF("Field of View", &c.FovChangerValue, 30.f, 180.f, "%.0f");
        ImGui::Checkbox("Third Person", &c.ThirdPerson);
        ToggleKey("Zoom", &c.ZoomFov, &c.ZoomKey);
        if (c.ZoomFov)
            SliderF("Zoom FOV", &c.ZoomFovValue, 10.f, 120.f, "%.0f");
    }
    EndPanel();

    NextColumn();
    BeginPanel("Chams");
    {
        ImGui::Checkbox("Enable", &c.Chams);
        if (c.Chams) {
            ComboRow("Players", &c.PlayerChamsMaterial, kChamsMaterials, IM_ARRAYSIZE(kChamsMaterials));
            ComboRow("NPCs", &c.NpcChamsMaterial, kChamsMaterials, IM_ARRAYSIZE(kChamsMaterials));
        }
    }
    EndPanel();

    BeginPanel("Environment");
    {
        ImGui::Checkbox("Bright Night", &c.BrightNight);
        ImGui::Checkbox("Change Time", &c.ChangeTime);
        if (c.ChangeTime)
            SliderF("Time Of Day", &c.time, 0.f, 24.f, "%.1f h");
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
    }
    EndPanel();

    NextColumn();
    BeginPanel("Spectator List");
    {
        SliderF("Position X", &s->specPos.x, 0.f, 3000.f, "%.0f");
        SliderF("Position Y", &s->specPos.y, 0.f, 3000.f, "%.0f");
    }
    EndPanel();

    FontsPanel(s, [s](int font) {
        auto& p = s->Visuals.Players;
        p.NickName.FontStyle = font;
        p.Weapon.FontStyle = font;
        p.Distance.FontStyle = font;
        p.Status.FontStyle = font;
        p.Velocity.FontStyle = font;
        p.HotBar.FontStyle = font;
        s->Visuals.NPC.NickName.FontStyle = font;
        s->Visuals.NPC.Distance.FontStyle = font;
        s->Visuals.Items.FontStyle = font;
    });
    EndColumns();
}

} // namespace

void RenderMyMenu(bool* ShowMenu, SETTINGS* Settings, CConfig* g_pConfig) {
    MENU_LOG_FRAME(ShowMenu, Settings, g_pConfig);
    if (!ShowMenu || !Settings)
        return;

    const Chip chips[] = {
        { "AIM",   &Settings->Aimbot.Enable },
        { "ESP",   &Settings->Visuals.Players.Enable },
        { "LOOT",  &Settings->Visuals.Items.Enable },
        { "RADAR", &Settings->Visuals.Radar.Enable },
    };

    ShellDesc desc;
    desc.brand = "RUST";
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
    case P_Labels:   PageLabels(Settings); break;
    case P_NPC:      PageNPC(Settings); break;
    case P_Loot:     PageLoot(Settings); break;
    case P_Radar:    PageRadar(Settings); break;
    case P_World:    PageWorld(Settings); break;
    case P_Settings: PageSettings(Settings, ShowMenu); break;
    case P_Configs:  ConfigsPage(Settings, g_pConfig, cfgname, (int)sizeof cfgname); break;
    }
    EndShell();
}
#endif
