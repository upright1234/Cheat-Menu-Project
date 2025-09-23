#pragma once
#include <direct.h>
#include <stdio.h>
#include <Windows.h>
#include <psapi.h>
#include <intrin.h>
#include <vector>
#include "../Config.h"
#include "imconfig.h"

#if IsFortnite

struct SETTINGS {
    int iConfig = 0;
    int oldiConfig = 0;
    int selectedWeapon = 0; //0 = Shotgun, 1 = SMG, 2 = Pistol, 3 = Bolt, 4 = Sniper, 5 = Rifle , 6 = Others
    std::vector<std::string> InstalledFonts;
    std::vector<std::string> AvailableFonts;
    struct AimStruct {
        bool ByWeapon = false;
        bool Enable = true;
        bool VisibilityCheck = true;
        bool IgnoreKnocked = false;
        bool IgnoreBots = false;
        bool WeaponOnly = false;
        bool Predict = true;

        bool TriggerBot = false;
        int TriggerKey = VK_XBUTTON1;
        float TriggerDelay = 0.05f; //0.1-1
        int TriggerMaxDistance = 150; //1-1000

        bool DeadZone = false;
        float DeadZoneRadius = 5.0f; //1-20

        int HoldPrimaryKey = VK_LBUTTON;
        int HoldSecondaryKey = VK_RBUTTON;
        int SwitchToHeadKey = VK_SHIFT;
        int ToggleKey = VK_F6;

        bool SaveTarget = true;

        bool AimLock = true;
        float TargetSwitchDelay = 0.5f; //0-5, seconds

        bool DrawFov = true;
        float FovColor[4] = { 0.f, 0.f, 0.f, 0.1f };
        float FovThickness = 1.f;//1-5

        bool DrawCrossHair = true;
        float CrossHairColor[4] = { 1.f, 1.f, 1.f, 1.f };
        float CrossHairThickness = 1.f;//1-5

        bool DrawTarget = true;
        int TargetType = 1; //0 = circle, 1 = triangle
        float TargetThickness = 1.f;//1-5
        float TargetCollor[4] = { 1.f, 1.f, 1.f, 1.f };

        int HitBox = 0;
        //{
        float MinFOV = 5.f; //1.f
        float MaxFOV = 180.f; //180.f
        bool CustomFov = false; //false = static, true = increase by coefficient
        float DistanceCoefficient = 0; //0-3
        float TimeCoefficient = 0.3f; //0-3
        //}
        float Smooth = 2.f;//1-10
        int MaxDistance = 600;//1-1000
    }Aim;
    AimStruct aimbot[7]; // 
    struct {
        struct {
            bool Enable = true;
            int ToggleKey = VK_F6;

            int MaxDistance = 800;//1-1000
            float MinFontSize = 10.f;//10-30
            float MaxFontSize = 16.f;//10-30

            bool DistanceTransparency = true;

            float MinTransparency = 0.3f;//0-1
            float MaxTransparency = 1.0f;//0-1

            struct {
                bool Enable = true;
                float Size = 5.f;//1-10
                float Thickness = 4.f;//1-5
                bool Filled = false;
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float NonVisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                int TeamColorAlpha = 255;//0-255
            }HeadCircle;

            struct {
                bool Enable = true;
                bool MergeNickName = false;
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                int position = 3; //0 = top, 1 = bottom, 2 = left, 3 = right
                float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float NonVisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                int TeamColorAlpha = 255;//0-255
                int FontStyle = 0;
            }Distance;

            struct {
                bool Enable = true;
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                int position = 2; //0 = top, 1 = bottom, 2 = left, 3 = right
                float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float NonVisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                int TeamColorAlpha = 255;//0-255
                int FontStyle = 0;
            }NickName;

            struct {
                bool Enable = true;
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                int position = 0; //0 = top, 1 = bottom, 2 = left, 3 = right
                float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float NonVisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                int TeamColorAlpha = 255;//0-255
                int FontStyle = 0;
            }Weapon;

