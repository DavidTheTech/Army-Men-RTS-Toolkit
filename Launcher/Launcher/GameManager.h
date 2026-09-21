#pragma once
#include <windows.h>
#include <string>

class GameManager
{
public:
    GameManager();
    ~GameManager();

    void SetWindowMode(bool enable) { m_windowMode = enable; }
    void SetCursorLock(bool enable) { m_cursorLock = enable; }
    void SetBasicStudio(bool enable) { m_basicStudio = enable; }
    void SetMultipleInstances(bool enable) { m_multiInst = enable; }
    void SetHigherGPULimits(bool enable) { m_higherGPULimits = enable; }
    void SetLuKeStormPatch(bool enable) { m_luKeStormPatch = enable; }

    bool Launch(const char* exePath);
    std::string GetLastError() const { return m_lastError; }

    void InstallLuKeStormPatch();
    void RemoveLuKeStormPatch();

private:
    bool ApplyPatches();
    bool InjectDll(HANDLE hProcess, const char* dllPath);

    bool m_windowMode;
    bool m_cursorLock;
    bool m_basicStudio;
    bool m_multiInst;
    bool m_higherGPULimits;
    bool m_luKeStormPatch;
    std::string m_lastError;
};