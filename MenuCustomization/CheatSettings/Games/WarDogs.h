#pragma once
#include <direct.h>
#include <stdio.h>
#include <Windows.h>
#include <psapi.h>
#include <intrin.h>
#include <vector>
#include <string>
#include "../Config.h"
#include "imconfig.h"

#if IsWarDogs

// Item categories: the index of WorldEsp.Categories.Style[]. WDC_Other = anything the cheat could not classify:
// it is never drawn, so do not put it in the menu.
enum WDCategory : int {
    WDC_Other = 0,
    // Weapons
    WDC_AssaultRifle, WDC_SMG, WDC_Shotgun, WDC_LMG, WDC_MarksmanRifle, WDC_SniperRifle, WDC_Bow, WDC_Sidearm, WDC_Launcher,
    // Equipment
    WDC_Tool, WDC_Medicine, WDC_Spotter, WDC_Explosive, WDC_Smoke,
    // Vehicle types: NOT used by the item tables (a vehicle is configured in Vehicles.Categories[])
    WDC_LandTransport, WDC_Tank, WDC_Artillery, WDC_AirCombat, WDC_AirTransport, WDC_Stationary,
    // More equipment
    WDC_Building, WDC_Misc, WDC_VehicleSupply,
    // Gear
    WDC_Helmet, WDC_Armor, WDC_Vest, WDC_Backpack, WDC_Traversal,
    // Attachments
    WDC_Magazine, WDC_Optic, WDC_Muzzle, WDC_Barrel, WDC_Stock, WDC_Grip, WDC_Handguard,
    // Ammunition
    WDC_Ammo,
    WDC_Count // = 37
};
constexpr int kWDCategoryCount = 37;
static_assert(WDC_Count == kWDCategoryCount, "WDCategory and kWDCategoryCount must match");

inline const char* const kWDCategoryNames[kWDCategoryCount] = {
    "Other",
    "Assault Rifles", "SMGs", "Shotguns", "LMGs", "Marksman Rifles", "Sniper Rifles", "Bows", "Sidearms", "Launchers",
    "Tool", "Medicine", "Spotter", "Explosive", "Smoke",
    "Land Transport", "Tanks", "Artillery", "Air Combat", "Air Transport", "Stationary",
    "Building", "Misc", "Vehicle Supply",
    "Helmets", "Armor", "Vests", "Backpacks", "Traversal",
    "Magazines", "Optics", "Muzzles", "Barrels", "Stocks", "Grips", "Handguards",
    "Ammo"
};

constexpr int kWDWeaponCategoryCount = 9;
inline const char* const kWDWeaponCategoryNames[kWDWeaponCategoryCount] = {
    "Assault Rifles", "SMGs", "Shotguns", "LMGs", "Marksman Rifles",
    "Sniper Rifles", "Bows", "Sidearms", "Launchers"
};

// Groups that share one max distance (WorldEsp.GroupDistance[]). Ammo has no group: it is a toggle + color in the
// World ESP list and uses WorldEsp.MaxDistance (like containers, bodies and deployables).
enum WDItemGroup : int { WDG_Weapons = 0, WDG_Equipment, WDG_Gear, WDG_Attachments, WDG_Count };
constexpr int kWDItemGroupCount = 4;
inline const char* const kWDItemGroupNames[kWDItemGroupCount] = { "Weapons", "Equipment", "Gear", "Attachments" };

// The categories of each group, in the order the cheat's own menu shows them.
inline const WDCategory kWDGroupWeapons[] = { WDC_AssaultRifle, WDC_SMG, WDC_Shotgun, WDC_LMG, WDC_MarksmanRifle, WDC_SniperRifle, WDC_Bow, WDC_Sidearm, WDC_Launcher };
inline const WDCategory kWDGroupEquipment[] = { WDC_Tool, WDC_Medicine, WDC_Spotter, WDC_Explosive, WDC_Smoke, WDC_Building, WDC_VehicleSupply, WDC_Misc };
inline const WDCategory kWDGroupGear[] = { WDC_Helmet, WDC_Armor, WDC_Vest, WDC_Backpack, WDC_Traversal };
inline const WDCategory kWDGroupAttachments[] = { WDC_Magazine, WDC_Optic, WDC_Muzzle, WDC_Barrel, WDC_Stock, WDC_Grip, WDC_Handguard };

// Vehicle types: the index of Vehicles.Categories[].
enum WDVehicleGroup : int { WDV_LandTransport = 0, WDV_Tank, WDV_Artillery, WDV_AirCombat, WDV_AirTransport, WDV_Stationary, WDV_Other, WDV_Count };
constexpr int kWDVehicleGroupCount = 7;
inline const char* const kWDVehicleGroupNames[kWDVehicleGroupCount] = {
    "Land Transport", "Tanks", "Artillery", "Air Combat", "Air Transport", "Stationary", "Other Vehicles"
};

// One toggle + color of an item category.
struct EntityStyle {
    bool Enable;
    float Color[4];
};

