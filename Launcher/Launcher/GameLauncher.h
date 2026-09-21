#pragma once
#include "IMGUI/imgui.h"
#include <d3d11.h>
#include <windows.h>
#include <string>
#include <deque>

class GameLauncher
{
public:
    GameLauncher(ID3D11Device* device, HWND hwnd);
    ~GameLauncher();

    void SetupTheme();
    void Render();
    void LoadJoke();

private:
    void Log(const char* fmt, ...);
    void LaunchGame();
    bool LoadLogoTexture(const unsigned char* data, size_t size);
    void LoadSettings();
    void LoadLuKeStormIni();
    void SaveLuKeStormIni();

    HWND hwnd;
    ID3D11Device* d3dDevice;
    ID3D11ShaderResourceView* logoTexture;

    bool tweak1Enabled;
    bool tweak2Enabled;
    bool tweak3Enabled;
    bool tweak4Enabled;
    bool tweak5Enabled;
    bool tweakLuKeStormEnabled;

    bool showAboutPopup;
    bool showLuKeStorm;
    bool showLuKeSettings;

    bool lukAltTabFix;
    bool lukEnableAltF4;
    bool lukHigherResolutions;
    bool lukMPAllowStartWithAIOnly;
    bool lukMultipleInstances;
    bool lukNoErrorDialog;
    bool lukNoIntro;
    bool lukNoLogfile;
    bool lukStartupInitHelper;

    std::deque<std::string> logLines;
    static constexpr int MAX_LOG_LINES = 100;
};