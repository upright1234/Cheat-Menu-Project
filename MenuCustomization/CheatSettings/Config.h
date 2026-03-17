#pragma once
#include <Windows.h>
#include <string>
#include <vector>

#define IsApex 0
#define IsArc 
#define IsFortnite 0
#define IsBO6 0
#define IsMW3 0
#define IsRust 1
#define IsValorant 0
#define IsPUBG 0

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