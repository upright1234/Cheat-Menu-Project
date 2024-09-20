#pragma once
#include <Windows.h>
#include "../Config.h"

#if IsApex

struct SETTINGS {

    int iConfig = 0;
    int oldiConfig = 0;

    struct {
        int Enable = 1;
        bool SaveTarget = true;
        bool RecoilControl = true;
        bool AimLock = true;
        bool VisibilityCheck = true;
        bool HumanizedSmooth = true;
        bool IgnoreKnocked = true;

        bool DrawFov = true;
        bool DrawCrossHair = true;
        bool DrawTarget = true;
        int DrawTargetType = 0;
        float DrawThickness = 1.f;

        bool Predict = true;
        int HitBox = 0;

        int RecoilControlValue = 100;
        float FOV = 5.f;
        float Smooth = 2.f;
        float HumanizedSmoothPercent = 2.f;
        int MaxDistance = 600;

    }Aimbot;

    struct {

        bool Enable = true;
        bool WhileADS = true;
        bool IgnoreKnocked = true;
        bool ByKey = true;
        int MaxDistance = 600;
        int Delay = 50;

    }TriggerBot;

    struct {
        bool FakeDuck = false;
        bool Bhop = false;
        bool SuperGrappler = false;
        bool FreeCam = false;
        bool SuperGlide = false;
        int SuperGlideType = 0;
        int SuperGlideMode = 0;
        bool FastMovement = false;
        bool SkinChanger = false;
        int SkinID = 1;
        bool SpectatorCount = true;
        bool SpectatorList = true;
        bool WallJump = true;
        bool RapidFire = true;
        int RapidFireDelay = 75;

    }Misc;

    struct {
        struct {
            bool Enable = false;
            bool Text = false;

            bool Glow = true;

            int MaxDistance = 150;
            float FontSize = 12.f;

            bool Weapon = true;
            bool Deathbox = true;
            bool Vehicle = true;
            bool Ammo = true;
            bool Attachment = true;
            bool Grenades = true;
            bool Medicine = true;

            bool Helmet = true;
            bool Body = true;
            bool Evo = true;
            bool Knockdown = true;
            int KnockdownType = 3;
            bool Backpack = true;
            int BackpackType = 3;
        }Items;

        struct {
            bool Enable = true;
            bool IgnoreKnocked = true;

            int MaxDistance = 800;
            float FontSize = 16.f;

            bool Glow = true;
            int GlowRadius = 64;
            int BodyStyle = 15;
            int OutlineStyle = 1;

            bool HeadCircle = true;
            bool OutScreen = true;
            int OutScreenType = 1;
            int OutScreenFov = 15;
            float OutScreenThickness = 2.f;
            int OutScreenSize = 10.f;

            bool NickName = true;
            bool Distance = true;
            bool Weapon = true;

            bool Box = true;
            int BoxType = 0;
            float BoxThickness = 1.f;

            bool Lines = true;
            int LinesPosition = 0;
            float LinesThickness = 1.f;

            bool Health = true;
            int HealthType = 1;
            float HealthThickness = 4.f;

            bool Shield = true;
            int ShieldType = 1;
            float ShieldThickness = 4.f;

            bool Skeleton = false;
            float SkeletonThickness = 1.f;
        }Players;

        struct {
            bool Enable = true;
            int MaxDistance = 800;
            int PositionX = 10;
            int PositionY = 40;
            int Zoom = 10;
            int Size = 200;

            bool VisibleColor = true;
            bool ClosestColor = true;
            int ClosestDistance = 100;
        }Radar;
    }Visuals;

    struct {

        struct {
            int HoldPrimary = VK_LBUTTON;
            int HoldSecondary = VK_RBUTTON;
            int SwitchToHead = VK_SHIFT;
            int Toggle = VK_F6;
        }Aimbot;

        struct {
            int TogglePlayers = VK_F7;
            int ToggleItems = VK_F8;
            int ToggleRadar = VK_F9;
        }Visuals;

        int Menu = VK_INSERT;
        int Bhop = VK_SPACE;
        int FakeDuck = 0x5A;
        int FreeCam = 0x58;
        int SuperGlide = 0x57;
        int SuperGrappler = 0x51;
        int Trigger = VK_XBUTTON1;
        int RapidFire = VK_XBUTTON2;
        int WallJump = 0x58;

    }Keys;

    struct {
        struct {
            float Fov[3] = { 0.f, 1.f, 0.f };
            float CrossHair[3] = { 1.f, 0.f, 0.f };
            float Target[3] = { 1.f, 1.f, 0.f };
        }Aimbot;

        struct {
            float Visible[3] = { 1.f, 1.f, 1.f };
            float NonVisible[3] = { 1.f, 1.f, 1.f };
            float GlowVisible[3] = { 0.62f, 0.05f, 0.05f };
            float GlowNonVisible[3] = { 0.62f, 0.05f, 0.05f };
        }Players;

        struct {
            float Enemies[3] = { 1.f, 0.f, 0.f };
            float Visible[3] = { 0.f, 1.0f, 0.f };
            float Closest[3] = { 1.f, 1.0f, 0.f };
        }Radar;

        struct {
            float Deathbox[3] = { 1.f, 0.f, 0.f };
            float Vehicle[3] = { 1.f, 0.f, 0.f };
            float Weapon[3] = { 1.f, 0.f, 0.f };
        }Items;

    }Colors;

    struct {
        int Language = 0;
        bool FeaturesDefinition = true;
        int FontStyle = 0;
    }Configs;
};

#endif