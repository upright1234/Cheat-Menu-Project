#pragma once
#include <direct.h>
#include <stdio.h>
#include <Windows.h>
#include <psapi.h>
#include <intrin.h>
#include <vector>

struct SETTINGS {
    int iConfig = 0;
    int oldiConfig = 0;
    struct {

        bool Enable = true;
        bool SaveTarget = true;
        bool AimLock = false;
        bool VisibilityCheck = true;
        bool IgnoreKnocked = false;

        bool DrawFov = true;
        bool DrawCrossHair = true;
        bool DrawTarget = true;
        int DrawTargetType = 0;
        float DrawThickness = 1.f;

        int HitBox = 0;

        float FOV = 5.f;
        float min_time = 0.1f;
        float max_time = 0.7f;
        int MaxDistance = 600;

    }Aimbot;

    struct {

        struct {
            bool Enable = false;

            int MaxDistance = 150;
            float FontSize = 15.f;

            bool AssaultRifleAmmo = true;
            bool PistolAmmo = true;
            bool SMGAmmo = true;
            bool ShotgunAmmo = true;
            bool SniperAmmo = true;
            bool RocketAmmo = true;

            bool Cash = true;
            bool Armor = true;
            bool Grenades = true;

            bool Crates = true;
            bool Boxes = true;
            bool Missions = true;
            bool Perks = true;
            bool Knifes = true;
            bool Stim = true;
            bool DMZ = true;
        }Items;

        struct {
            bool Enable = true;

            int MaxDistance = 800;
            float FontSize = 16.f;

            bool Box = true;
            int BoxType = 0;
            float BoxThickness = 1.f;

            bool Lines = true;
            int LinesPosition = 0;
            float LinesThickness = 1.f;

            bool Health = true;
            int HealthType = 0;
            float HealthThickness = 4.f;

            bool Skeleton = true;
            float SkeletonThickness = 2.f;

            bool NickName = true;
            bool Distance = true;
            bool HeadCircle = true;

            bool OutScreen = true;
            int OutScreenType = 0;
            int OutScreenFov = 15;
            float OutScreenThickness = 2.f;
            int OutScreenSize = 10.f;

        }Players;

        struct {
            bool Enable = true;
            int PositionX = 40;
            int PositionY = 30;
            int Size = 200;
            int Zoom = 1;

            bool VisibleColor = true;
            int MaxDistance = 150;
        }Radar;
    }Visuals;

    struct {

        struct {
            int HoldPrimary = VK_LBUTTON;
            int HoldSecondary = VK_RBUTTON;

            int SwitchToHead = VK_SHIFT;
            int Toggle = VK_F6;
        }Aimbot;

        struct {
            int TogglePlayers = VK_F7;
            int ToggleBots = VK_F7;
            int ToggleItems = VK_F8;
            int ToggleRadar = VK_F9;
        }Visuals;

        int Menu = VK_INSERT;

    }Keys;

    struct {
        struct {
            float Fov[3] = { 0.f, 1.f, 0.f };
            float CrossHair[3] = { 1.f, 0.f, 0.f };
            float Target[3] = { 1.f, 1.f, 0.f };
        }Aimbot;

        struct {
            float Visible[3] = { 1.f, 1.f, 1.f };
            float NonVisible[3] = { 1.f, 1.f, 1.f };
            float NickName[3] = { 1.f, 1.f, 1.f };
            float Distance[3] = { 1.f, 1.f, 1.f };
        }Players;

        struct {
            float Visible[3] = { 1.f, 1.f, 1.f };
            float NonVisible[3] = { 1.f, 1.f, 1.f };
            float NickName[3] = { 1.f, 1.f, 1.f };
            float Distance[3] = { 1.f, 1.f, 1.f };
        }Bots;

        struct {
            float Enemies[3] = { 1.f, 0.f, 0.f };
            float Visible[3] = { 0.f, 1.0f, 0.f };
        }Radar;

    }Colors;

    struct {
        int Language = 0;
        bool FeaturesDefinition = true;
    }Configs;
};