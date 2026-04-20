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

    if (!jsonData.contains("Team"))
    {
        jsonData["Team"]["GetRelationColor"]["ally"] = { {"r", 0}, {"g", 255}, {"b", 0}, {"a", 255} };
        jsonData["Team"]["GetRelationColor"]["enemy"] = { {"r", 255}, {"g", 0}, {"b", 0}, {"a", 255} };
        jsonData["Team"]["GetRelationColor"]["neutral"] = { {"r", 0}, {"g", 255}, {"b", 255}, {"a", 255} };
        updated = true;
    }
    auto& teamColors = jsonData["Team"]["GetRelationColor"];
    allyColor.r = teamColors["ally"]["r"].get<int>();
    allyColor.g = teamColors["ally"]["g"].get<int>();
    allyColor.b = teamColors["ally"]["b"].get<int>();
    allyColor.a = teamColors["ally"]["a"].get<int>();

    enemyColor.r = teamColors["enemy"]["r"].get<int>();
    enemyColor.g = teamColors["enemy"]["g"].get<int>();
    enemyColor.b = teamColors["enemy"]["b"].get<int>();
    enemyColor.a = teamColors["enemy"]["a"].get<int>();

    neutralColor.r = teamColors["neutral"]["r"].get<int>();
    neutralColor.g = teamColors["neutral"]["g"].get<int>();
    neutralColor.b = teamColors["neutral"]["b"].get<int>();
    neutralColor.a = teamColors["neutral"]["a"].get<int>();

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
        {"InitBucketsIndices", 4350},
        {"Team", {
            {"GetRelationColor", {
                {"ally", {{"r", 0}, {"g", 255}, {"b", 0}, {"a", 255}}},
                {"enemy", {{"r", 255}, {"g", 0}, {"b", 0}, {"a", 255}}},
                {"neutral", {{"r", 0}, {"g", 255}, {"b", 255}, {"a", 255}}}
            }}
        }}
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
