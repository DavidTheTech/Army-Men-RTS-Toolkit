#include "MapObjManager.h"

typedef int(__fastcall* sub_005FDDC0)();
sub_005FDDC0 MapObjCtrlGetOnMapList = (sub_005FDDC0)(Memory::ScanAddress(0x5FDDC0));

void MapObjManager::Update()
{
    objects.clear();

    //DWORD* listHead = MapObjCtrlGetOnMapList();// ? *(DWORD)(MapObjCtrlGetOnMapList() + 1) : NULL;
    DWORD* listHead = *(DWORD**)(MapObjCtrlGetOnMapList() + 4);
    while (listHead && *listHead)
    {
        objects.emplace_back(*listHead);
        listHead = reinterpret_cast<DWORD*>(listHead[2]);
    }
}

MapObject* MapObjManager::GetObjectByID(DWORD id)
{
    for (auto& obj : objects)
    {
        if (obj.GetID() == id)
            return &obj;
    }
    return nullptr;
}