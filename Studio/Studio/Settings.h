#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <json.hpp>

using json = nlohmann::ordered_json;

struct ColorConfig
{
    int r = 0, g = 0, b = 0, a = 255;
};

class Settings
{
public:
    bool DoWeLockCursor = false;
    bool ShowConsole = false;
    int CursorLockTimer = 0;
    bool AutoLaunch = false;

    //initbuckets
    int InitBucketMulti = 4;
    int InitBucketVert = 1450;
    int InitBucketIndi = 4350;

    // Team colors
    ColorConfig allyColor;
    ColorConfig enemyColor;
    ColorConfig neutralColor;

    void LoadJson(const std::string& filename = "settings.json");
    void CreateDefaultJson(const std::string& filename = "settings.json");
};

extern Settings g_settings;