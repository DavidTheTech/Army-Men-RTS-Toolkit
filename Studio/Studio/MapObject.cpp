#include "MapObject.h"
#include <cstring>

MapObject::MapObject(DWORD baseAddress) : objectAddress(baseAddress) {}

const char* MapObject::GetName() const
{
    if (!objectAddress) return "Invalid";
    DWORD inner = *reinterpret_cast<DWORD*>(objectAddress + 4);
    if (!inner) return "NoName";
    return reinterpret_cast<const char*>(inner + 144);
}

float MapObject::GetX() const
{
    DWORD pos = *reinterpret_cast<DWORD*>(objectAddress + 120);
    if (!pos) return 0.0f;
    return *reinterpret_cast<float*>(pos + 160);
}

void MapObject::SetX(float x)
{
    DWORD pos = *reinterpret_cast<DWORD*>(objectAddress + 120);
    if (!pos) return;
    *reinterpret_cast<float*>(pos + 160) = x;
}

float MapObject::GetY() const
{
    DWORD pos = *reinterpret_cast<DWORD*>(objectAddress + 120);
    if (!pos) return 0.0f;
    return *reinterpret_cast<float*>(pos + 164);
}

void MapObject::SetY(float y)
{
    DWORD pos = *reinterpret_cast<DWORD*>(objectAddress + 120);
    if (!pos) return;
    *reinterpret_cast<float*>(pos + 164) = y;
}

float MapObject::GetZ() const
{
    DWORD pos = *reinterpret_cast<DWORD*>(objectAddress + 120);
    if (!pos) return 0.0f;
    return *reinterpret_cast<float*>(pos + 168);
}

void MapObject::SetZ(float z)
{
    DWORD pos = *reinterpret_cast<DWORD*>(objectAddress + 120);
    if (!pos) return;
    *reinterpret_cast<float*>(pos + 168) = z;
}

int MapObject::GetHitpoints() const
{
    if (!objectAddress) return 0;
    return *reinterpret_cast<int*>(objectAddress + 168);
}

void MapObject::SetHitpoints(int hp)
{
    if (!objectAddress) return;
    *reinterpret_cast<int*>(objectAddress + 168) = hp;
}

int MapObject::GetTotalHitpoints() const
{
    DWORD inner = *reinterpret_cast<DWORD*>(objectAddress + 4);
    if (!inner) return 0;
    return *reinterpret_cast<int*>(inner + 984);
}

DWORD MapObject::GetID() const
{
    if (!objectAddress) return 0;
    return *reinterpret_cast<DWORD*>(objectAddress + 60);
}