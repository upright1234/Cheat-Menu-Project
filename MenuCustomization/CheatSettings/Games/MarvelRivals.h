#pragma once
#include <direct.h>
#include <stdio.h>
#include <Windows.h>
#include <psapi.h>
#include <intrin.h>
#include <vector>
#include "../Config.h"
#include "imconfig.h"

#if IsMarvel

struct SETTINGS {
    int iConfig = 0;
    int oldiConfig = 0;
    std::vector<std::string> InstalledFonts = { "FortFont", "VerdanaFont" };
    std::vector<std::string> AvailableFonts;
    struct AimStruct {
        bool Enable = true;
        bool DeadZone = false;
        float DeadZoneRadius = 5.0f; //1-20

        int HoldPrimaryKey = VK_LBUTTON;
        int HoldSecondaryKey = VK_RBUTTON;
        int SwitchToHeadKey = VK_SHIFT;
        int ToggleKey = VK_F6;

        bool SaveTarget = true;
        bool Predict = true;

        bool AimLock = false;
        float TargetSwitchDelay = 0.5f; //0-5, seconds

        bool DrawFov = true;
        float FovColor[4] = { 0.f, 0.f, 0.f, 0.1f };
        float FovThickness = 1.f;//1-5

        bool DrawCrossHair = true;
        float CrossHairColor[4] = { 1.f, 1.f, 1.f, 0.6f };
        float CrossHairThickness = 1.f;//1-5

        bool DrawTarget = true;
        int TargetType = 1; //0 = circle, 1 = triangle
        float TargetThickness = 1.f;//1-5
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
        int MaxDistance = 600; //1-1000
    }Aim;

    struct {
        struct {
            bool Enable = true;
            int ToggleKey = VK_F6;

            int MaxDistance = 800; //0-800
            float MinFontSize = 10.f; //10-30
            float MaxFontSize = 16.f; //10-30

            bool DistanceTransparency = true;

            float MinTransparency = 0.3f; //0.0-1.0
            float MaxTransparency = 1.0f; //0.0-1.0

            struct {
                bool Enable = true;
                float Size = 5.f; //1-10
                float Thickness = 2.f; //1-5
                bool Filled = false;
                float StaticColor[4] = { 1.f, 0.f, 0.f, 1.f };
            }HeadCircle;

            struct {
                bool Enable = true;
                int position = 1; //0 = top, 1 = bottom, 2 = left, 3 = right
                float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
                int FontStyle = 0; //0-(InstalledFonts.size()-1)
            }Distance;

            struct {
                bool Enable = true;
                int position = 2; //0 = top, 1 = bottom, 2 = left, 3 = right
                float StaticColor[4] = { 0.8f, 0.8f, 0.9f, 1.f };
                int FontStyle = 0; //0-(InstalledFonts.size()-1)
            }HeroName;

            struct {
                bool Enable = true;
                bool Filled = true;
                int Type = 1; //0 = square, 1 = corner
                int rounding = 10; //1-20
                float Thickness = 2.f; //1-5
                float Width = 0.45f; //0.1-1.0
                float FillColor[4] = { 0.f, 0.f, 0.f, .2f };
                float StaticColor[4] = { 0.8f, 0.8f, 0.9f, 1.f };
            }Box;

            struct {
                bool Enable = false;
                int Position = 0; //0 = top, 1 = bottom
                float Thickness = 1.f; //1-5
                float StaticColor[4] = { 0.6f, 0.4f, 1.f, 1.f };
            }Lines;

            struct {
                bool Enable = true;
                float Thickness = 1.75f; //1-5
                float StaticColor[4] = { 0.f, 1.f, 1.f, 1.f };
            }Skeleton;

            struct {
                bool Enable = false;
                bool Gradient = true;
                int position = 2; //0 = top, 1 = bottom, 2 = left, 3 = right
                float Thickness = 4.f; //1-10
                int ColorAlpha = 205; //0-255
            }Health;

            struct {
                bool Enable = false;
                bool Gradient = true;
                int position = 2; //0 = top, 1 = bottom, 2 = left, 3 = right
                float Thickness = 4.f; //1-10
                int ColorAlpha = 205; //0-255
            }Shield;

            struct {
                bool Enable = true;
                float ProximityRadius = 50.0f; //10-200

                bool PulsatingBorder = true;
                bool ProximityText = true;

                float BorderThickness = 4.0f; //1-20
                float TextSize = 20.0f; //10-50

                float AlertColor[4] = { 1.f, 0.1f, 0.1f, 0.9f };
                float TextColor[4] = { 1.f, 1.f, 0.f, 1.f };

            }ProximityAlerts;


        }Players;

        struct {
            bool Enable = true;
            int PositionX = 10; //0-OverlayWidth
            int PositionY = 30; //0-OverlayHeight
            int ToggleKey = VK_F7;
            int MaxDistance = 150;//0-800

            int Zoom = 10;//1-100
            int Size = 200;//50-500

            float MinSize = 1.f;//1-6
            float MaxSize = 4.f;//1-6
            float MinOpacity = 0.2f;//0-1
            float MaxOpacity = 1.0f;//0-1
            float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
        }Radar;

        struct {
            bool Enable = true;
            int ToggleKey = VK_F7;
            int MaxDistance = 150; //0-800

            int Type = 0; //0 = circle, 1 = triangle
            int Fov = 15; //1-180
            float Thickness = 2.f; //1-5
            float MinOpacity = 0.2f; //0.0-1.0
            float MaxOpacity = 1.0f; //0.0-1.0
            float MinSize = 10.f; //1.0-MaxSize (UI), up to 30.0
            float MaxSize = 30.f; //MinSize-30.0 (UI)
            float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
        }OutScreen;

    }Visuals;
    struct {
        int Language = 0; //0 = English, 1 = Portuguese
        bool FeaturesDefinition = true;
        int GameFPS = 60; //30-240
        int RenderFPS = 144; //30-360
        int MenuKey = VK_INSERT;
        bool VSync = false;
    }Configs;
};


#endif