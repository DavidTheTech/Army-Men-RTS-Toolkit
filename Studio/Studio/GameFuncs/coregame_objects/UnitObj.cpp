#include "UnitObj.h"

UnitObj::UnitObj(DWORD address)
    : m_address(address)
{
}

const char* UnitObj::GetName() const
{
    DWORD typePtr = *reinterpret_cast<DWORD*>(m_address + 0x4);
    return reinterpret_cast<const char*>(typePtr + 0x90);
}

int UnitObj::GetHitpoints() const
{
    return *reinterpret_cast<int*>(m_address + 0xA8);
}

int UnitObj::GetTotalHitpoints() const
{
    DWORD typePtr = *reinterpret_cast<DWORD*>(m_address + 0x4);
    return *reinterpret_cast<int*>(typePtr + 0x3D8);
}

int UnitObj::GetID() const
{
    return *reinterpret_cast<int*>(m_address + 0x3C);
}

BYTE UnitObj::GetTeamsHaveSeen() const
{
    return *reinterpret_cast<BYTE*>(m_address + 0x185);
}

BYTE UnitObj::GetTeamsCanSee() const
{
    return *reinterpret_cast<BYTE*>(m_address + 0x184);
}

Vector3 UnitObj::GetPosition() const
{
    Vector3 pos{};

    DWORD transform = *reinterpret_cast<DWORD*>(m_address + 0x78);

    pos.x = *reinterpret_cast<float*>(transform + 0xA0);
    pos.y = *reinterpret_cast<float*>(transform + 0xA4);
    pos.z = *reinterpret_cast<float*>(transform + 0xA8);

    return pos;
}
