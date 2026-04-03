#include "Settings.h"
#include "GameFuncs/system/Log.h"

Settings g_settings;

void Settings::LoadJson(const std::string& filename)
{
    std::ifstream jsonFile(filename);
    if (!jsonFile.is_open())
    {
        Log::Client::Write("[STUDIO DLL: Settings]: Failed to open file: %s\tCreating default", filename.c_str());
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
        Log::Client::Write("[STUDIO DLL: Settings]: JSON parse error");
        return;
    }

    jsonFile.close();

    bool updated = false;

    if (!jsonData.contains("DoWeLockCursor"))
    {
        jsonData["DoWeLockCursor"] = true;
        updated = true;
    }
    DoWeLockCursor = jsonData["DoWeLockCursor"].get<bool>();

    if (!jsonData.contains("ShowConsole"))
    {
        jsonData["ShowConsole"] = true;
        updated = true;
    }
    ShowConsole = jsonData["ShowConsole"].get<bool>();

    if (!jsonData.contains("CursorLockTimer"))
    {
        jsonData["CursorLockTimer"] = 5;
        updated = true;
    }
    CursorLockTimer = jsonData["CursorLockTimer"].get<int>();

    if (!jsonData.contains("AutoLaunchStudio"))
    {
        jsonData["AutoLaunchStudio"] = false;
        updated = true;
    }
    AutoLaunch = jsonData["AutoLaunchStudio"].get<bool>();

    if (!jsonData.contains("InitBucketsMultiplier"))
    {
        jsonData["InitBucketsMultiplier"] = 4;
        updated = true;
    }
    InitBucketMulti = jsonData["InitBucketsMultiplier"].get<int>();

    if (!jsonData.contains("InitBucketsVertices"))
    {
        jsonData["InitBucketsVertices"] = 1450;
        updated = true;
    }
    InitBucketVert = jsonData["InitBucketsVertices"].get<int>();

    if (!jsonData.contains("InitBucketsIndices"))
    {
        jsonData["InitBucketsIndices"] = 4350;
        updated = true;
    }
    InitBucketIndi = jsonData["InitBucketsIndices"].get<int>();

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
        {"DoWeLockCursor", true},
        {"ShowConsole", true},
        {"CursorLockTimer", 5},
        {"AutoLaunchStudio", false},
        {"InitBucketsMultiplier", 4},
        {"InitBucketsVertices", 1450},
        {"InitBucketsIndices", 4350}
    };

    std::ofstream outFile(filename);
    if (!outFile.is_open())
    {
        Log::Client::Write("[STUDIO DLL: Settings]: Failed to create default settings.json");
        return;
    }

    outFile << defaultJson.dump(4);
    outFile.close();

    Log::Client::Write("[STUDIO DLL: Settings]: Default settings.json created.");
}
