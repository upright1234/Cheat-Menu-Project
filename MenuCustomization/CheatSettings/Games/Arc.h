#pragma once
#include <direct.h>
#include <stdio.h>
#include <Windows.h>
#include <psapi.h>
#include <intrin.h>
#include <vector>
#include "../Config.h"
#include "imconfig.h"

#if IsArc

struct SETTINGS {
    int iConfig = 0;
    int oldiConfig = 0;
    std::vector<std::string> InstalledFonts = { "FortFont", "VerdanaFont" };
    std::vector<std::string> AvailableFonts;
    struct AimStruct {
        int ByWeapon = 0; //0 = disabled, AR, DMR, LMG, Pistol, Shotgun, SMG, Sniper
        bool Enable = true;
        bool MemoryAim = false;
        bool VisibilityCheck = true;
        bool RecoilControl = true;
        int RecoilControlValue = 100; //0-100
        bool IgnoreKnocked = false;
        int MaxSpectators = 5;//0-20

        bool TriggerEnable = true;
        int TriggerKey = VK_XBUTTON1;
        bool TriggerWhileADS = true;
        bool TriggerIgnoreKnocked = true;
        bool TriggerByKey = true;
        int TriggerMaxDistance = 200; //1-1000
        int TriggerDelay = 50; //ms

        bool DeadZone = false;
        float DeadZoneRadius = 5.0f; //1-20

        int HoldPrimaryKey = VK_LBUTTON;
        int HoldSecondaryKey = VK_RBUTTON;
        int SwitchToHeadKey = VK_SHIFT;
        int ToggleKey = VK_F6;

        bool SaveTarget = true;

        bool AimLock = true;
        bool Predict[2] = { false, false }; //velocity, drop
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
        struct SkyNadeStruct {
            bool Enable = false;
            int Key = VK_F10;
            bool ShowIndicator = true;
            float IndicatorColor[4] = { 0.0f, 1.0f, 0.5f, 0.8f };
            float FontSize = 10.f;
        }SkyNade;
    }Aim;
    AimStruct aimbot[7]; // AR, DMR, LMG, Pistol, Shotgun, SMG, Sniper
    struct {
        struct {
            bool Enable = false;
            bool Text = false;
            bool Glow = true;
            int ToggleKey = VK_F8;

            int MaxDistance = 150;
            float MinFontSize = 10.f;
            float MaxFontSize = 16.f;
            int FontStyle = 0;
            bool DistanceTransparency = true;

            float MinTransparency = 0.3f;
            float MaxTransparency = 1.0f;

            struct {
                bool Enable = true;
                int ShowType = 0; //0 by type, 1 by name
                bool Types[7] = { false, false, false, false, false, false, false };
                std::vector<ItemSelector> List;
                std::vector<int> Selected;
                bool rarity_color = true;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            }Weapon;

            struct {
                bool Enable = true;
                std::vector<ItemSelector> List;
                std::vector<int> Selected;
                bool rarity_color = true;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            }Ammo;

            struct {
                bool Enable = true;
                int ShowType = 0; //0 by type, 1 by name, 2 by current weapon
                bool Types[7] = { false, false, false, false, false, false, false }; // 0=Sights, 1=Barrel, 2=Magazine, 3=Stock, 4=ShotgunBolt, 5=HopUp, 6=Laser
                std::vector<ItemSelector> List;
                std::vector<int> Selected;
                bool rarity_color = true;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            }Attachment;

            struct {
                bool Enable = true;
                std::vector<ItemSelector> List;
                std::vector<int> Selected;
                bool rarity_color = true;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            }Grenades;

            struct {
                bool Enable = true;
                std::vector<ItemSelector> List;
                std::vector<int> Selected;
                bool rarity_color = true;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            }Medicine;

            struct {
                bool Enable = true;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            }Deathbox;

            struct {
                bool Enable = true;
                std::vector<ItemSelector> List;
                std::vector<int> Selected;
                bool rarity_color = true;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            }Helmet;

            struct {
                bool Enable = true;
                std::vector<ItemSelector> List;
                std::vector<int> Selected;
                bool rarity_color = true;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            }Body;

            struct {
                bool Enable = true;
                std::vector<ItemSelector> List;
                std::vector<int> Selected;
                bool rarity_color = true;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            }Knockdown;

            struct {
                bool Enable = true;
                std::vector<ItemSelector> List;
                std::vector<int> Selected;
                bool rarity_color = true;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            }Backpack;
        }Items;

        struct {
            bool Enable = true;
            int ToggleKey = VK_F6;
            bool IgnoreKnocked = true;

            int MaxDistance = 800;
            float MinFontSize = 10.f;
            float MaxFontSize = 16.f;
            bool DistanceTransparency = true;

            float MinTransparency = 0.3f;
            float MaxTransparency = 1.0f;

            struct {
                bool Enable = true;
                int Radius = 64;
                int BodyStyle = 15;
                int OutlineStyle = 1;
                float VisibleColor[4] = { 0.62f, 0.05f, 0.05f };
                float NonVisibleColor[4] = { 0.62f, 0.05f, 0.05f };
            }Glow;

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
                bool MergeNickName = true;
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                int position = 1; //0 = top, 1 = bottom, 2 = left, 3 = right
                float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255;
                int FontStyle = 0;
            }Distance;

            struct {
                bool Enable = true;
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                int position = 1; //0 = top, 1 = bottom, 2 = left, 3 = right
                float StaticColor[4] = { 0.8f, 0.8f, 0.9f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255;
                int FontStyle = 0;
            }NickName;

            struct {
                bool Enable = true;
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                int position = 2; //0 = top, 1 = bottom, 2 = left, 3 = right
                float StaticColor[4] = { 0.8f, 0.8f, 0.9f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255;
                int FontStyle = 0;
            }Weapon;

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
                bool Gradient = true;
                int position = 2; //0 = top, 1 = bottom, 2 = left, 3 = right
                float Thickness = 4.f;
                int ColorAlpha = 205;
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
    }Visuals;

    struct {
        bool SpectatorCount = false;
        bool FakeDuck = false;
        int FakeDuckKey = 0x5A;
        bool Bhop = false;
        int BhopKey = VK_SPACE;
        int JumpKey = VK_SPACE;
        bool SkinChanger = false;
        int SkinID = 1;
        bool RapidFire = true;
        int RapidFireDelay = 75;
        int RapidFireKey = VK_XBUTTON2;

        int Language = 0;
        bool FeaturesDefinition = true;
        int GameFPS = 60;//30-240 //literally your game FPS count
        int RenderFPS = 144;//30-360 //The cheat render FPS limit
        int MenuKey = VK_INSERT;
        bool VSync = false;
    }Configs;
};


#endif