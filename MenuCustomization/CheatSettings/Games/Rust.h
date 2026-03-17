#pragma once
#include <direct.h>
#include <stdio.h>
#include <Windows.h>
#include <psapi.h>
#include <intrin.h>
#include <vector>
#include "../Config.h"
#include "imconfig.h"

#if IsRust

struct SETTINGS {

    int iConfig = 0;
    int oldiConfig = 0;
    ImVec2 specPos = ImVec2();
    ImVec2 radarPos = ImVec2();
    std::vector<std::string> InstalledFonts = { "FortFont", "VerdanaFont" };
    std::vector<std::string> AvailableFonts;


    struct AimStruct {
        bool Enable = true;
        bool MemoryAim = true;
        bool VisibilityCheck = true;
        bool AimNPC = true;
        bool IgnoreWounded = true;
        bool IgnoreSleeping = true;
        bool Predict = true;

        bool TriggerEnable = true;
        int TriggerKey = VK_XBUTTON1;
        bool TriggerWhileADS = true;
        bool TriggerIgnoreKnocked = true;
        bool TriggerByKey = true;
        int TriggerMaxDistance = 200; // 1-500
        int TriggerDelay = 50; // 0-250
        float TriggerFOV = 5.0f; // 1.0-30.0
        bool TriggerDrawFOV = false;
        float TriggerFOVColor[4] = { 1.f, 0.5f, 0.f, 0.6f };
        float TriggerFOVThickness = 1.5f; // 1.0-5.0

        bool DeadZone = false;
        float DeadZoneRadius = 5.0f; // 1.0-20.0 (Header), 0-25 (GUI)

        int HoldPrimaryKey = VK_LBUTTON;
        int HoldSecondaryKey = VK_RBUTTON;
        int SwitchToHeadKey = VK_SHIFT;
        int ToggleKey = VK_F6;

        bool SaveTarget = true;

        bool AimLock = false;
        int LockFov = 5; // 1-30
        float TargetSwitchDelay = 2.f; // 0.0-5.0 seconds

        bool DrawFov = true;
        bool FovFilled = false;
        float FovFillColor[4] = { 0.f, 0.f, 0.f, 0.1f };
        float FovColor[4] = { 1.f, 1.f, 0.f, 1.f };
        bool FovChangeTargetingColor = true;
        float FovTargetingColor[4] = { 1.f, 0.f, 0.f, 1.f };
        float FovThickness = 1.5f; // 1.0-5.0

        bool DrawCrossHair = true;
        int CrossHairType = 0; // 0 = Cross
        float CrossHairColor[4] = { 1.f, 1.f, 1.f, 0.6f };
        float CrossHairThickness = 1.f; // 1.0-5.0

        bool DrawTarget = true;
        int TargetType = 1; // 0 = Circle, 1 = Triangle
        float TargetThickness = 1.f; // 1.0-5.0
        float TargetSize = 3.f; // 1.0-20.0
        bool TargetFilled = true;
        bool TargetLine = false;
        int TargetFillOpacity = 30; // 0-255
        float TargetCollor[4] = { 1.f, 0.f, 0.f, 0.8f };

        int HitBox = 0; // 0 = Head, 1 = Chest, 2 = Body, 3 = Nearest
        float MinFOV = 5.f; // 1.0-180.0
        float MaxFOV = 15.f; // 1.0-180.0
        bool CustomFov = true; //false = static, true = dynamic by distance/time
        float DistanceCoefficient = 0.1f; // 0.0-3.0
        float TimeCoefficient = 1.0f; // 0.0-3.0
        float Smooth = 4.f; // 1.0-20.0

        int MaxDistance = 600; // 1-1000

    }Aimbot;

    struct {
        struct {
            bool Enable = true;
            int ToggleKey = VK_F6;
            bool RemoveSleeping = false;
            bool RemoveWounded = false;
            bool TeamCheck = true;

            int MaxDistance = 800; // 1-1000
            float MinFontSize = 10.f; // 8.0-24.0
            float MaxFontSize = 16.f; // 8.0-32.0
            bool DistanceTransparency = true;
            float MinTransparency = 0.3f; // 0.0-1.0
            float MaxTransparency = 1.0f; // 0.0-1.0

