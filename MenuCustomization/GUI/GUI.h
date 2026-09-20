#pragma once
#include "../CheatSettings/Games/PUBG.h"
#include "../CheatSettings/Games/BO6.h"
#include "../CheatSettings/Games/BO7Ext.h"
#include "../CheatSettings/Games/BO7Int.h"
#include "../CheatSettings/Games/MarvelRivals.h"
#include "../CheatSettings/Games/MW19.h"
#include "../CheatSettings/Games/MW3.h"
#include "../CheatSettings/Games/Fortnite.h"
#include "../CheatSettings/Games/Apex.h"
#include "../CheatSettings/Games/Arc.h"
#include "../CheatSettings/Games/R6.h"
#include "../CheatSettings/Games/Rust.h"
#include "../CheatSettings/Games/WarDogs.h"
#include "../CheatSettings/Config.h"
#include <d3d11.h>
#include "../Imgui/imgui.h"


extern void InitMyMenu(ID3D11Device* g_pd3dDevice);
extern void RenderMyMenu(bool* ShowMenu, SETTINGS* Settings, CConfig* g_pConfig);