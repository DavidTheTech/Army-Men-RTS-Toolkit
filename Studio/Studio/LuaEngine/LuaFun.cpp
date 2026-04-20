#include "LuaFun.h"

int Lua_FunRandomColor(lua_State* L)
{
    printf("hello\n");
    DWORD** teamArray = (DWORD**)0x76F898;
    if (!teamArray) return 0;

    for (DWORD** pTeam = teamArray; *pTeam; ++pTeam)
    {
        DWORD* team = *pTeam;
        if (!team)
        {
            continue;
        }

        Color randomColor;
        randomColor.r = rand() % 256;
        randomColor.g = rand() % 256;
        randomColor.b = rand() % 256;
        randomColor.a = 255;
        Team::SetColor(team, randomColor);
    }

    return 0;
}