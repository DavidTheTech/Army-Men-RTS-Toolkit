#include "GameLauncher.h"
#include "GameManager.h"
#include "logo_data.h"
#include <commdlg.h>
#include <cstdarg>
#include <cstdio>
#include <shellapi.h>
#include <thread>
#include <fstream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "json.hpp"

using json = nlohmann::ordered_json;

static std::string GetLauncherDir()
{
    char modulePath[MAX_PATH];
    GetModuleFileNameA(NULL, modulePath, MAX_PATH);
    char* lastSlash = strrchr(modulePath, '\\');
    if (lastSlash)
    {
        *(lastSlash + 1) = '\0';
    }
    return std::string(modulePath);
}

GameLauncher::GameLauncher(ID3D11Device* device, HWND hwnd_)
    : d3dDevice(device)
    , hwnd(hwnd_)
    , logoTexture(nullptr)
    , tweak1Enabled(false)
    , tweak2Enabled(false)
    , tweak3Enabled(false)
    , tweak4Enabled(false)
    , tweak5Enabled(false)
    , tweakLuKeStormEnabled(false)
    , showAboutPopup(false)
    , showLuKeStorm(false)
    , showLuKeSettings(false)
    , lukAltTabFix(true)
    , lukEnableAltF4(true)
    , lukHigherResolutions(true)
    , lukMPAllowStartWithAIOnly(true)
    , lukMultipleInstances(true)
    , lukNoErrorDialog(false)
    , lukNoIntro(true)
    , lukNoLogfile(true)
    , lukStartupInitHelper(true)
{
    LoadLogoTexture(logo_png, logo_png_len);
    LoadSettings();
    LoadLuKeStormIni();
}

GameLauncher::~GameLauncher()
{
    if (logoTexture)
    {
        logoTexture->Release();
    }
}

void GameLauncher::Log(const char* fmt, ...)
{
    char buffer[512];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    logLines.push_back(std::string(buffer));
    if (logLines.size() > MAX_LOG_LINES)
    {
        logLines.pop_front();
    }
}

bool GameLauncher::LoadLogoTexture(const unsigned char* data, size_t size)
{
    int width, height, channels;
    unsigned char* decoded = stbi_load_from_memory(data, (int)size, &width, &height, &channels, 4);
    if (!decoded)
    {
        return false;
    }

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = decoded;
    initData.SysMemPitch = width * 4;

    ID3D11Texture2D* texture = nullptr;
    HRESULT hr = d3dDevice->CreateTexture2D(&desc, &initData, &texture);
    stbi_image_free(decoded);
    if (FAILED(hr))
    {
        return false;
    }

    hr = d3dDevice->CreateShaderResourceView(texture, nullptr, &logoTexture);
    texture->Release();
    return SUCCEEDED(hr);
}

