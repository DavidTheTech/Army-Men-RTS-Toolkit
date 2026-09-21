#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "json.hpp"
#include <algorithm>

using json = nlohmann::ordered_json;

class Settings
{
public:
    bool windowMode = false;
    bool BasicStudio = false;
    bool MultipleInstances = false;
    bool higherGPULimits = false;

    bool LuKeStormPatch = false;

    bool DoWeLockCursor = false;
    int CursorLockTimer = 0;

    //initbuckets
    int InitBucketMulti = 4;
    int InitBucketVert = 1450;
    int InitBucketIndi = 4350;

    void LoadJson(const std::string& filename = "Launcher.json");
    void CreateDefaultJson(const std::string& filename = "Launcher.json");
};

extern Settings g_settings;