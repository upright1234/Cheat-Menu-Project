#pragma once
#include <Windows.h>
#include <string>
#include <vector>

struct CConfig {
    std::string szConfigs;
    std::vector<std::string> vecConfigs;
    std::string pathname;

    void InitPath(char* config_name = NULL);
    void Read();
    void Save();
    void ParseConfigs();
};