void GameLauncher::SetupTheme()
{
    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowPadding = ImVec2(8, 8);
    style.FramePadding = ImVec2(6, 4);
    style.ItemSpacing = ImVec2(6, 6);
    style.ItemInnerSpacing = ImVec2(4, 4);
    style.TouchExtraPadding = ImVec2(0, 0);
    style.IndentSpacing = 21.0f;
    style.ScrollbarSize = 15.0f;
    style.GrabMinSize = 10.0f;

    style.WindowBorderSize = 1.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupBorderSize = 1.0f;
    style.FrameBorderSize = 1.0f;
    style.TabBorderSize = 0.0f;

    style.WindowRounding = 4.0f;
    style.ChildRounding = 4.0f;
    style.FrameRounding = 3.0f;
    style.PopupRounding = 3.0f;
    style.ScrollbarRounding = 3.0f;
    style.GrabRounding = 3.0f;
    style.TabRounding = 3.0f;

    ImVec4 olive(0.35f, 0.45f, 0.20f, 1.00f);
    ImVec4 khaki(0.55f, 0.50f, 0.30f, 1.00f);
    ImVec4 tan(0.70f, 0.60f, 0.40f, 1.00f);
    ImVec4 darkOlive(0.15f, 0.20f, 0.10f, 1.00f);
    ImVec4 lightKhaki(0.75f, 0.70f, 0.50f, 1.00f);

    style.Colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.85f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.40f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = darkOlive;
    style.Colors[ImGuiCol_ChildBg] = darkOlive;
    style.Colors[ImGuiCol_PopupBg] = darkOlive;
    style.Colors[ImGuiCol_Border] = khaki;
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = olive;
    style.Colors[ImGuiCol_FrameBgHovered] = khaki;
    style.Colors[ImGuiCol_FrameBgActive] = tan;
    style.Colors[ImGuiCol_TitleBg] = olive;
    style.Colors[ImGuiCol_TitleBgActive] = khaki;
    style.Colors[ImGuiCol_TitleBgCollapsed] = olive;
    style.Colors[ImGuiCol_MenuBarBg] = olive;
    style.Colors[ImGuiCol_ScrollbarBg] = darkOlive;
    style.Colors[ImGuiCol_ScrollbarGrab] = olive;
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = khaki;
    style.Colors[ImGuiCol_ScrollbarGrabActive] = tan;
    style.Colors[ImGuiCol_CheckMark] = tan;
    style.Colors[ImGuiCol_SliderGrab] = olive;
    style.Colors[ImGuiCol_SliderGrabActive] = khaki;
    style.Colors[ImGuiCol_Button] = olive;
    style.Colors[ImGuiCol_ButtonHovered] = khaki;
    style.Colors[ImGuiCol_ButtonActive] = tan;
    style.Colors[ImGuiCol_Header] = olive;
    style.Colors[ImGuiCol_HeaderHovered] = khaki;
    style.Colors[ImGuiCol_HeaderActive] = tan;
    style.Colors[ImGuiCol_Separator] = khaki;
    style.Colors[ImGuiCol_SeparatorHovered] = tan;
    style.Colors[ImGuiCol_SeparatorActive] = lightKhaki;
    style.Colors[ImGuiCol_ResizeGrip] = olive;
    style.Colors[ImGuiCol_ResizeGripHovered] = khaki;
    style.Colors[ImGuiCol_ResizeGripActive] = tan;
    style.Colors[ImGuiCol_Tab] = olive;
    style.Colors[ImGuiCol_TabHovered] = khaki;
    style.Colors[ImGuiCol_TabActive] = tan;
    style.Colors[ImGuiCol_TabUnfocused] = olive;
    style.Colors[ImGuiCol_TabUnfocusedActive] = olive;
    style.Colors[ImGuiCol_PlotLines] = tan;
    style.Colors[ImGuiCol_PlotLinesHovered] = lightKhaki;
    style.Colors[ImGuiCol_PlotHistogram] = tan;
    style.Colors[ImGuiCol_PlotHistogramHovered] = lightKhaki;
    style.Colors[ImGuiCol_TableHeaderBg] = olive;
    style.Colors[ImGuiCol_TableBorderStrong] = khaki;
    style.Colors[ImGuiCol_TableBorderLight] = olive;
    style.Colors[ImGuiCol_TableRowBg] = darkOlive;
    style.Colors[ImGuiCol_TableRowBgAlt] = olive;
    style.Colors[ImGuiCol_TextSelectedBg] = khaki;
    style.Colors[ImGuiCol_DragDropTarget] = tan;
    style.Colors[ImGuiCol_NavHighlight] = tan;
    style.Colors[ImGuiCol_NavWindowingHighlight] = tan;
    style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.10f, 0.10f, 0.05f, 0.80f);
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.10f, 0.10f, 0.05f, 0.80f);
}

