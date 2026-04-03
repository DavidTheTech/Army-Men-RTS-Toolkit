#include "Game.h"

Game g_game;

VarSys::VarStringFake TommyTime;
VarSys::VarStringFake TommyTimeU;
VarSys::VarStringFake GGMissionName;
VarSys::VarStringFake GGMissionNameDate;
VarSys::VarStringFake GGGroup;

VarSys::VarIntegerFake studioNum;

void Game::Loop()
{
    Internals::WaitForTrue(GameGod::IsInitialized());
    Sleep(5000);
    while (true)
    {
        Sleep(10);
        VarSys::SetString((DWORD*)&TommyTime, Internals::GetDate());
        VarSys::SetString((DWORD*)&TommyTimeU, Internals::GetDate(1));

        DWORD selected = Missions::GetSelected();

        if (selected)
        {
            const char* mapName = (const char*)(selected + 0x18);
            DWORD ptr = *(DWORD*)(selected + 0x14);
            const char* missionPath = nullptr;
            if (ptr)
            {
                missionPath = (const char*)(ptr + 0xC);
                VarSys::SetString((DWORD*)&GGGroup, missionPath);
            }
        }

        const char* MissionName = Missions::GetName();
        VarSys::SetString((DWORD*)&GGMissionName, MissionName);

        const char* MissionNameDate = Missions::GetNameDate();
        VarSys::SetString((DWORD*)&GGMissionNameDate, MissionNameDate);
    }
}

void Game::Setup()
{
    VarSys::CreateString("sys.date", "swag", VarSys::DEFAULT, &TommyTime, 0);
    VarSys::CreateString("sys.dateu", "swag", VarSys::DEFAULT, &TommyTimeU, 0);
    VarSys::CreateString("gamegod.missions.name", "swag", VarSys::DEFAULT, &GGMissionName, 0);
    VarSys::CreateString("gamegod.missions.namewithdate", "swag", VarSys::DEFAULT, &GGMissionNameDate, 0);

    VarSys::CreateString("gamegod.missions.group", "N/A", VarSys::DEFAULT, &GGGroup, 0);

    Loop();
}