struct EntityStyles {
    EntityStyle Style[kWDCategoryCount];
};

// One toggle + friendly / enemy / unknown color of a vehicle type.
struct VehicleStyle {
    bool Enable;
    float FriendlyColor[4];
    float EnemyColor[4];
    float UnknownColor[4];
};

struct SETTINGS {
    int iConfig = 0;
    int oldiConfig = 0;
    int selectedWeapon = 0;
    std::vector<std::string> InstalledFonts = { "FortFont", "VerdanaFont" };
    std::vector<std::string> AvailableFonts;
    struct AimStruct{
        bool ByWeapon = false;
        bool Enable = true;
        bool MemoryAimbot = false;
        bool VisibilityCheck = true;

        bool DeadZone = false;
        float DeadZoneRadius = 5.0f; //1-20

        int HoldPrimaryKey = VK_LBUTTON;
        int HoldSecondaryKey = VK_RBUTTON;
        int SwitchToHeadKey = VK_SHIFT;
        int ToggleKey = VK_F6;

        bool SaveTarget = true;
        bool AimLock = false;
        bool Predict = true;
        int LockFov = 5;
        float TargetSwitchDelay = 0.5f; //0-5, seconds

        bool DrawFov = true;
        bool FovFilled = false;
        float FovFillColor[4] = { 0.f, 0.f, 0.f, 0.1f };
        float FovColor[4] = { 0.f, 0.f, 0.f, 0.1f };
        bool FovChangeTargetingColor = true;
        float FovTargetingColor[4] = { 1.f, 0.f, 0.f, 1.f };
        float FovThickness = 1.f;//1-5

        bool DrawCrossHair = true;
        int CrossHairType = 0; //0 = cross
        float CrossHairColor[4] = { 1.f, 1.f, 1.f, 0.6f };
        float CrossHairThickness = 1.f;//1-5

        bool DrawTarget = true;
        int TargetType = 1; //0 = circle, 1 = triangle
        float TargetThickness = 1.f;//1-5
        float TargetSize = 3.f;
        bool TargetFilled = true;
        bool TargetLine = false;
        int TargetFillOpacity = 30;
        float TargetCollor[4] = { 1.f, 0.f, 0.f, 0.8f };

        int HitBox = 0; //0 = head, 1 = chest, 2 = body, 3 = random
        //{
            float MinFOV = 5.f; //1-180 (dynamic <= MaxFOV)
            float MaxFOV = 180.f; //1-180 (dynamic >= MinFOV)
            bool CustomFov = false; //false = static, true = increase by coefficient
            float DistanceCoefficient = 0; //0-3
            float TimeCoefficient = 0.3f; //0-3
        //}
        float Smooth = 2.f;//1-10

        int MaxDistance = 600;

        // Heal Bot
        bool HealBotEnable = false;
        bool HealBotPrioritiseLowest = false; // prioritise ally with lowest HP
        int HealBotKey = VK_XBUTTON2;         // hold to target allies instead of enemies

    }Aim;
    AimStruct aimbot[kWDWeaponCategoryCount];

    struct VisualsStr{
        struct PlayerStr {
            bool Enable = true;
            int ToggleKey = VK_F6;

            int MaxDistance = 800;
            float MinFontSize = 10.f;
            float MaxFontSize = 16.f;

            bool DistanceTransparency = true;

            float MinTransparency = 0.6f;
            float MaxTransparency = 1.0f;

            struct {
                bool Enable = true;
                float Size = 5.f;
                float Thickness = 2.f;
                bool Filled = false;
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                float StaticColor[4] = { 1.f, 0.f, 0.f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.7f, 0.7f, 0.7f, 0.3f };
                int TeamColorAlpha = 255;
            }HeadCircle;

            struct {
                bool Enable = true;
                bool MergeNickName = true;
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                int position = 1; //0 = top, 1 = bottom, 2 = left, 3 = right
                float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255;
                int FontStyle = 0;
            }Distance;

            struct {
                bool Enable = true;
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                int position = 1; //0 = top, 1 = bottom, 2 = left, 3 = right
                float StaticColor[4] = { 0.8f, 0.8f, 0.9f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255;
                int FontStyle = 0;
            }NickName;

            struct {
                bool Enable = true;
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                int position = 1; //0 = top, 1 = bottom, 2 = left, 3 = right
                float StaticColor[4] = { 0.8f, 0.8f, 0.9f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255;
                int FontStyle = 0;
            }Weapon;

            struct {
                bool Enable = true;
                bool Filled = true;
                int Type = 1; //0 = square, 1 = corner
                int rounding = 10;
                float Thickness = 2.f;
                float Width = 0.45f;
                int ColorType = 1; //0 = static, 1 = by team, 2 = by visible
                float FillColor[4] = { 0.f, 0.f, 0.f, .2f };
                float StaticColor[4] = { 0.8f, 0.8f, 0.9f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255;
            }Box;

            struct {
                bool Enable = false;
                int Position = 0; //0 = top, 1 = bottom
                float Thickness = 1.f;
                int ColorType = 1; //0 = static, 1 = by team, 2 = by visible
                float StaticColor[4] = { 0.6f, 0.4f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255;
            }Lines;

