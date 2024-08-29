#pragma once
#include "../CheatSettings/Games/ApexLegends.h"
#include "../CheatSettings/Games/Fortnite.h"
#include "../CheatSettings/Games/MW3.h"
#include "../CheatSettings/Games/Rust.h"
#include "../CheatSettings/Games/Valorant.h"
#include "../CheatSettings/Games/XDefiant.h"
#include "../CheatSettings/Games/PUBG.h"
#include "../CheatSettings/Config.h"
#include <d3d11.h>
#include "../Imgui/imgui.h"


extern void InitMyMenu(ID3D11Device* g_pd3dDevice);
extern void RenderMyMenu(bool* ShowMenu, SETTINGS* Settings, CConfig* g_pConfig);

inline ImFont* icons = 0;
inline ID3D11ShaderResourceView* logo = 0;