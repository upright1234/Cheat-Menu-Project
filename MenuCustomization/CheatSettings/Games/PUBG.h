#pragma once
#include <direct.h>
#include <stdio.h>
#include <Windows.h>
#include <psapi.h>
#include <intrin.h>
#include <vector>
#include "../Config.h"
#include "imconfig.h"

#if IsPUBG

struct SETTINGS {
    int iConfig = 0;
    int oldiConfig = 0;
    std::vector<std::string> InstalledFonts;
    std::vector<std::string> AvailableFonts;
    struct AimStruct {
        bool Enable = true;
        bool VisibilityCheck = true;
        bool RecoilControl = true;
        bool IgnoreKnocked = false;
        bool IgnoreDriving = false;
        bool IgnoreBots = false;
        int ByWeapon = 0; //0 = disabled, AR, DMR, LMG, Melee, Pistol, Shotgun, SMG, Sniper, Special, Throw
        bool Predict = false;
        int MaxSpectators = 5;

        bool SmoothInterpolation = true;
        float InterpolationSpeed = 0.8f;
        int InterpolationType = 0; // 0 = Linear, 1 = Bezier

        bool TriggerBot = false;
        int TriggerKey = VK_XBUTTON1;
        float TriggerDelay = 0.05f;
        float TriggerFOV = 2.0f;
        bool TriggerBotDrawFOV = true;
        float TriggerBotFOVColor[4] = { 1.0f, 0.5f, 0.0f, 0.6f };
        float TriggerBotFOVThickness = 2.0f;

        bool Humanization = false;
        float ReactionTime = 0.1f;
        float RandomOffset = 0.5f;

        bool KillSwitch = false;
        int KillSwitchKey = VK_F9;
        bool AutoKillSwitch = false;
        float KillSwitchDistance = 10.0f;

        bool DeadZone = false;
        float DeadZoneRadius = 5.0f;

        int HoldPrimaryKey = VK_LBUTTON;
        int HoldSecondaryKey = VK_RBUTTON;
        int SwitchToHeadKey = VK_SHIFT;
        int ToggleKey = VK_F6;

        bool SaveTarget = true;

        bool AimLock = true;
        float TargetSwitchDelay = 0.5f; //seconds

        bool DrawFov = true;
        float FovColor[4] = { 0.f, 0.f, 0.f, 0.1f };
        float FovThickness = 1.f;

        bool DrawCrossHair = true;
        float CrossHairColor[4] = { 1.f, 1.f, 1.f, 1.f };
        float CrossHairThickness = 1.f;

        bool DrawTarget = true;
        int TargetType = 1; //0 = circle, 1 = triangle
        float TargetThickness = 1.f;
        float TargetCollor[4] = { 1.f, 1.f, 1.f, 1.f };

        int HitBox = 0;
        //{
            float MinFOV = 5.f; //1.f
            float MaxFOV = 180.f; //180.f
            bool CustomFov = false; //false = static, true = increase by coefficient
            float DistanceCoefficient = 0;
            float TimeCoefficient = 0.3f;
        //}
        float Smooth = 2.f;
        int MaxDistance = 600;
    }Aim;
    AimStruct aimbot[10]; // AR, DMR, LMG, Melee, Pistol, Shotgun, SMG, Sniper, Special, Throw
    struct {
        struct {
            bool Enable = true;
            int ToggleKey = VK_F6;

            int MaxDistance = 800;
            float MinFontSize = 10.f;
            float MaxFontSize = 16.f;

            bool DistanceTransparency = true;

            float MinTransparency = 0.3f;
            float MaxTransparency = 1.0f;

            struct {
                bool Enable = true;
                float Size = 5.f;
                float Thickness = 4.f;
                bool Filled = false;
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float NonVisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                int TeamColorAlpha = 255;
            }HeadCircle;

            struct {
                bool Enable = true;
                bool MergeNickName = false;
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                int position = 3; //0 = top, 1 = bottom, 2 = left, 3 = right
                float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float NonVisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                int TeamColorAlpha = 255;
                int FontStyle = 0;
            }Distance;

            struct {
                bool Enable = true;
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                int position = 2; //0 = top, 1 = bottom, 2 = left, 3 = right
                float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float NonVisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                int TeamColorAlpha = 255;
                int FontStyle = 0;
            }NickName;

