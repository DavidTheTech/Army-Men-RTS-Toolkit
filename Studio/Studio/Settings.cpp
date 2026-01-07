#include "Settings.h"
#include "GameFuncs/system/Log.h"

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

    if (jsonData.contains("DoWeLockCursor"))
    {
        DoWeLockCursor = jsonData["DoWeLockCursor"].get<bool>();
    }

    if (jsonData.contains("ShowConsole"))
    {
        ShowConsole = jsonData["ShowConsole"].get<bool>();
    }

    if (jsonData.contains("CursorLockTimer"))
    {
        CursorLockTimer = jsonData["CursorLockTimer"].get<int>();
    }

    if (jsonData.contains("AutoLaunchStudio"))
    {
        AutoLaunch = jsonData["AutoLaunchStudio"].get<bool>();
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
        {"DoWeLockCursor", false},
        {"ShowConsole", true},
        {"CursorLockTimer", 5},
        {"AutoLaunchStudio", false}
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