            struct HeadCircleStruct {
                bool Enable = true;
                float Size = 5.f; // 1.0-15.0
                float Thickness = 2.f; // 1.0-5.0
                bool Filled = false;
                int ColorType = 0; // 0 = Static, 1 = By Team, 2 = By Visible
                float StaticColor[4] = { 1.f, 0.f, 0.f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.7f, 0.7f, 0.7f, 0.3f };
                int TeamColorAlpha = 255; //1-255
            }HeadCircle;

            struct DistanceStruct {
                bool Enable = true;
                bool MergeNickName = true;
                int ColorType = 0; // 0 = Static, 1 = By Team, 2 = By Visible
                int position = 1; // 0 = Top, 1 = Bottom, 2 = Left, 3 = Right
                float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255; //1-255
                int FontStyle = 0;
            }Distance;

            struct NickNameStruct {
                bool Enable = true;
                int ColorType = 0; // 0 = Static, 1 = By Team, 2 = By Visible
                int position = 1; // 0 = Top, 1 = Bottom, 2 = Left, 3 = Right
                float StaticColor[4] = { 0.8f, 0.8f, 0.9f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255; //1-255
                int FontStyle = 0;
            }NickName;

            struct WeaponStruct {
                bool Enable = true;
                int ColorType = 0; // 0 = Static, 1 = By Team, 2 = By Visible
                int position = 2; // 0 = Top, 1 = Bottom, 2 = Left, 3 = Right
                float StaticColor[4] = { 0.8f, 0.8f, 0.9f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255; //1-255
                int FontStyle = 0;
            }Weapon;

            struct BoxStruct {
                bool Enable = true;
                bool Filled = true;
                int Type = 1; // 0 = Square, 1 = Corner
                int rounding = 10; // 0-20
                float Thickness = 2.f; // 1.0-5.0
                int ColorType = 1; // 0 = Static, 1 = By Team, 2 = By Visible
                float FillColor[4] = { 0.f, 0.f, 0.f, .2f };
                float StaticColor[4] = { 0.8f, 0.8f, 0.9f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255; //1-255
            }Box;

            struct LinesStruct {
                bool Enable = false;
                int Position = 0; // 0 = Top, 1 = Bottom
                float Thickness = 1.f; // 1.0-5.0
                int ColorType = 1; // 0 = Static, 1 = By Team, 2 = By Visible
                float StaticColor[4] = { 0.6f, 0.4f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255; //1-255
            }Lines;

            struct SkeletonStruct {
                bool Enable = true;
                float Thickness = 1.75f; // 1.0-5.0
                int ColorType = 2; // 0 = Static, 1 = By Team, 2 = By Visible
                float StaticColor[4] = { 0.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255; //1-255
            }Skeleton;

            struct StatusStruct {
                bool Enable = true;
                int ColorType = 0; // 0 = Static, 1 = By Team, 2 = By Visible
                int position = 1; // 0 = Top, 1 = Bottom, 2 = Left, 3 = Right
                float StaticColor[4] = { 1.f, 0.f, 0.f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255; //1-255
                int FontStyle = 0;
            }Status;

            struct VelocityStruct {
                bool Enable = true;
                int ColorType = 0; // 0 = Static, 1 = By Team, 2 = By Visible
                int position = 1; // 0 = Top, 1 = Bottom, 2 = Left, 3 = Right
                float StaticColor[4] = { 0.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255; //1-255
                int FontStyle = 0;
            }Velocity;

            struct PredictionLinesStruct {
                bool Enable = true;
                float PredictionTime = 2.0f; // 0.5-5.0
                float Length = 50.0f; // 5.0-200.0
                float Thickness = 2.0f; // 1.0-5.0
                bool ShowEndPoint = true;
                float EndPointSize = 3.0f; // 1.0-10.0

                int ColorType = 0; // 0 = Static, 1 = By Team, 2 = By Visible
                float StaticColor[4] = { 1.f, 1.f, 0.f, 0.8f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 204; //1-255

                float MinVelocity = 5.0f; // 0.0-20.0
            }PredictionLines;

            struct ProximityAlertsStruct {
                bool Enable = true;
                float ProximityRadius = 50.0f; // 5.0-150.0

