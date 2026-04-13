#pragma once
#include <direct.h>
#include <stdio.h>
#include <Windows.h>
#include <psapi.h>
#include <intrin.h>
#include <vector>
#include "../Config.h"
#include "imconfig.h"

#if IsR6

struct SETTINGS {
    int iConfig = 0;
    int oldiConfig = 0;
    std::vector<std::string> InstalledFonts = { "FortFont", "VerdanaFont" };
    std::vector<std::string> AvailableFonts;
    ImVec2 radarPos = ImVec2(15, 100);
    struct AimStruct {
        bool Enable = true;

        bool DeadZone = false;
        float DeadZoneRadius = 5.0f; //1-20

        int HoldPrimaryKey = VK_LBUTTON;
        int HoldSecondaryKey = VK_RBUTTON;
        int SwitchToHeadKey = VK_SHIFT;
        int ToggleKey = VK_F6;

        bool SaveTarget = true;
        bool AimLock = false;
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

        struct GadgetStruct {
            bool Enable = false;
            int AimKey = VK_XBUTTON1;
            float Smooth = 5.f;
            float Fov = 10.f;
            bool DrawFov = true;
            bool FovFilled = false;
            float FovFillColor[4] = { 0.f, 0.f, 0.f, 0.1f };
            float FovColor[4] = { 1.f, 1.f, 0.f, 1.f };
            bool FovChangeTargetingColor = true;
            float FovTargetingColor[4] = { 1.f, 0.f, 0.f, 1.f };
            float FovThickness = 1.5f;//1-5
        } Gadget;

    }Aim;

    struct VisualsStr {
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
                float ProximityRadius = 50.0f;

                bool PulsatingBorder = true;
                bool ProximityText = true;

                float BorderThickness = 4.0f;
                float TextSize = 20.0f;

                float AlertColor[4] = { 1.f, 0.1f, 0.1f, 0.9f };
                float TextColor[4] = { 1.f, 1.f, 0.f, 1.f };

            }ProximityAlerts;

            struct {
                bool Enable = true;
                int position = 1;
                int ColorType = 0;
                float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float NonVisibleColor[4] = { 0.8f, 0.8f, 0.8f, 0.5f };
                int TeamColorAlpha = 255;
                int FontStyle = 0;
            }Distance;

        };
        PlayerStr Players;

        struct WorldStr {
            bool Enable = true;
            bool ShowGadgets = true;
            bool ShowElectronics = true;
            int MaxDistance = 150;
            float MinFontSize = 10.f;
            float MaxFontSize = 16.f;
            bool DistanceTransparency = true;
            float MinTransparency = 0.6f;
            float MaxTransparency = 1.0f;
        };
        WorldStr World;

        struct {
            bool Enable = true;
            int ToggleKey = VK_F7;
            int MaxDistance = 150;

            int Zoom = 10;
            int Size = 200;

            float MinSize = 1.f;
            float MaxSize = 4.f;
            float MinOpacity = 0.2f;
            float MaxOpacity = 1.0f;
            float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
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