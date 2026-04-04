#include "MapObject.h"
#include <cstring>

MapObject::MapObject(DWORD baseAddress) : objectAddress(baseAddress) {}

const char* MapObject::GetName() const
{
    if (!objectAddress)
    {
        return "Invalid";
    }

    DWORD inner = *reinterpret_cast<DWORD*>(objectAddress + 4);
    if (!inner)
    {
        return "NoName";
    }
    return reinterpret_cast<const char*>(inner + 144);
}

const char* MapObject::GetTeam() const
{
    if (!objectAddress)
    {
        return "Invalid";
    }

    DWORD inner = *reinterpret_cast<DWORD*>(objectAddress + 380);
    if (!inner)
    {
        return "NoTeam";
    }
    return reinterpret_cast<const char*>(inner + 8);
}

float MapObject::GetX() const
{
    DWORD pos = *reinterpret_cast<DWORD*>(objectAddress + 120);
    if (!pos)
    {
        return 0.0f;
    }
    return *reinterpret_cast<float*>(pos + 160);
}

void MapObject::SetX(float x)
{
    DWORD pos = *reinterpret_cast<DWORD*>(objectAddress + 120);
    if (!pos)
    {
        return;
    }
    *reinterpret_cast<float*>(pos + 160) = x;
}

float MapObject::GetY() const
{
    DWORD pos = *reinterpret_cast<DWORD*>(objectAddress + 120);
    if (!pos)
    {
        return 0.0f;
    }
    return *reinterpret_cast<float*>(pos + 164);
}

void MapObject::SetY(float y)
{
    DWORD pos = *reinterpret_cast<DWORD*>(objectAddress + 120);
    if (!pos)
    {
        return;
    }
    *reinterpret_cast<float*>(pos + 164) = y;
}

float MapObject::GetZ() const
{
    DWORD pos = *reinterpret_cast<DWORD*>(objectAddress + 120);
    if (!pos)
    {
        return 0.0f;
    }
    return *reinterpret_cast<float*>(pos + 168);
}

void MapObject::SetZ(float z)
{
    DWORD pos = *reinterpret_cast<DWORD*>(objectAddress + 120);
    if (!pos)
    {
        return;
    }
    *reinterpret_cast<float*>(pos + 168) = z;
}

int MapObject::GetHitpoints() const
{
    if (!objectAddress)
    {
        return 0;
    }
    return *reinterpret_cast<int*>(objectAddress + 168);
}

void MapObject::SetHitpoints(int hp)
{
    if (!objectAddress)
    {
        return;
    }
    *reinterpret_cast<int*>(objectAddress + 168) = hp;
}

int MapObject::GetTotalHitpoints() const
{
    DWORD inner = *reinterpret_cast<DWORD*>(objectAddress + 4);
    if (!inner)
    {
        return 0;
    }
    return *reinterpret_cast<int*>(inner + 984);
}

DWORD MapObject::GetID() const
{
    if (!objectAddress)
    {
        return 0;
    }
    return *reinterpret_cast<DWORD*>(objectAddress + 60);
}

int MapObject::GetResourceCount() const
{
    int count = 0;
    if (IsResource())
    {
        count = *reinterpret_cast<int*>(objectAddress + 0x178);
    }

    return count;
}

bool MapObject::IsResource() const
{
    DWORD inner = *reinterpret_cast<DWORD*>(objectAddress + 4);
    DWORD inner2 = inner + 0x51C;
    bool isRes = IsResource_Func(reinterpret_cast<U32*>(inner2));
    
    return isRes;
}