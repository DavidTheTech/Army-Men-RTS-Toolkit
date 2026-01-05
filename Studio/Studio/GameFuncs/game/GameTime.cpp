#include "GameTime.h"

typedef void(__thiscall* GameTime_Pause_t)(void* thisptr, bool triggerMessage);
typedef bool(__cdecl* GameTime_Paused_t)();
typedef void(__cdecl* GameTime_StepOnce_t)();
typedef void(__cdecl* GameTime_Readjust_t)();
typedef void(__cdecl* GameTime_Synchronize_t)();
typedef void(__cdecl* GameTime_SetFastMode_t)();

static GameTime_Pause_t GameTime_Pause_Fn = (GameTime_Pause_t)(Memory::ScanAddress(0x40FF20));
static GameTime_Paused_t GameTime_Paused_Fn = (GameTime_Paused_t)(Memory::ScanAddress(0x40FF90));
static GameTime_StepOnce_t GameTime_StepOnce_Fn = (GameTime_StepOnce_t)(Memory::ScanAddress(0x40FFA0));
static GameTime_Readjust_t GameTime_Readjust_Fn = (GameTime_Readjust_t)(Memory::ScanAddress(0x40FFC0));
static GameTime_Synchronize_t GameTime_Synchronize_Fn = (GameTime_Synchronize_t)(Memory::ScanAddress(0x410000));
static GameTime_SetFastMode_t SetFastMode_Fn = (GameTime_SetFastMode_t)(Memory::ScanAddress(0x410010));

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

void GameTime::SetFastMode()
{
    SetFastMode_Fn();
}