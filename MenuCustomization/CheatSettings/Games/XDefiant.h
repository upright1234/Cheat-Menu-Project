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
    int selectedWeapon = 0;
    struct AimStruct {
        bool Enable = true;
        bool SaveTarget = true;
        bool AimLock = false;
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
        }Players;
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
        }Visuals;

        int Menu = VK_INSERT;

    }Keys;

    struct {
        struct {
            float Fov[3] = { 0.f, 1.f, 0.f };
            float CrossHair[3] = { 1.f, 0.f, 0.f };
            float Target[3] = { 1.f, 1.f, 0.f };
        }Aim;

        struct {
            float Visible[3] = { 1.f, 1.f, 1.f };
            float Distance[3] = { 1.f, 1.f, 1.f };
            float NickName[3] = { 1.f, 1.f, 1.f };
        }Players;
    }Colors;

    struct {
        int Language = 0;
        bool FeaturesDefinition = true;
        int FontStyle = 0;
    }Configs;
};

#endif