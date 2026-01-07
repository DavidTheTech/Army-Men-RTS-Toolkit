#include "MultiPlayer_Host.h"

typedef bool(__thiscall* Host_CreateAITeam_t)();
typedef bool(__thiscall* Host_FillAITeams_t)();
typedef bool(__thiscall* Host_CheckLaunch_t)();

static Host_CreateAITeam_t Host_CreateAITeam_Fn = (Host_CreateAITeam_t)(Memory::ScanAddress(0x4BF320));
static Host_FillAITeams_t Host_FillAITeams_Fn = (Host_FillAITeams_t)(Memory::ScanAddress(0x4BF3C0));
static Host_CheckLaunch_t Host_CheckLaunch_Fn = (Host_CheckLaunch_t)(Memory::ScanAddress(0x4BF6D0));

bool MultiPlayer::Host::CreateAITeam()
{
    return Host_CreateAITeam_Fn();
}

bool MultiPlayer::Host::FillAITeams()
{
    return Host_FillAITeams_Fn();
}

bool MultiPlayer::Host::CheckLaunch()
{
    return Host_CheckLaunch_Fn();
}