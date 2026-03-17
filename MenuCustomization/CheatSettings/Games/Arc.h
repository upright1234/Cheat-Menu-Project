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
        int TriggerMaxDistance = 200;
        int TriggerDelay = 50;
        float TriggerFOV = 5.0f;
        bool TriggerDrawFOV = false;
        float TriggerFOVColor[4] = { 1.f, 0.5f, 0.f, 0.6f };
        float TriggerFOVThickness = 1.5f;

        bool DeadZone = false;
        float DeadZoneRadius = 5.0f; //1-20

        int HoldPrimaryKey = VK_LBUTTON;
        int HoldSecondaryKey = VK_RBUTTON;
        int SwitchToHeadKey = VK_SHIFT;
        int ToggleKey = VK_F6;

        bool SaveTarget = true;

        bool AimLock = false;
        int LockFov = 5;
        float TargetSwitchDelay = 2.f; //0-5, seconds

        bool DrawFov = true;
        bool FovFilled = false;
        float FovFillColor[4] = { 0.f, 0.f, 0.f, 0.1f };
        float FovColor[4] = { 1.f, 1.f, 0.f, 1.f };
        bool FovChangeTargetingColor = true;
        float FovTargetingColor[4] = { 1.f, 0.f, 0.f, 1.f };
        float FovThickness = 1.5f;//1-5

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

        int HitBox = 0; //0 = head, 1 = chest, 2 = body, 3 = nearest
        float MinFOV = 5.f;
        float MaxFOV = 15.f;
        bool CustomFov = true; //false = static, true = dynamic by distance/time
        float DistanceCoefficient = 0.1f;
        float TimeCoefficient = 1.0f;
        float Smooth = 4.f;

        int MaxDistance = 600;

    }Aimbot;

    struct {
        struct {
            bool Enable = true;
            int ToggleKey = VK_F6;
            bool RemoveSleeping = false;
            bool RemoveWounded = false;
            bool TeamCheck = true;

            int MaxDistance = 800;
            float MinFontSize = 10.f;
            float MaxFontSize = 16.f;
            bool DistanceTransparency = true;
            float MinTransparency = 0.3f;
            float MaxTransparency = 1.0f;

            struct HeadCircleStruct {
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

            struct DistanceStruct {
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

            struct NickNameStruct {
                bool Enable = true;
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                int position = 1; //0 = top, 1 = bottom, 2 = left, 3 = right
                float StaticColor[4] = { 0.8f, 0.8f, 0.9f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255;
                int FontStyle = 0;
            }NickName;

            struct WeaponStruct {
                bool Enable = true;
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                int position = 2; //0 = top, 1 = bottom, 2 = left, 3 = right
                float StaticColor[4] = { 0.8f, 0.8f, 0.9f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255;
                int FontStyle = 0;
            }Weapon;

            struct BoxStruct {
                bool Enable = true;
                bool Filled = true;
                int Type = 1; //0 = square, 1 = corner
                int rounding = 10;
                float Thickness = 2.f;
                int ColorType = 1; //0 = static, 1 = by team, 2 = by visible
                float FillColor[4] = { 0.f, 0.f, 0.f, .2f };
                float StaticColor[4] = { 0.8f, 0.8f, 0.9f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255;
            }Box;

            struct LinesStruct {
                bool Enable = false;
                int Position = 0; //0 = top, 1 = bottom
                float Thickness = 1.f;
                int ColorType = 1; //0 = static, 1 = by team, 2 = by visible
                float StaticColor[4] = { 0.6f, 0.4f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255;
            }Lines;

            struct SkeletonStruct {
                bool Enable = true;
                float Thickness = 1.75f;
                int ColorType = 2; //0 = static, 1 = by team, 2 = by visible
                float StaticColor[4] = { 0.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255;
            }Skeleton;

            struct StatusStruct {
                bool Enable = true;
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                int position = 1; //0 = top, 1 = bottom, 2 = left, 3 = right
                float StaticColor[4] = { 1.f, 0.f, 0.f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255;
                int FontStyle = 0;
            }Status;

            struct VelocityStruct {
                bool Enable = true;
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                int position = 1; //0 = top, 1 = bottom, 2 = left, 3 = right
                float StaticColor[4] = { 0.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255;
                int FontStyle = 0;
            }Velocity;

            struct PredictionLinesStruct {
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

            struct ProximityAlertsStruct {
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
                int PosX = 300;
                int PosY = 20;
                float TextSize = 20.0f;
                bool Clothing = false;
                int FontStyle = 0;

            }HotBar;
        }Players;

        struct {
            bool Enable = true;
            std::vector<ItemSelector> List;
            std::vector<int> Selected;
            int ToggleKey = VK_F7;

            int MaxDistance = 800;
            float MinFontSize = 10.f;
            float MaxFontSize = 16.f;
            bool DistanceTransparency = true;
            float MinTransparency = 0.3f;
            float MaxTransparency = 1.0f;

            struct HeadCircleStruct {
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

            struct DistanceStruct {
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

            struct NickNameStruct {
                bool Enable = true;
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                int position = 1; //0 = top, 1 = bottom, 2 = left, 3 = right
                float StaticColor[4] = { 0.8f, 0.8f, 0.9f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255;
                int FontStyle = 0;
            }NickName;

            struct BoxStruct {
                bool Enable = true;
                bool Filled = true;
                int Type = 1; //0 = square, 1 = corner
                int rounding = 10;
                float Thickness = 2.f;
                int ColorType = 1; //0 = static, 1 = by team, 2 = by visible
                float FillColor[4] = { 0.f, 0.f, 0.f, .2f };
                float StaticColor[4] = { 0.8f, 0.8f, 0.9f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255;
            }Box;

            struct LinesStruct {
                bool Enable = false;
                int Position = 0; //0 = top, 1 = bottom
                float Thickness = 1.f;
                int ColorType = 1; //0 = static, 1 = by team, 2 = by visible
                float StaticColor[4] = { 0.6f, 0.4f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255;
            }Lines;

            struct SkeletonStruct {
                bool Enable = true;
                float Thickness = 1.75f;
                int ColorType = 2; //0 = static, 1 = by team, 2 = by visible
                float StaticColor[4] = { 0.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 0.2f, 0.2f, 1.f };
                float NonVisibleColor[4] = { 0.6f, 0.6f, 0.6f, 0.3f };
                int TeamColorAlpha = 255;
            }Skeleton;

        }NPC;

        struct {
            bool Enable = false;
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
            int MaxDistance = 150;

            int Type = 1;
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

        struct OutScreenStruct {
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
    }Visuals;

    struct {
        bool SilentAim = true;
        bool SuperBow = false;

        bool FovChanger = false;
        float FovChangerValue = 150.f;
        bool ThirdPerson = false;
        bool BrightNight = false;
        bool ChangeTime = false;
        float time = 24.f;

        bool ZoomFov = false;
        float ZoomFovValue = 30.f;
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
        int RecoilScale = 10;

        bool Chams = false;
        int PlayerChamsMaterial = 0;
        int NpcChamsMaterial = 0;

        int Language = 0;
        int GameFPS = 60;
        int RenderFPS = 144;
        int MenuKey = VK_INSERT;
        bool VSync = false;
    }Configs;
};


#endif