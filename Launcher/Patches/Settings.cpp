#include "Settings.h"

Settings g_settings;

void Settings::LoadJson(const std::string& filename)
{
    std::ifstream jsonFile(filename);
    if (!jsonFile.is_open())
    {
        CreateDefaultJson();
        return;
    }

    json jsonData;
    try
    {
        jsonFile >> jsonData;
    }
    catch (const json::parse_error& e)
    {
        return;
    }

    jsonFile.close();

    bool updated = false;

    if (!jsonData.contains("WindowMode"))
    {
        jsonData["WindowMode"] = false;
        updated = true;
    }
    windowMode = jsonData["WindowMode"].get<bool>();

    if (!jsonData.contains("BasicStudio"))
    {
        jsonData["BasicStudio"] = false;
        updated = true;
    }
    BasicStudio = jsonData["BasicStudio"].get<bool>();

    if (!jsonData.contains("MultipleInstances"))
    {
        jsonData["MultipleInstances"] = false;
        updated = true;
    }
    MultipleInstances = jsonData["MultipleInstances"].get<bool>();

    if (!jsonData.contains("higherGPULimits"))
    {
        jsonData["higherGPULimits"] = false;
        updated = true;
    }
    higherGPULimits = jsonData["higherGPULimits"].get<bool>();

    if (!jsonData.contains("LuKeStormPatch"))
    {
        jsonData["LuKeStormPatch"] = false;
        updated = true;
    }
    LuKeStormPatch = jsonData["LuKeStormPatch"].get<bool>();

    if (!jsonData.contains("DoWeLockCursor"))
    {
        jsonData["DoWeLockCursor"] = true;
        updated = true;
    }
    DoWeLockCursor = jsonData["DoWeLockCursor"].get<bool>();

    if (!jsonData.contains("CursorLockTimer"))
    {
        jsonData["CursorLockTimer"] = 5;
        updated = true;
    }
    CursorLockTimer = jsonData["CursorLockTimer"].get<int>();

    if (updated)
    {
        std::ofstream outFile(filename);
        if (outFile.is_open())
        {
            outFile << jsonData.dump(4);
        }
    }
}

void Settings::CreateDefaultJson(const std::string& filename)
{
    std::ifstream infile(filename);
    if (infile.good())
    {
        return;
    }

    json defaultJson = {
        {"WindowMode", false},
        {"BasicStudio", false},
        {"MultipleInstances", false},
        {"LuKeStormPatch", false},
        {"DoWeLockCursor", true},
        {"CursorLockTimer", 5}
    };

    std::ofstream outFile(filename);
    if (!outFile.is_open())
    {

        return;
    }

    outFile << defaultJson.dump(4);
    outFile.close();
}
