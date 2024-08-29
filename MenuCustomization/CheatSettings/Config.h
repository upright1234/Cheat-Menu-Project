#pragma once
#include <Windows.h>
#include <string>
#include <vector>

#define IsApex 0
#define IsFortnite 0
#define IsMW3 0
#define IsRust 0
#define IsValorant 0
#define IsXDefiant 1

void InitPath(char *config_name = NULL);
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
};