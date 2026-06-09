#include "Game.h"

typedef bool(__fastcall* Game_SaveMission_t)(const char* path, const char* mission);
static Game_SaveMission_t SaveMission_Fn = (Game_SaveMission_t)(Memory::ScanAddress(0x53B200));

bool Game::SaveMission(const char* path, const char* mission)
{
    return SaveMission_Fn(path, mission);
}
