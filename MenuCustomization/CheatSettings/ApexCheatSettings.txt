#pragma once
#include <Windows.h>

struct SETTINGS {

    int iConfig = 0;
    int oldiConfig = 0;

    struct {

        bool Enable = true;
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

        //bool SpectatorCount = true;
        //bool RingHelper = true;
        //bool Thirdperson = true;
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
            int GlowType = 0;

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
            int PositionY = 30;
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
    }Configs;
};