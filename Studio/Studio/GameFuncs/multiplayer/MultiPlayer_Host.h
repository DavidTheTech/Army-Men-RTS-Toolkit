#pragma once
#include <Windows.h>
#include "..\..\Memory\Memory.h"

namespace MultiPlayer
{
    class Host
    {
    public:
        static bool CreateAITeam();
        static bool FillAITeams();
        static bool CheckLaunch();
    };
}