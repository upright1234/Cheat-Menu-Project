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
    int selectedWeapon = 0;
    struct AimStruct {
        bool Enable = true;
        bool VisibilityCheck = true;
        bool RecoilControl = true;
        bool IgnoreKnocked = false;
        bool ByWeapon = false;
        bool Predict = false;

        int HoldPrimaryKey = VK_LBUTTON;
        int HoldSecondaryKey = VK_RBUTTON;
        int SwitchToHeadKey = VK_SHIFT;
        int ToggleKey = VK_F6;

        bool SaveTarget = true;

        bool AimLock = true;
        float TargetSwitchDelay = 0.5f; //0-5, seconds

        bool DrawFov = true;
        float FovColor[4] = { 0.f, 0.f, 0.f, 0.1f };
        float FovThickness = 1.f; //1-5

        bool DrawCrossHair = true;
        float CrossHairColor[4] = { 1.f, 1.f, 1.f, 1.f };
        float CrossHairThickness = 1.f; //1-5

        bool DrawTarget = true;
        int TargetType = 1; //0 = circle, 1 = triangle
        float TargetThickness = 1.f;//1-5
        float TargetCollor[4] = { 1.f, 1.f, 1.f, 1.f };

        int HitBox = 0; //0 = Head, 1 = Chest, 2 = Body, 3 = Random
        //{
            float MinFOV = 5.f; //0-180
            float MaxFOV = 180.f; //0-180
            bool CustomFov = false; //false = static, true = increase by coefficient
            float DistanceCoefficient = 0.3f;
            float TimeCoefficient = 0.3f;
        //}
        float Smooth = 2.f; //1-10
        bool HumanizedSmooth = true;

        float HumanizedSmoothPercent = 2.f; //1-10
        int MaxDistance = 600; //0-1000
    }Aim;
    AimStruct aimbot[10]; // AR, DMR, LMG, Melee, Pistol, Shotgun, SMG, Sniper, Special, Throw
    struct {
        struct {
            bool Enable = true;
            int ToggleKey = VK_F6;

            int MaxDistance = 800; //0-1000
            float MinFontSize = 10.f; //10-30
            float MaxFontSize = 16.f; //10-30

            struct {
                bool Enable = true;
                float Size = 5.f; //1-10
                float Thickness = 4.f;//1-5
                bool Filled = false;
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float NonVisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                int TeamColorAlpha = 255; //0-255
            }HeadCircle;

            struct {
                bool Enable = true;
                bool MergeNickName = false;
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                int position = 3; //0 = top, 1 = bottom, 2 = left, 3 = right
                float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float NonVisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                int TeamColorAlpha = 255; //0-255
            }Distance;

            struct {
                bool Enable = true;
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                int position = 2; //0 = top, 1 = bottom, 2 = left, 3 = right
                float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float NonVisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                int TeamColorAlpha = 255; //0-255
            }NickName;

            struct {
                bool Enable = true;
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                int position = 0; //0 = top, 1 = bottom, 2 = left, 3 = right
                float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float NonVisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                int TeamColorAlpha = 255; //0-255
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
                int TeamColorAlpha = 255; //0-255
            }Box;

            struct {
                bool Enable = true;
                int Position = 0; //0 = top, 1 = bottom
                float Thickness = 1.f;//1-5
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float NonVisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                int TeamColorAlpha = 255; //0-255
            }Lines;

            struct {
                bool Enable = true;
                float Thickness = 1.f;//1-5
                int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
                float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float VisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                float NonVisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
                int TeamColorAlpha = 255; //0-255
            }Skeleton;

            struct {
                bool Enable = true;
                int position = 1; //0 = top, 1 = bottom, 2 = left, 3 = right
                float Thickness = 4.f;//1-5
                int ColorAlpha = 255; //0-255
            }Health;
        }Players;

        struct {
            bool Enable = true;
            int PositionX = 10;
            int PositionY = 30;
            int ToggleKey = VK_F7;
            int MaxDistance = 150; //0-1000

            int Zoom = 10; //1-100
            int Size = 200; //50-500

            float MinSize = 1.f; //1-6
            float MaxSize = 4.f; //1-6
            float MinOpacity = 0.2f; //0-1
            float MaxOpacity = 1.0f; //0-1
            int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
            float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
            float VisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
            float NonVisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
            int TeamColorAlpha = 255; //0-255
        }Radar;

        struct {
            bool Enable = true;
            int ToggleKey = VK_F7;
            int MaxDistance = 150; //0-1000

            int Type = 0; //0 = Circle, 1 = Triange
            int Fov = 15; //0-180
            float Thickness = 2.f;//1-5
            float MinOpacity = 0.2f; //0-1
            float MaxOpacity = 1.0f; //0-1
            float MinSize = 1.f; //1-6
            float MaxSize = 6.f; //1-6
            int ColorType = 0; //0 = static, 1 = by team, 2 = by visible
            float StaticColor[4] = { 1.f, 1.f, 1.f, 1.f };
            float VisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
            float NonVisibleColor[4] = { 1.f, 1.f, 1.f, 1.f };
            int TeamColorAlpha = 255; //0-255
        }OutScreen;

        struct {
            bool Enable = false;
            int ToggleKey = VK_F8;

            int MaxDistance = 150; //0-1000
            float MinFontSize = 10.f; //10-30
            float MaxFontSize = 16.f; //10-30

            struct {
                bool Enable = true;
                int ShowType = 0; //0 by type, 1 by name
                bool Types[10] = { false, false, false, false, false, false, false, false, false, false };
                std::vector<ItemSelector> List;
                std::vector<std::string> Selected;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            }Weapon;

            struct {
                bool Enable = true;
                std::vector<ItemSelector> List;
                std::vector<std::string> Selected;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            }Package;

            struct {
                bool Enable = true;
                int ShowType = 0; //0 by type, 1 by name
                bool Types[2] = { true, true };
                std::vector<ItemSelector> List;
                std::vector<std::string> Selected;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            }Vehicle;

            struct {
                bool Enable = true;
                int ShowType = 0; //0 by holding, 1 by type, 2 by weapon name, 3 by name
                bool Types[10] = { false, false, false, false, false, false, false, false, false, false };
                std::vector<ItemSelector> List;
                std::vector<std::string> SelectedWeapon;
                std::vector<std::string> Selected;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            }Ammo;

            struct {
                bool Enable = true;
                int ShowType = 0; //0 by holding, 1 by type, 2 by weapon name, 3 by name
                bool Types[10] = { false, false, false, false, false, false, false, false, false, false };
                std::vector<ItemSelector> List;
                std::vector<std::string> SelectedWeapon;
                std::vector<std::string> Selected;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            }Attachment;

            struct {
                bool Enable = true;
                std::vector<ItemSelector> List;
                std::vector<std::string> Selected;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            }Throwables;

            struct {
                bool Enable = true;
                std::vector<ItemSelector> List;
                std::vector<std::string> Selected;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            }Medicine;

            struct {
                bool Enable = true;
                std::vector<ItemSelector> List;
                std::vector<std::string> Selected;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            }OtherEquipable;

            struct {
                bool Enable = true;
                std::vector<ItemSelector> List;
                std::vector<std::string> Selected;
                float Color[4] = { 1.f, 0.f, 0.f, 1.f };
            }Equipable;
        }Items;
    }Visuals;

    struct {
        int Language = 0; //ignore
        bool FeaturesDefinition = true; //ignore
        int FontStyle = 0; //0 = Verdana, 1 = Fortnite
        int GameFPS = 60; //literally your game FPS count
        int RenderFPS = 144; //The cheat render FPS limit
        bool SpectatorCount = true;
        int MenuKey = VK_INSERT;
    }Configs;
};

#endif