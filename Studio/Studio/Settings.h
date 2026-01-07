#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <json.hpp>

using json = nlohmann::json;

class Settings
{
public:
    bool DoWeLockCursor = false;
    bool ShowConsole = false;
    int CursorLockTimer = 0;
    bool AutoLaunch = false;

    void LoadJson(const std::string& filename = "settings.json");
    void CreateDefaultJson(const std::string& filename = "settings.json");
};
