#pragma once
#include <Windows.h>
#include <string>
#include <vector>

// Active game: set exactly one to 1. The #ifndef guards let the preview build
// (Preview\preview.ps1 -Game X) pick a game from the command line; the DLL build uses these defaults.
#ifndef IsApex
#define IsApex 0
#endif
#ifndef IsArc
#define IsArc 0
#endif
#ifndef IsFortnite
#define IsFortnite 0
#endif
#ifndef IsBO6
#define IsBO6 0
#endif
#ifndef IsBO7Int
#define IsBO7Int 0
#endif
#ifndef IsBO7Ext
#define IsBO7Ext 0
#endif
#ifndef IsMarvel
#define IsMarvel 0
#endif
#ifndef IsMW19
#define IsMW19 0
#endif
#ifndef IsMW3
#define IsMW3 0
#endif
#ifndef IsR6
#define IsR6 0
#endif
#ifndef IsRust
#define IsRust 0
#endif
#ifndef IsValorant
#define IsValorant 0
#endif
#ifndef IsPUBG
#define IsPUBG 0
#endif
#ifndef IsWarDogs
#define IsWarDogs 1
#endif

void InitPath(char* config_name = NULL);
void CreateConfig(char* config_name = NULL);
void Read();
void Save();
void ParseConfigs();

struct CConfig
{
    decltype (&InitPath)(InitPath);
    decltype (&Read)(Read);
    decltype (&Save)(Save);
    decltype (&ParseConfigs)(ParseConfigs);
    std::string szConfigs;
    std::vector<std::string> vecConfigs;
    std::string pathname;
    decltype (&CreateConfig)(CreateConfig);
};
