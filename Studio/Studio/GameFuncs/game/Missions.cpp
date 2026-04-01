#include "Missions.h"

const char* Missions::GetName()
{
    char* missionObject = *(char**)0x6BB0D0;

    if (!missionObject)
    {
        return "NewMap";
    }

    return (const char*)(missionObject + 0x18);
}

const char* Missions::GetNameDate()
{
    std::string result = Internals::GetDate(1);
    const char* missionObject = *(const char**)0x6BB0D0;

    if (missionObject)
    {
        result += (const char*)(missionObject + 0x18);
    }
    else {
        result += "NewMap";
    }

    return result.c_str();
}

DWORD Missions::GetSelected()
{
    return *(DWORD*)Memory::ScanAddress(0x6BB0D4);
}