                bool PulsatingBorder = true;
                bool ProximityText = true;

                float BorderThickness = 4.0f; // 1.0-10.0
                float TextSize = 20.0f; // 10.0-40.0

                float AlertColor[4] = { 1.f, 0.1f, 0.1f, 0.9f };
                float TextColor[4] = { 1.f, 1.f, 0.f, 1.f };

            }ProximityAlerts;

            struct {
                bool Enable = true;
                int PosX = 300;
                int PosY = 20;
                float TextSize = 20.0f; // 10.0-40.0
                bool Clothing = false;
                int FontStyle = 0;

            }HotBar;
        }Players;

        struct {
            bool Enable = true;
            std::vector<ItemSelector> List;
            std::vector<int> Selected;
            int ToggleKey = VK_F7;

            int MaxDistance = 800; // 1-1000
            float MinFontSize = 10.f; // 8.0-24.0
            float MaxFontSize = 16.f; // 8.0-32.0
            bool DistanceTransparency = true;
            float MinTransparency = 0.3f; // 0.0-1.0
            float MaxTransparency = 1.0f; // 0.0-1.0

            struct HeadCircleStruct {
                bool Enable = true;
                float Size = 5.f; // 1.0-15.0
                float Thickness = 2.f; // 1.0-5.0
                bool Filled = false;
                int ColorType = 0; // 0 = Static, 1 = By Team, 2 = By Visible
                float StaticColor[4] = { 1.f, 0.f, 0.f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.7f, 0.7f, 0.7f, 0.3f };
                int TeamColorAlpha = 255;
            }HeadCircle;

            struct DistanceStruct {
                bool Enable = true;
                bool MergeNickName = true;
                int ColorType = 0; // 0 = Static, 1 = By Team, 2 = By Visible
                int position = 1; // 0 = Top, 1 = Bottom, 2 = Left, 3 = Right
                float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255; //1-255
                int FontStyle = 0;
            }Distance;

            struct NickNameStruct {
                bool Enable = true;
                int ColorType = 0; // 0 = Static, 1 = By Team, 2 = By Visible
                int position = 1; // 0 = Top, 1 = Bottom, 2 = Left, 3 = Right
                float StaticColor[4] = { 0.8f, 0.8f, 0.9f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255; //1-255
                int FontStyle = 0;
            }NickName;

            struct BoxStruct {
                bool Enable = true;
                bool Filled = true;
                int Type = 1; // 0 = Square, 1 = Corner
                int rounding = 10; // 0-20
                float Thickness = 2.f; // 1.0-5.0
                int ColorType = 1; // 0 = Static, 1 = By Team, 2 = By Visible
                float FillColor[4] = { 0.f, 0.f, 0.f, .2f };
                float StaticColor[4] = { 0.8f, 0.8f, 0.9f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255; //1-255
            }Box;

            struct LinesStruct {
                bool Enable = false;
                int Position = 0; // 0 = Top, 1 = Bottom
                float Thickness = 1.f; // 1.0-5.0
                int ColorType = 1; // 0 = Static, 1 = By Team, 2 = By Visible
                float StaticColor[4] = { 0.6f, 0.4f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255; //1-255
            }Lines;

            struct SkeletonStruct {
                bool Enable = true;
                float Thickness = 1.75f; // 1.0-5.0
                int ColorType = 2; // 0 = Static, 1 = By Team, 2 = By Visible
                float StaticColor[4] = { 0.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255; //1-255
            }Skeleton;

        }NPC;

        struct {
            bool Enable = false;
            int ToggleKey = VK_F8;

            int MaxDistance = 150; // 1-500
            float MinFontSize = 10.f; // 8.0-24.0
            float MaxFontSize = 16.f; // 8.0-32.0
            int FontStyle = 0;
            bool DistanceTransparency = true;
            float MinTransparency = 0.3f; // 0.0-1.0
            float MaxTransparency = 1.0f; // 0.0-1.0

            struct {
                bool Enable = true;
                std::vector<int> Selected;  // Indices of selected categories (0-13)
                float Color[4] = { 1.f, 1.f, 1.f, 1.f };
            } WorldDrop;