            struct {
                bool Enable = true;
                bool Filled = true;
                int Type = 1; //0 = square, 1 = corner
                int rounding = 20;//1-20
                float Thickness = 1.f;//1-5
                float Width = 0.35f;//0.1-1
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                float FillColor[4] = { 0.f, 0.f, 0.f, .2f };
                float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float NonVisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                int TeamColorAlpha = 255;//0-255
            }Box;

            struct {
                bool Enable = true;
                int Position = 0; //0 = top, 1 = bottom
                float Thickness = 1.f;//1-5
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float NonVisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                int TeamColorAlpha = 255;//0-255
            }Lines;

            struct {
                bool Enable = true;
                float Thickness = 1.f;//1-5
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float NonVisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                int TeamColorAlpha = 255;//0-255
            }Skeleton;

            struct {
                bool Enable = true;
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                int position = 1; //0 = top, 1 = bottom, 2 = left, 3 = right
                float StaticColor[4] = { 1.f, 0.f, 0.f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255;//0-255
                int FontStyle = 0;
            }Status;

            struct {
                bool Enable = true;
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                int position = 1; //0 = top, 1 = bottom, 2 = left, 3 = right
                float StaticColor[4] = { 0.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255;//0-255
                int FontStyle = 0;
            }Velocity;

            struct {
                bool Enable = true;
                float PredictionTime = 2.0f;//0.5-5
                float Length = 50.0f;//10-200
                float Thickness = 2.0f;//1-5
                bool ShowEndPoint = true;
                float EndPointSize = 3.0f;//1-10

                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                float StaticColor[4] = { 1.f, 1.f, 0.f, 0.8f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 204;//0-255

                float MinVelocity = 5.0f;//1-20
            }PredictionLines;

            struct {
                bool Enable = true;
                float ProximityRadius = 50.0f;//10-200

                bool PulsatingBorder = true;
                bool ProximityText = true;

                float BorderThickness = 4.0f;//1-5
                float TextSize = 20.0f;//10-50

                float AlertColor[4] = { 1.f, 0.1f, 0.1f, 0.9f };
                float TextColor[4] = { 1.f, 1.f, 0.f, 1.f };

            }ProximityAlerts;
        }Players;

        struct {
            bool Enable = true;
            int PositionX = 10;
            int PositionY = 30;
            int ToggleKey = VK_F7;
            int MaxDistance = 150;//1-1000

            int Zoom = 10;//1-100
            int Size = 200;//50-500

            float MinSize = 1.f;//1-6
            float MaxSize = 4.f;//1-6
            float MinOpacity = 0.2f;//0-1
            float MaxOpacity = 1.0f;//0-1
            int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
            float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
            float VisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
            float NonVisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
            int TeamColorAlpha = 255;//0-255
        }Radar;

        struct {
            bool Enable = true;
            int ToggleKey = VK_F7;
            int MaxDistance = 150;//1-1000

            int Type = 0;
            int Fov = 15;//1-180
            float Thickness = 2.f;//1-5
            float MinOpacity = 0.2f;//0-1
            float MaxOpacity = 1.0f;//0-1
            float MinSize = 10.f;//1-30
            float MaxSize = 30.f;//1-30
            int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
            float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
            float VisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
            float NonVisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
            int TeamColorAlpha = 255;//0-255
        }OutScreen;

        struct {
            bool Enable = false;
            int ToggleKey = VK_F8;

            int MaxDistance = 150;//1-1000
            float MinFontSize = 10.f;//10-30
            float MaxFontSize = 16.f;//10-30

            int FontStyle = 0;

            bool Chest = true;
            bool Vehicle = true;
            bool Ammo = true;
            bool Weapon = true;
            bool Consumable = true;
            bool Material = true;
            bool SupplyDrop = true;

            int Rarity = 0; //0 = All, 1 = Uncommon, 2 = Rare, 3 = Epic, 4 = Legendary
        }Items;
    }Visuals;

    struct {
        int Language = 0;
        bool FeaturesDefinition = true;
        int GameFPS = 60;//30-240 //literally your game FPS count
        int RenderFPS = 144;//30-360 //The cheat render FPS limit
        int MenuKey = VK_INSERT;
        bool VSync = false;
    }Configs;
};

#endif