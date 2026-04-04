#pragma once
#include <Windows.h>
#include "..\..\Memory\Memory.h"
#include "..\..\Utils\Internals.h"

class Orders
{
public:
    class Game
    {
    public:
        class Move
        {
        public:
            static void Generate(DWORD player, Internals::Math::Vector3 terrainPos, bool attack, int modifier, char a5);

        };
    };
};