            struct {
                bool Enable = true;
                std::vector<ItemSelector> List;
                std::vector<int> Selected;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            } Ores;

            struct {
                bool Enable = true;
                std::vector<ItemSelector> List;
                std::vector<int> Selected;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            } Animals;

            struct {
                bool Enable = true;
                std::vector<ItemSelector> List;
                std::vector<int> Selected;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            } Vehicles;

            struct {
                bool Enable = true;
                std::vector<ItemSelector> List;
                std::vector<int> Selected;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            } Traps;

            struct {
                bool Enable = true;
                std::vector<ItemSelector> List;
                std::vector<int> Selected;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            } Crates;

            struct {
                bool Enable = true;
                std::vector<ItemSelector> List;
                std::vector<int> Selected;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            } Barrels;

            struct {
                bool Enable = true;
                std::vector<ItemSelector> List;
                std::vector<int> Selected;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            } Corpse;

            struct {
                bool Enable = true;
                std::vector<ItemSelector> List;
                std::vector<int> Selected;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            } Food;

            struct {
                bool Enable = true;
                std::vector<ItemSelector> List;
                std::vector<int> Selected;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            } Collectibles;

            struct {
                bool Enable = true;
                std::vector<ItemSelector> List;
                std::vector<int> Selected;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            } Misc;
        }Items;

        struct RadarStruct {
            bool Enable = true;
            int ToggleKey = VK_F7;
            int MaxDistance = 150; // 1-500

            int Type = 1; // 0 = Dot, 1 = Arrow
            int Zoom = 10; // 1-50
            int Size = 200; // 100-400

            float MinSize = 1.0f; // 1.0-10.0
            float MaxSize = 4.0f; // 1.0-20.0
            float MinOpacity = 0.2f; // 0.0-1.0
            float MaxOpacity = 1.0f; // 0.0-1.0
            int ColorType = 0; // 0 = Static, 1 = By Team, 2 = By Visible
            float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
            float VisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
            float NonVisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
            int TeamColorAlpha = 255; //1-255
        }Radar;

        struct OutScreenStruct {
            bool Enable = true;
            int ToggleKey = VK_F7;
            int MaxDistance = 150; // 1-500

            int Type = 0; // 0 = Circle, 1 = Triangle
            int Fov = 15; // 1-90
            float Thickness = 2.f; // 1.0-5.0
            float MinOpacity = 0.2f; // 0.0-1.0
            float MaxOpacity = 1.0f; // 0.0-1.0
            float MinSize = 10.0f; // 1.0-20.0
            float MaxSize = 30.0f; // 1.0-40.0
            int ColorType = 0; // 0 = Static, 1 = By Team, 2 = By Visible
            float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
            float VisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
            float NonVisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
            int TeamColorAlpha = 255; //1-255
        }OutScreen;
    }Visuals;

    struct {
        bool SilentAim = true;
        bool SuperBow = false;

        bool FovChanger = false;
        float FovChangerValue = 150.0f; // 30.0-180.0
        bool ThirdPerson = false;
        bool BrightNight = false;
        bool ChangeTime = false;
        float time = 24.0f; // 0.0-24.0

        bool ZoomFov = false;
        float ZoomFovValue = 30.0f; // 10.0-120.0
        int ZoomKey = VK_INSERT;

        bool SpiderMan = false;
        bool ThickBullet = false;
        bool FullAuto = false;
        bool InstantEoka = false;
        bool ExtendedMelee = false;
        bool AttackOnMountable = false;
        bool NoFall = false;
        bool OmniSprint = false;

        bool NoSway = false;
        bool NoSpread = false;
        bool RecoilControl = false;
        int RecoilScale = 10; // 0-100

        bool Chams = false;
        int PlayerChamsMaterial = 0; // 0 = Red [Default], 1 = White, 2 = Mirror, 3 = Gray, 4 = Bright Sky Blue
        int NpcChamsMaterial = 0; // 0 = Red [Default], 1 = White, 2 = Mirror, 3 = Gray, 4 = Bright Sky Blue

        int Language = 0;
        int GameFPS = 60; // 30-240
        int RenderFPS = 144; // 30-360
        int MenuKey = VK_INSERT;
        bool VSync = false;
    }Configs;
};

#endif