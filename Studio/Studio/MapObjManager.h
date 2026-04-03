#pragma once
#include "MapObject.h"
#include <vector>
#include "Memory/Memory.h"

class MapObjManager
{
public:
    void Update();

    std::vector<MapObject>& GetObjects()
    {
        return objects;
    }

    MapObject* GetObjectByID(DWORD id);
    MapObject* GetObjectByName(const char* name);
    std::vector<MapObject*> GetObjectsByName(const char* name);
    std::vector<MapObject*> GetObjectsByTeam(const char* team);

private:
    std::vector<MapObject> objects;
};