            struct {
                bool Enable = true;
                float Thickness = 1.75f;
                int ColorType = 2; //0 = static, 1 = by team, 2 = by visible
                float StaticColor[4] = { 0.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255;
            }Skeleton;

            struct {
                bool Enable = true;
                bool HealthGradient = true;
                int position = 2; //0 = top, 1 = bottom, 2 = left, 3 = right
                float Thickness = 4.f;
                int ColorAlpha = 205;
            }Health;

            struct {
                bool Enable = true;
                float ProximityRadius = 50.0f;

                bool PulsatingBorder = true;
                bool ProximityText = true;

                float BorderThickness = 4.0f;
                float TextSize = 20.0f;

                float AlertColor[4] = { 1.f, 0.1f, 0.1f, 0.9f };
                float TextColor[4] = { 1.f, 1.f, 0.f, 1.f };

            }ProximityAlerts;

        };
        PlayerStr Players;

        struct {
            bool Enable = true;
            int PositionX = 10; //0-OverlayWidth
            int PositionY = 30; //0-OverlayHeight
            int ToggleKey = VK_F7;
            int MaxDistance = 150;

            int Type = 1;
            int Zoom = 1;
            int Size = 200;

            float MinSize = 1.f;
            float MaxSize = 4.f;
            float MinOpacity = 0.2f;
            float MaxOpacity = 1.0f;
            int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
            float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
            float VisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
            float NonVisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
            int TeamColorAlpha = 255;
        }Radar;

        struct {
            bool Enable = true;
            int ToggleKey = VK_F7;
            int MaxDistance = 150;

            int Type = 0;
            int Fov = 15;
            float Thickness = 2.f;
            float MinOpacity = 0.2f;
            float MaxOpacity = 1.0f;
            float MinSize = 10.f;
            float MaxSize = 30.f;
            int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
            float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
            float VisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
            float NonVisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
            int TeamColorAlpha = 255;
        }OutScreen;

        struct {
            bool Enable = false;
            int ToggleKey = VK_F8;
            int MaxDistance = 150;
            float MinFontSize = 10.f;
            float MaxFontSize = 16.f;
            int FontStyle = 0;

            bool ItemsEnable = true;     // dropped items (AWDDroppedItem) and free-standing ABHItem
            bool LootablesEnable = true; // world spawn points (AWDLootable)
            bool ContainersEnable = true;      // crates (AWDContainer)
            bool BodiesEnable = true;          // dead players' inventories (AWDPlayerInventoryContainer)
            bool DeployablesEnable = true;     // placed objects (AWDPlaceable)

            // Max distance of each item group (WDItemGroup); MaxDistance above is for ammo, containers, bodies, deployables.
            int GroupDistance[kWDItemGroupCount] = { 150, 150, 150, 150 };
            // Items, lootables and vehicle supplies: one toggle and color per WDCategory (Categories.Style[WDCategory]).
            EntityStyles Categories;
            // Kinds that have no item category get one color each.
            float ColorContainer[4]     = { 1.f, 0.85f, 0.3f, 1.f };
            float ColorBody[4]          = { 0.9f, 0.4f, 0.9f, 1.f };
            float ColorDeployable[4]    = { 0.8f, 0.6f, 0.4f, 1.f };
        }WorldEsp;
        struct {
            bool ShowVehicles = true;
            bool ShowFriendlyVehicles = true;
            bool ShowEnemyVehicles = true;
            bool ShowOccupants = true;
            bool ShowHealth = true;
            bool HealthBarOnly = false; // only the hull bar, no per-part health lines
            int MaxDistance = 2000;
            VehicleStyle Categories[kWDVehicleGroupCount]; // by WDVehicleGroup
        }Vehicles;
    }Visuals;

    struct {
        int Language = 0;
        bool FeaturesDefinition = true;
        int GameFPS = 60;
        int RenderFPS = 144;
        int MenuKey = VK_INSERT;
        bool TeamCheck = true;
        bool VSync = false;

        // Exploits (weapon / character attribute patching, sdk/WeaponExploits.cpp). *Percent = strength.
        bool NoRecoil = false;
        int NoRecoilPercent = 100;
        bool InstantADS = false;
        int InstantADSPercent = 100;
        bool InfiniteFocus = false;
        int InfiniteFocusPercent = 100;
        bool NoSway = false;
        int NoSwayPercent = 100;
        bool InstantLean = false;
        int InstantLeanPercent = 100;
        bool InfiniteStamina = false;
        int InfiniteStaminaPercent = 100;
        bool NoWeight = false;
        int NoWeightPercent = 50; //1-50
        bool NoHeavyBreathing = false;
        int NoHeavyBreathingPercent = 100;
        bool SpeedHack = false;
        float SpeedHackMultiplier = 1.05f; //1.0-1.1 (10%)
        int SpeedHackFrictionPercent = 100;
    }Configs;
};
#endif
