#pragma once
#include <Windows.h>
#include "..\..\Memory\Memory.h"

struct Color
{
    union
    {
        struct
        {
            U32 color;
        };
        struct
        {
            U8 b;
            U8 g;
            U8 r;
            U8 a;
        };
    };
};

class Team
{
public:
    static DWORD* SetColor(DWORD* team, const Color& c);
    static DWORD GetRelation(DWORD* team1, DWORD* team2);
};