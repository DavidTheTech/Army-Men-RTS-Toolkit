#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <json.hpp>

using json = nlohmann::ordered_json;

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

    void LoadJson(const std::string& filename = "settings.json");
    void CreateDefaultJson(const std::string& filename = "settings.json");
};

extern Settings g_settings;