#pragma once
#include <vector>
#include <string>
#include <Windows.h>

class MapObject
{
public:
    MapObject(DWORD baseAddress);

    const char* GetName() const;

    float GetX() const;
    void SetX(float x);
    float GetY() const;
    void SetY(float y);
    float GetZ() const;
    void SetZ(float z);

    int GetHitpoints() const;
    void SetHitpoints(int hp);

    int GetTotalHitpoints() const;

    DWORD GetID() const;

    DWORD objectAddress;

private:
};