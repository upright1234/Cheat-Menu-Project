#pragma once
#include <Windows.h>
#include "../Config.h"

#if IsFortnite

struct SETTINGS {
    int iConfig = 0;
    int oldiConfig = 0;
    int selectedWeapon = 0;
    struct AimStruct {
        bool Enable = true;
        bool SaveTarget = true;
        bool AimLock = false;
        bool VisibilityCheck = false;
        bool HumanizedSmooth = false;
        bool WeaponOnly = true;
        bool IgnoreDowned = true;
        bool Predict = true;
        bool NPC = false;
        bool ByWeapon = false;

        bool DrawFov = false;
        bool DrawCrossHair = false;
        bool DrawTarget = false;
        int DrawTargetType = 1;
        float DrawThickness = 1.f;

        int HitBox = 0;

        float FOV = 5.f;
        float Smooth = 2.f;
        float HumanizedSmoothPercent = 2.f;
        int MaxDistance = 600;
    }Aim;

    AimStruct aimbot[11];

    struct {
        bool TriggerBot = true;
        int TriggerBotMaxDistance = 50;
        int TriggerBotDelay = 150;

        bool AimWhileJumping = false;
        bool InstantSwitch = false;
    }Misc;

    struct {
        struct {
            bool Enable = true;

            int MaxDistance = 800;
            float FontSize = 16.f;

            bool Box = true;
            bool RemoveDowned = false;
            bool HeadCircle = true;

            int BoxType = 0;
            float BoxThickness = 1.f;

            bool Lines = true;
            int LinesPosition = 0;
            float LinesThickness = 1.f;

            bool Distance = true;
            bool NickName = true;

            bool Weapon = true;
            bool WeaponRarity = true;
            bool WeaponAmmo = true;

            bool Skeleton = false;
            float SkeletonThickness = 1.f;

            bool OutScreen = false;
            int OutScreenType = 0;
            int OutScreenFov = 15;
            float OutScreenThickness = 2.f;
            int OutScreenSize = 10.f;

        }Players;

        struct {
            bool Enable = false;

            int MaxDistance = 800;
            float FontSize = 16.f;

            bool Chest = true;
            bool Vehicle = true;
            bool Ammo = true;
            bool Weapon = true;
            bool Consumable = true;
            bool Material = true;
            bool SupplyDrop = true;

            int Rarity = 0;
        }World;

        struct {
            bool Enable = true;
            int PositionX = 10;
            int PositionY = 30;
            int Zoom = 10;
            int Size = 200;

            bool VisibleColor = true;
            bool ClosestColor = true;
            int ClosestDistance = 100;
            int MaxDistance = 150;
        }Radar;
    }Visuals;

    struct {

        struct {
            int HoldPrimary = VK_LBUTTON;
            int HoldSecondary = VK_RBUTTON;
            int SwitchToHead = VK_SHIFT;
            int Toggle = VK_F6;
        }Aim;

        struct {
            int TogglePlayers = VK_F7;
            int ToggleWorld = VK_F7;
            int ToggleRadar = VK_F9;
        }Visuals;

        int Menu = VK_INSERT;
        int Trigger = VK_XBUTTON1;

    }Keys;

    struct {
        struct {
            float Fov[3] = { 0.f, 1.f, 0.f };
            float CrossHair[3] = { 1.f, 0.f, 0.f };
            float Target[3] = { 1.f, 1.f, 0.f };
        }Aim;

        struct {
            float Visible[3] = { 1.f, 1.f, 1.f };
            float NonVisible[3] = { 1.f, 0.f, 0.f };
            float Distance[3] = { 1.f, 1.f, 1.f };
            float NickName[3] = { 1.f, 1.f, 1.f };
            float Weapon[3] = { 1.f, 1.f, 1.f };
        }Players;

        struct {
            float Enemies[3] = { 1.f, 0.f, 0.f };
            float Visible[3] = { 0.f, 1.0f, 0.f };
            float Closest[3] = { 1.f, 1.0f, 0.f };
        }Radar;

    }Colors;

    struct {
        int Language = 0;
        bool FeaturesDefinition = true;
        int FontStyle = 0;
    }Configs;

    struct TriggerStruct {
        bool ByWeapon = false;

        bool Enable = true;
        int MaxDistance = 50;
        int Delay = 150;
    }Trigger;
    TriggerStruct triggerbot[11];
};


#endif