            struct {
                bool Enable = true;
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                int position = 0; //0 = top, 1 = bottom, 2 = left, 3 = right
                float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float NonVisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                int TeamColorAlpha = 255;
                int FontStyle = 0;
            }Weapon;

            struct {
                bool Enable = true;
                bool Filled = true;
                int Type = 1; //0 = square, 1 = corner
                int rounding = 20;
                float Thickness = 1.f;
                float Width = 0.35f;
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                float FillColor[4] = { 0.f, 0.f, 0.f, .2f };
                float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float NonVisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                int TeamColorAlpha = 255;
            }Box;

            struct {
                bool Enable = true;
                int Position = 0; //0 = top, 1 = bottom
                float Thickness = 1.f;
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float NonVisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                int TeamColorAlpha = 255;
            }Lines;

            struct {
                bool Enable = true;
                float Thickness = 1.f;
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float NonVisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                int TeamColorAlpha = 255;
            }Skeleton;

            struct {
                bool Enable = true;
                bool HealthGradient = true;
                int position = 1; //0 = top, 1 = bottom, 2 = left, 3 = right
                float Thickness = 4.f;
                int ColorAlpha = 255;
            }Health;

            struct {
                bool Enable = true;
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                int position = 1; //0 = top, 1 = bottom, 2 = left, 3 = right
                float StaticColor[4] = { 1.f, 0.f, 0.f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255;
                int FontStyle = 0;
            }Status;

            struct {
                bool Enable = true;
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                int position = 1; //0 = top, 1 = bottom, 2 = left, 3 = right
                float StaticColor[4] = { 0.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255;
                int FontStyle = 0;
            }Velocity;

            struct {
                bool Enable = true;
                float PredictionTime = 2.0f;
                float Length = 50.0f;
                float Thickness = 2.0f;
                bool ShowEndPoint = true;
                float EndPointSize = 3.0f;

                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                float StaticColor[4] = { 1.f, 1.f, 0.f, 0.8f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 204;

                float MinVelocity = 5.0f;
            }PredictionLines;

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
        }Players;

        struct {
            bool Enable = true;
            int PositionX = 10;
            int PositionY = 30;
            int ToggleKey = VK_F7;
            int MaxDistance = 150;

            int Zoom = 10;
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

            struct {
                bool Enable = true;
                int ShowType = 0; //0 by type, 1 by name
                bool Types[10] = { false, false, false, false, false, false, false, false, false, false };
                std::vector<ItemSelector> List;
                std::vector<int> Selected;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            }Weapon;

            struct {
                bool Enable = true;
                std::vector<ItemSelector> List;
                std::vector<int> Selected;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            }Package;

            struct {
                bool Enable = true;
                int ShowType = 0; //0 by type, 1 by name
                bool Types[2] = { true, true };
                std::vector<ItemSelector> List;
                std::vector<int> Selected;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            }Vehicle;

            struct {
                bool Enable = true;
                int ShowType = 0; //0 by holding, 1 by type, 2 by weapon name, 3 by name
                bool Types[10] = { false, false, false, false, false, false, false, false, false, false };
                std::vector<ItemSelector> List;
                std::vector<int> SelectedWeapon;
                std::vector<int> Selected;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            }Ammo;

            struct {
                bool Enable = true;
                int ShowType = 0; //0 by holding, 1 by type, 2 by weapon name, 3 by name
                bool Types[10] = { false, false, false, false, false, false, false, false, false, false };
                std::vector<ItemSelector> List;
                std::vector<int> SelectedWeapon;
                std::vector<int> Selected;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            }Attachment;

            struct {
                bool Enable = true;
                std::vector<ItemSelector> List;
                std::vector<int> Selected;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            }Throwables;

            struct {
                bool Enable = true;
                std::vector<ItemSelector> List;
                std::vector<int> Selected;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            }Medicine;

            struct {
                bool Enable = true;
                std::vector<ItemSelector> List;
                std::vector<int> Selected;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            }OtherEquipable;

            struct {
                bool Enable = true;
                std::vector<ItemSelector> List;
                std::vector<int> Selected;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            }Equipable;
        }Items;
    }Visuals;

    struct {
        int Language = 0;
        bool FeaturesDefinition = true;
        int GameFPS = 60; //literally your game FPS count
        int RenderFPS = 144; //The cheat render FPS limit
        bool SpectatorCount = true;
        int MenuKey = VK_INSERT;
    }Configs;
};

#endif