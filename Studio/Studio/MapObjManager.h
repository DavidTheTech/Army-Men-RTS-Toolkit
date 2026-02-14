#pragma once
#include "MapObject.h"
#include <vector>
#include "Memory/Memory.h"

class MapObjManager
{
public:
    void Update();

    std::vector<MapObject>& GetObjects() { return objects; }

    MapObject* GetObjectByID(DWORD id);

private:
    std::vector<MapObject> objects;
};