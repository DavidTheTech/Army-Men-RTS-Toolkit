#pragma once
#include <Windows.h>
#include "..\..\Memory\Memory.h"
#include "../../Utils/Internals.h"

class Client
{
public:
    class Events
    {
    public:
        static bool CreateParticle(DWORD* mouse, Internals::Math::Vector3& location, U32 typeCrc);
        static void SelectUnit(DWORD unit, int flags);
        static void UpdateSelectedLists(DWORD mouse);
        static void TriggerResponse(DWORD* mouse, DWORD* unitObjList, U32 crc);
        static void TriggerClientMode(DWORD* mouse, int mode);
    };
};