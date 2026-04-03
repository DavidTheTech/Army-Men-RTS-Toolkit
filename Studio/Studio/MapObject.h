#pragma once
#include <vector>
#include <string>
#include <Windows.h>
#include "Memory/Memory.h"

class MapObject
{
public:
    typedef int(__fastcall* sub_543A80)(U32* a1);
    sub_543A80 Func_543A80 = (sub_543A80)(Memory::ScanAddress(0x543A80));

    MapObject(DWORD baseAddress);

    const char* GetName() const;
    const char* GetTeam() const;

    float GetX() const;
    void SetX(float x);
    float GetY() const;
    void SetY(float y);
    float GetZ() const;
    void SetZ(float z);

    int GetHitpoints() const;
    void SetHitpoints(int hp);

    int GetTotalHitpoints() const;
    int GetResourceCount() const;
    bool IsResource() const;

    DWORD GetID() const;

    DWORD objectAddress;

private:
};