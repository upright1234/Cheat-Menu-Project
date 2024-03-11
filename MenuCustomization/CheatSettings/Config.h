#pragma once
#include <Windows.h>
#include <string>
#include <vector>

void InitPath(char* config_name = NULL);
void Read();
void Save();
void ParseConfigs();

struct CConfig {
    decltype(&InitPath)(InitPath);
    decltype(&Read)(Read);
    decltype(&Save)(Save);
    decltype(&ParseConfigs)(ParseConfigs);
    std::string szConfigs;
    std::vector<std::string> vecConfigs;
    std::string pathname;
};