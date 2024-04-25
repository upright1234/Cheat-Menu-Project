#pragma once
#include <direct.h>
#include <stdio.h>
#include <Windows.h>
#include <psapi.h>
#include <intrin.h>
#include <vector>
#include "../Config.h"

#if IsValorant

struct SETTINGS {

    int iConfig = 0;
    int oldiConfig = 0;

    struct {

        bool Enable = true;
        bool SaveTarget = true;
        bool AimLock = false;
        bool VisibilityCheck = true;
        bool HumanizedSmooth = true;

        bool DrawFov = true;
        bool DrawCrossHair = false;
        bool DrawTarget = true;
        int DrawTargetType = 0;
        float DrawThickness = 1.f;

        bool RecoilControl = true;
        int Recoil = 30;
        int delay = 0;

        int HitBox = 3;

        int FOV = 45;
        int Smooth = 1;
        int HumanizedSmoothPercent = 1;
        int MaxDistance = 600;

    }Aim;

    struct {
        struct {
            bool Enable = true;

            int MaxDistance = 800;
            float FontSize = 16.f;

            bool TeamCheck = true;
            bool RemoveDormant = false;
            bool HeadCircle = true;

            bool Box = true;
            int BoxType = 0;
            float BoxThickness = 1.f;

            bool Health = true;
            int HealthType = 0;
            float HealthThickness = 4.f;

            bool Lines = true;
            int LinesPosition = 0;
            float LinesThickness = 1.f;

            bool Distance = true;

            bool Skeleton = true;
            float SkeletonThickness = 1.f;

            bool OutScreen = false;
            int OutScreenType = 0;
            int OutScreenFov = 15;
            float OutScreenThickness = 2.f;
            int OutScreenSize = 10.f;

        }Players;

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
            int ToggleRadar = VK_F9;
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
            float NonVisible[3] = { 1.f, 0.f, 0.f };
            float Distance[3] = { 1.f, 1.f, 1.f };
            float NickName[3] = { 1.f, 1.f, 1.f };
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
};

#endif