#include "MapObjCtrl.h"

typedef bool(__fastcall* MapObjCtrl_GetOnMapList_t)();
static MapObjCtrl_GetOnMapList_t GetOnMapList_Fn = (MapObjCtrl_GetOnMapList_t)(Memory::ScanAddress(0x5FDDC0));

bool MapObjCtrl::GetOnMapList()
{
    //return GetOnMapList_Fn();
}