void GameLauncher::LoadSettings()
{
    char jsonPath[MAX_PATH];
    char modulePath[MAX_PATH];
    GetModuleFileNameA(NULL, modulePath, MAX_PATH);
    char* lastSlash = strrchr(modulePath, '\\');
    if (lastSlash)
    {
        *(lastSlash + 1) = '\0';
    }
    strcpy_s(jsonPath, modulePath);
    strcat_s(jsonPath, "Launcher.json");

    std::ifstream inFile(jsonPath);
    if (!inFile.is_open())
    {
        return;
    }

    json config;
    try
    {
        inFile >> config;
    }
    catch (const json::parse_error&)
    {
        return;
    }
    inFile.close();

    if (config.contains("WindowMode"))
    {
        tweak1Enabled = config["WindowMode"].get<bool>();
    }
    if (config.contains("DoWeLockCursor"))
    {
        tweak5Enabled = config["DoWeLockCursor"].get<bool>();
    }
    if (config.contains("BasicStudio"))
    {
        tweak2Enabled = config["BasicStudio"].get<bool>();
    }
    if (config.contains("MultipleInstances"))
    {
        tweak3Enabled = config["MultipleInstances"].get<bool>();
    }
    if (config.contains("higherGPULimits"))
    {
        tweak4Enabled = config["higherGPULimits"].get<bool>();
    }
    if (config.contains("LuKeStormPatch"))
    {
        tweakLuKeStormEnabled = config["LuKeStormPatch"].get<bool>();
    }
}

void GameLauncher::LoadLuKeStormIni()
{
    std::string path = "custompatch.ini";
    const char* p = path.c_str();

    lukAltTabFix = GetPrivateProfileIntA("Patch", "AltTabFix", 1, p) != 0;
    lukEnableAltF4 = GetPrivateProfileIntA("Patch", "EnableAltF4", 1, p) != 0;
    lukHigherResolutions = GetPrivateProfileIntA("Patch", "HigherResolutions", 1, p) != 0;
    lukMPAllowStartWithAIOnly = GetPrivateProfileIntA("Patch", "MPAllowStartWithAIOnly", 1, p) != 0;
    lukMultipleInstances = GetPrivateProfileIntA("Patch", "MultipleInstances", 1, p) != 0;
    lukNoErrorDialog = GetPrivateProfileIntA("Patch", "NoErrorDialog", 0, p) != 0;
    lukNoIntro = GetPrivateProfileIntA("Patch", "NoIntro", 1, p) != 0;
    lukNoLogfile = GetPrivateProfileIntA("Patch", "NoLogfile", 1, p) != 0;
    lukStartupInitHelper = GetPrivateProfileIntA("Patch", "StartupInitHelper", 1, p) != 0;
}

void GameLauncher::SaveLuKeStormIni()
{
    std::string path = "custompatch.ini";
    const char* p = path.c_str();

    WritePrivateProfileStringA("Patch", "AltTabFix", lukAltTabFix ? "1" : "0", p);
    WritePrivateProfileStringA("Patch", "EnableAltF4", lukEnableAltF4 ? "1" : "0", p);
    WritePrivateProfileStringA("Patch", "HigherResolutions", lukHigherResolutions ? "1" : "0", p);
    WritePrivateProfileStringA("Patch", "MPAllowStartWithAIOnly", lukMPAllowStartWithAIOnly ? "1" : "0", p);
    WritePrivateProfileStringA("Patch", "MultipleInstances", lukMultipleInstances ? "1" : "0", p);
    WritePrivateProfileStringA("Patch", "NoErrorDialog", lukNoErrorDialog ? "1" : "0", p);
    WritePrivateProfileStringA("Patch", "NoIntro", lukNoIntro ? "1" : "0", p);
    WritePrivateProfileStringA("Patch", "NoLogfile", lukNoLogfile ? "1" : "0", p);
    WritePrivateProfileStringA("Patch", "StartupInitHelper", lukStartupInitHelper ? "1" : "0", p);
}

