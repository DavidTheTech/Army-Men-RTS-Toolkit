#include "GameManager.h"
#include <tlhelp32.h>
#include <vector>
#include <cstdio>
#include <fstream>
#include <windows.h>
#include "json.hpp"

using json = nlohmann::ordered_json;

GameManager::GameManager()
    : m_windowMode(false)
    , m_basicStudio(false)
    , m_multiInst(false)
    , m_higherGPULimits(false)
    , m_luKeStormPatch(false)
{
}

GameManager::~GameManager()
{
}

bool GameManager::Launch(const char* exePath)
{
    m_lastError.clear();

    ApplyPatches();

    if (m_luKeStormPatch)
    {
        InstallLuKeStormPatch();
    }
    else
    {
        RemoveLuKeStormPatch();
    }

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi = {};

    if (!CreateProcessA(exePath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        char buf[256];
        sprintf_s(buf, "CreateProcess failed\n(error %lu)", GetLastError());
        m_lastError = buf;
        return false;
    }

    ResumeThread(pi.hThread);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return true;
}

/*bool GameManager::InjectDll(HANDLE hProcess, const char* dllPath)
{
    char fullPath[MAX_PATH];
    GetFullPathNameA(dllPath, MAX_PATH, fullPath, NULL);

    size_t pathSize = strlen(fullPath) + 1;
    LPVOID remoteMem = VirtualAllocEx(hProcess, NULL, pathSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!remoteMem)
    {
        return false;
    }

    if (!WriteProcessMemory(hProcess, remoteMem, fullPath, pathSize, NULL))
    {
        VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
        return false;
    }

    HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
    LPTHREAD_START_ROUTINE loadLib = (LPTHREAD_START_ROUTINE)GetProcAddress(hKernel32, "LoadLibraryA");

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, loadLib, remoteMem, 0, NULL);
    if (!hThread)
    {
        VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);
        return false;
    }

    WaitForSingleObject(hThread, INFINITE);

    DWORD exitCode = 0;
    GetExitCodeThread(hThread, &exitCode);

    CloseHandle(hThread);
    VirtualFreeEx(hProcess, remoteMem, 0, MEM_RELEASE);

    return exitCode != 0;
}*/

bool GameManager::ApplyPatches()
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

    json config;

    std::ifstream inFile(jsonPath);
    if (inFile.is_open())
    {
        try
        {
            inFile >> config;
        }
        catch (const json::parse_error&)
        {
        }
        inFile.close();
    }

    config["WindowMode"] = m_windowMode;
    config["BasicStudio"] = m_basicStudio;
    config["MultipleInstances"] = m_multiInst;
    config["higherGPULimits"] = m_higherGPULimits;
    config["LuKeStormPatch"] = m_luKeStormPatch;

    if (!config.contains("DoWeLockCursor"))
    {
        config["DoWeLockCursor"] = true;
    }
    if (!config.contains("CursorLockTimer"))
    {
        config["CursorLockTimer"] = 5;
    }

    std::ofstream outFile(jsonPath);
    if (outFile.is_open())
    {
        outFile << config.dump(4);
        outFile.close();
    }

    return true;
}

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

void GameManager::InstallLuKeStormPatch()
{
    std::string base = GetLauncherDir();
    std::string src = base + "Launcher\\Patches\\LuKeStorm\\patch\\";

    printf("%s\n", src.c_str());
    const char* files[] = { "ddraw.dll", "dinput.dll", "dxcfg.ini", "winmm.dll", "custompatch.ini"};

    for (const char* f : files)
    {
        std::string s = src + f;
        std::string d = base + f;
        CopyFileA(s.c_str(), d.c_str(), FALSE);
    }
}

void GameManager::RemoveLuKeStormPatch()
{
    std::string base = GetLauncherDir();
    const char* files[] = { "ddraw.dll", "dinput.dll", "dxcfg.ini", "winmm.dll" };

    for (const char* f : files)
    {
        std::string p = base + f;
        DeleteFileA(p.c_str());
    }
}