#include "GameTime.h"

typedef void(__thiscall* GameTime_Pause_t)(void* thisptr, bool triggerMessage);
typedef bool(__cdecl* GameTime_Paused_t)();
typedef void(__cdecl* GameTime_StepOnce_t)();
typedef void(__cdecl* GameTime_Readjust_t)();
typedef void(__cdecl* GameTime_Synchronize_t)();
typedef void(__cdecl* GameTime_SetFastMode_t)(bool flag);
typedef bool(__cdecl* GameTime_GetFastMode_t)();
typedef bool(__cdecl* GameTime_GetFastForwardMode_t)();
typedef void(__cdecl* GameTime_SetDisplayMode_t)(bool flag);
typedef bool(__cdecl* GameTime_GetDisplayMode_t)();
typedef float(__cdecl* GameTime_TimeSinceCycle_t)(unsigned long cycle);
typedef void(__cdecl* GameTime_IncGameCycle_t)();

static GameTime_Pause_t GameTime_Pause_Fn = (GameTime_Pause_t)(Memory::ScanAddress(0x40FF20));
static GameTime_Paused_t GameTime_Paused_Fn = (GameTime_Paused_t)(Memory::ScanAddress(0x40FF90));
static GameTime_StepOnce_t GameTime_StepOnce_Fn = (GameTime_StepOnce_t)(Memory::ScanAddress(0x40FFA0));
static GameTime_Readjust_t GameTime_Readjust_Fn = (GameTime_Readjust_t)(Memory::ScanAddress(0x40FFC0));
static GameTime_Synchronize_t GameTime_Synchronize_Fn = (GameTime_Synchronize_t)(Memory::ScanAddress(0x410000));
static GameTime_SetFastMode_t SetFastMode_Fn = (GameTime_SetFastMode_t)(Memory::ScanAddress(0x410010));
static GameTime_GetFastMode_t GetFastMode_Fn = (GameTime_GetFastMode_t)(Memory::ScanAddress(0x410020));
static GameTime_GetFastForwardMode_t GetFastForwardMode_Fn = (GameTime_GetFastForwardMode_t)(Memory::ScanAddress(0x410030));
static GameTime_SetDisplayMode_t SetDisplayMode_Fn = (GameTime_SetDisplayMode_t)(Memory::ScanAddress(0x410040));
static GameTime_GetDisplayMode_t GetDisplayMode_Fn = (GameTime_GetDisplayMode_t)(Memory::ScanAddress(0x410050));
static GameTime_TimeSinceCycle_t TimeSinceCycle_Fn = (GameTime_TimeSinceCycle_t)(Memory::ScanAddress(0x410060));
static GameTime_IncGameCycle_t IncGameCycle_Fn = (GameTime_IncGameCycle_t)(Memory::ScanAddress(0x410090));

void GameTime::Pause(bool triggerMessage)
{
    GameTime_Pause_Fn(nullptr, triggerMessage);
}

bool GameTime::Paused()
{
    return GameTime_Paused_Fn();
}

void GameTime::StepOnce()
{
    GameTime_StepOnce_Fn();
}

void GameTime::Readjust()
{
    GameTime_Readjust_Fn();
}

void GameTime::Synchronize()
{
    GameTime_Synchronize_Fn();
}

void GameTime::SetFastMode(bool flag)
{
    SetFastMode_Fn(flag);
}

bool GameTime::GetFastMode()
{
    return GetFastMode_Fn();
}

bool GameTime::GetFastForwardMode()
{
    return GetFastForwardMode_Fn();
}

void GameTime::SetDisplayMode(bool flag)
{
    SetDisplayMode_Fn(flag);
}

bool GameTime::GetDisplayMode()
{
    return GetDisplayMode_Fn();
}

float GameTime::TimeSinceCycle(unsigned long cycle)
{
    return TimeSinceCycle_Fn(cycle);
}

void GameTime::IncGameCycle()
{
    IncGameCycle_Fn();
}

