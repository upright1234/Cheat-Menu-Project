#pragma once
#include <Windows.h>
#include <string>
#include <vector>

struct CConfig {
    std::string szConfigs;
    std::vector<std::string> vecConfigs;
    std::string pathname;

    virtual void InitPath(char* config_name = NULL);
    virtual void Read();
    virtual void Save();
    virtual void ParseConfigs();
};