void GameLauncher::LaunchGame()
{
    GameManager gm;
    gm.SetWindowMode(tweak1Enabled);
    gm.SetCursorLock(tweak5Enabled);
    gm.SetBasicStudio(tweak2Enabled);
    gm.SetMultipleInstances(tweak3Enabled);
    gm.SetHigherGPULimits(tweak4Enabled);
    gm.SetLuKeStormPatch(tweakLuKeStormEnabled);

    if (gm.Launch("amrts.exe"))
    {
        Log("Tweaks applied:\nWindowMode=%s\nMouseLock=%s\nBasicStudio=%s\nMultiInst=%s\nHigherGPU=%s\nLuKeStorm's Patch=%s",
            tweak1Enabled ? "ON" : "OFF",
            tweak5Enabled ? "ON" : "OFF",
            tweak2Enabled ? "ON" : "OFF",
            tweak3Enabled ? "ON" : "OFF",
            tweak4Enabled ? "ON" : "OFF",
            tweakLuKeStormEnabled ? "ON" : "OFF");

        Log("Game launched successfully.\n");
    }
    else
    {
        Log("Failed to launch: %s\n", gm.GetLastError().c_str());
    }
}

//Dante Martel's jokes
void GameLauncher::LoadJoke()
{
    /*Log("Joke of the day\n");
    Log("Dad: Knock knock!\n"
        "Child: Who's there?\n"
        "Dad : Hike\n"
        "Child : Hike who ?\n"
        "Dad : Unsuspecting child\n"
        "Dad waiting with bated breath\n"
        "Sets the perfect trap\n");*/
}

