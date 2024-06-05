#pragma once
#include <direct.h>
#include <stdio.h>
#include <Windows.h>
#include <psapi.h>
#include <intrin.h>
#include <vector>
#include "../Config.h"

#if IsXDefiant

struct SETTINGS {
    int iConfig = 0;
    int oldiConfig = 0;
    int default_display = 0;
    int display_amount = 0;

    struct AimStruct {
        bool Enable = true;
        bool MemoryAimbot = true;
        bool TriggerBot = true;
        int TriggerBotDelay = 50;
        bool SaveTarget = true;
        bool AimLock = false;
        bool VisibilityCheck = false;
        bool HumanizedSmooth = false;
        bool IgnoreDowned = true;

        bool DrawFov = false;
        bool DrawCrossHair = false;
        bool DrawTarget = false;
        int DrawTargetType = 1;
        float DrawThickness = 1.f;

        int HitBox = 0;

        float FOV = 5.f;
        float Smooth = 2.f;
        float MemorySmooth = 0.1f;
        float HumanizedSmoothPercent = 2.f;
        int MaxDistance = 600;
    }Aim;

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

            bool Skeleton = false;
            float SkeletonThickness = 1.f;

            bool Health = true;
            int HealthType = 1;
            float HealthThickness = 4.f;

            bool OutScreen = true;
            int OutScreenType = 0;
            int OutScreenFov = 15;
            float OutScreenThickness = 2.f;
            int OutScreenSize = 10.f;
        }Players;

        struct {
            bool Enable = true;
            int PositionX = 40;
            int PositionY = 30;
            int Size = 200;
            int Zoom = 1;

            bool VisibleColor = true;
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
            int ToggleRadar = VK_F8;
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
        }Players;

        struct {
            float Enemies[3] = { 1.f, 0.f, 0.f };
            float Visible[3] = { 0.f, 1.0f, 0.f };
        }Radar;
    }Colors;

    struct {
        int Language = 0;
        bool FeaturesDefinition = true;
        int FontStyle = 0;
        bool VisibilityByColor = true;


        float Color[3] = { 255 / 255.f, 102 / 255.f, 102 / 255.f };
        int offset_red = 60;
        int offset_blue = 60;
        int offset_green = 60;
    }Configs;
};

#endif