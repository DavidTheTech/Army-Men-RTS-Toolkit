#pragma once
#include <Windows.h>

struct Vector3
{
    float x;
    float y;
    float z;
};

class UnitObj
{
public:
    explicit UnitObj(DWORD address);

    const char* GetName() const;
    int GetHitpoints() const;
    int GetTotalHitpoints() const;
    int GetID() const;
    BYTE GetTeamsHaveSeen() const;
    BYTE GetTeamsCanSee() const;
    Vector3 GetPosition() const;

    DWORD GetAddress() const
    {
        return m_address;
    }

private:
    DWORD m_address;
};