void GameLauncher::Render()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    if (ImGui::Begin("AMRTSL", nullptr, flags))
    {
        ImVec2 winSize = ImGui::GetWindowSize();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.00f, 0.09f, 0.39f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.00f, 0.20f, 0.60f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.00f, 0.30f, 0.70f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.63f, 0.69f, 0.78f, 1.00f));

        ImGui::SetCursorPosX(10);
        ImGui::SetCursorPosY(4);
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.00f), "Army Men RTS Launcher");

        ImGui::SameLine(winSize.x - 95);

        ImGui::PushID("about");
        if (ImGui::Button("?##aboutbtn", ImVec2(30, 32)))
        {
            showAboutPopup = true;
        }
        ImGui::PopID();

        ImGui::SameLine(0, 0);
        ImGui::PushID("minimize");
        if (ImGui::Button("_", ImVec2(30, 32)))
        {
            ShowWindow(hwnd, SW_MINIMIZE);
        }
        ImGui::PopID();

        ImGui::SameLine(0, 0);
        ImGui::PushID("close");
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.2f, 0.2f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 0.8f));
        if (ImGui::Button("X", ImVec2(30, 32)))
        {
            PostQuitMessage(0);
        }
        ImGui::PopStyleColor(2);
        ImGui::PopID();

        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar();

        float titleHeight = 32.0f;
        float contentY = titleHeight + 10.0f;
        ImGui::SetCursorPosY(contentY);

        ImGui::BeginChild("ContentArea", ImVec2(0, winSize.y - contentY - 10), false, ImGuiWindowFlags_NoScrollbar);

        if (logoTexture)
        {
            float availWidth = ImGui::GetContentRegionAvail().x;
            float logoWidth = availWidth;
            float logoHeight = logoWidth * (272.0f / 640.0f);
            ImGui::Image((ImTextureID)logoTexture, ImVec2(logoWidth, logoHeight));
        }

        ImGui::Separator();

        ImGui::Checkbox("Window mode", &tweak1Enabled);
        ImGui::Checkbox("Mouse lock", &tweak5Enabled);
        ImGui::Checkbox("Basic Studio", &tweak2Enabled);
        ImGui::Checkbox("Multiple Instances", &tweak3Enabled);
        ImGui::Checkbox("Higher GPU limits", &tweak4Enabled);
        ImGui::Checkbox("LuKeStorm's Patch", &tweakLuKeStormEnabled);

        float winWidth = ImGui::GetWindowContentRegionMax().x;
        ImGui::SameLine();
        ImGui::SetCursorPosX(winWidth - 55.0f);
        if (ImGui::Button("Credits##lukestormbtn", ImVec2(55, 28)))
        {
            showLuKeStorm = true;
        }

        ImGui::SameLine();
        ImGui::SetCursorPosX(winWidth - 120.0f);
        if (ImGui::Button("Settings##lukestormcfg", ImVec2(60, 28)))
        {
            showLuKeSettings = true;
        }

        ImGui::Separator();

        ImGui::BeginChild("LogArea", ImVec2(0, 190), true);
        for (const auto& line : logLines)
        {
            ImGui::TextColored(ImVec4(0.63f, 0.69f, 0.78f, 1.00f), "%s", line.c_str());
        }
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        {
            ImGui::SetScrollHereY(1.0f);
        }
        ImGui::EndChild();

        ImGui::Separator();

        float remainingHeight = ImGui::GetContentRegionAvail().y - 10.0f;
        if (remainingHeight > 30.0f)
        {
            if (ImGui::Button("Play", ImVec2(-1.0f, remainingHeight)))
            {
                LaunchGame();
            }
        }
        else
        {
            if (ImGui::Button("Play", ImVec2(-1.0f, 40.0f)))
            {
                LaunchGame();
            }
        }

        ImGui::EndChild();

        if (showAboutPopup)
        {
            ImGui::OpenPopup("Army Men RTS Launcher##About");
        }

        if (showLuKeStorm)
        {
            ImGui::OpenPopup("About##LuKeStorm");
        }

        if (showLuKeSettings)
        {
            ImGui::OpenPopup("LuKeStorm Settings");
        }

        if (ImGui::BeginPopupModal("About##LuKeStorm", &showLuKeStorm, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Created by LuKeStorm");
            ImGui::Separator();
            if (ImGui::Button("Close"))
            {
                showLuKeStorm = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        if (ImGui::BeginPopupModal("LuKeStorm Settings", &showLuKeSettings, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Checkbox("Alt+Tab Fix", &lukAltTabFix);
            ImGui::Checkbox("Enable Alt+F4", &lukEnableAltF4);
            ImGui::Checkbox("Higher Resolutions", &lukHigherResolutions);
            ImGui::Checkbox("MP Allow Start With AI Only", &lukMPAllowStartWithAIOnly);
            ImGui::Checkbox("Multiple Instances", &lukMultipleInstances);
            ImGui::Checkbox("No ErrorDialog", &lukNoErrorDialog);
            ImGui::Checkbox("No Intro", &lukNoIntro);
            ImGui::Checkbox("No Logfile", &lukNoLogfile);
            ImGui::Checkbox("Startup Init Helper", &lukStartupInitHelper);

            ImGui::Separator();
            if (ImGui::Button("Save"))
            {
                SaveLuKeStormIni();
                showLuKeSettings = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
            {
                showLuKeSettings = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        if (ImGui::BeginPopupModal("Army Men RTS Launcher##About", &showAboutPopup, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Part of the Army Men RTS Toolkit\n\n");

            ImGui::Text("With Credits to");
            ImGui::BeginChild("Credits", ImVec2(300, 200), true);

            ImGui::Text("LuKeStorm for the custom patch\n");
            ImGui::Text("And the whole Army Men RTS community <3");

            ImGui::EndChild();

            ImGui::Text("Created by DavidTheTech");
            ImGui::Separator();
            if (ImGui::Button("Discord"))
            {
                ShellExecuteA(hwnd, "open", "https://discord.gg/nHbDxzV8ej", NULL, NULL, SW_SHOW);
            }
            ImGui::SameLine();
            if (ImGui::Button("Close"))
            {
                showAboutPopup = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        ImGui::End();
    }
}