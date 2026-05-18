#include "LuaMapObject.h"
#include "../GameFuncs/system/Log.h"
#include "../GameFuncs/system/Crc.h"
#include "../GameFuncs/coregame_orders/Orders_game_constructor.h"
#include "../GameFuncs/util/Console.h"

static int Lua_MapObject_GetID(lua_State* L)
{
    MapObject* obj = *(MapObject**)luaL_checkudata(L, 1, "MapObjectMeta");
    lua_pushinteger(L, obj->GetID());
    printf("0x%08X\n", obj->objectAddress);
    return 1;
}

static int Lua_MapObject_GetName(lua_State* L)
{
    MapObject* obj = *(MapObject**)luaL_checkudata(L, 1, "MapObjectMeta");
    lua_pushstring(L, obj->GetName());
    return 1;
}

static int Lua_MapObject_GetTeam(lua_State* L)
{
    MapObject* obj = *(MapObject**)luaL_checkudata(L, 1, "MapObjectMeta");
    lua_pushstring(L, obj->GetTeam());
    return 1;
}

static int Lua_MapObject_GetHitpoints(lua_State* L)
{
    MapObject* obj = *(MapObject**)luaL_checkudata(L, 1, "MapObjectMeta");
    lua_pushnumber(L, obj->GetHitpoints());
    return 1;
}

static int Lua_MapObject_GetTotalHitpoints(lua_State* L)
{
    MapObject* obj = *(MapObject**)luaL_checkudata(L, 1, "MapObjectMeta");
    lua_pushnumber(L, obj->GetTotalHitpoints());
    return 1;
}

static int Lua_MapObject_SetHitpoints(lua_State* L)
{
    MapObject* obj = *(MapObject**)luaL_checkudata(L, 1, "MapObjectMeta");
    int hp = (int)luaL_checkinteger(L, 2);
    obj->SetHitpoints(hp);
    return 1;
}

static int Lua_MapObject_SetPosition(lua_State* L)
{
    MapObject* obj = *(MapObject**)luaL_checkudata(L, 1, "MapObjectMeta");
    float x = (float)luaL_checknumber(L, 2);
    float y = (float)luaL_checknumber(L, 3);
    float z = (float)luaL_checknumber(L, 4);

    obj->SetX(x);
    obj->SetY(y);
    obj->SetZ(z);

    return 0;
}

static int Lua_MapObject_GetPosition(lua_State* L)
{
    MapObject* obj = *(MapObject**)luaL_checkudata(L, 1, "MapObjectMeta");
    lua_pushnumber(L, obj->GetX());
    lua_pushnumber(L, obj->GetY());
    lua_pushnumber(L, obj->GetZ());
    return 3;
}

static int Lua_MapObject_GetX(lua_State* L)
{
    MapObject* obj = *(MapObject**)luaL_checkudata(L, 1, "MapObjectMeta");
    lua_pushnumber(L, obj->GetX());
    return 1;
}

static int Lua_MapObject_GetY(lua_State* L)
{
    MapObject* obj = *(MapObject**)luaL_checkudata(L, 1, "MapObjectMeta");
    lua_pushnumber(L, obj->GetY());
    return 1;
}

static int Lua_MapObject_GetZ(lua_State* L)
{
    MapObject* obj = *(MapObject**)luaL_checkudata(L, 1, "MapObjectMeta");
    lua_pushnumber(L, obj->GetZ());
    return 1;
}

static int Lua_MapObject_SetX(lua_State* L)
{
    MapObject* obj = *(MapObject**)luaL_checkudata(L, 1, "MapObjectMeta");
    float x = (float)luaL_checknumber(L, 2);
    obj->SetX(x);
    return 0;
}

static int Lua_MapObject_SetY(lua_State* L)
{
    MapObject* obj = *(MapObject**)luaL_checkudata(L, 1, "MapObjectMeta");
    float y = (float)luaL_checknumber(L, 2);
    obj->SetY(y);
    return 0;
}

static int Lua_MapObject_SetZ(lua_State* L)
{
    MapObject* obj = *(MapObject**)luaL_checkudata(L, 1, "MapObjectMeta");
    float z = (float)luaL_checknumber(L, 2);
    obj->SetZ(z);
    return 0;
}

static int Lua_MapObject_GetResourceCount(lua_State* L)
{
    MapObject* obj = *(MapObject**)luaL_checkudata(L, 1, "MapObjectMeta");
    lua_pushnumber(L, obj->GetResourceCount());
    return 1;
}

static int Lua_MapObject_IsResource(lua_State* L)
{
    MapObject* obj = *(MapObject**)luaL_checkudata(L, 1, "MapObjectMeta");
    lua_pushboolean(L, obj->IsResource());
    return 1;
}

static int Lua_MapObject_QueueUnit2(lua_State* L)
{
    MapObject* obj = *(MapObject**)luaL_checkudata(L, 1, "MapObjectMeta");
    if (!obj)
    {
        luaL_error(L, "MapObject.QueueUnit() called on invalid object");
        return 0;
    }

    const char* unitnameStr = luaL_checkstring(L, 2);
    U32 unitnameCrc = Crc::CalcStr(unitnameStr);

    // Get the task pointer using the offsets from sub_5DF710
    DWORD* unitPtr7F = *(DWORD**)((DWORD)obj + 0x1FC);
    if (!unitPtr7F)
    {
        luaL_error(L, "MapObject.QueueUnit() - no UnitConstructor task structure");
        return 0;
    }

    DWORD* task = *(DWORD**)((DWORD)unitPtr7F + 0x38);
    if (!task)
    {
        luaL_error(L, "MapObject.QueueUnit() - task pointer is NULL");
        return 0;
    }

    // Validate the task (check magic at task+0x10 -> +0x44)
    DWORD* vtableCheck = *(DWORD**)((DWORD)task + 0x10);
    if (!vtableCheck)
    {
        luaL_error(L, "MapObject.QueueUnit() - task validation failed");
        return 0;
    }

    DWORD magic = *(DWORD*)((DWORD)vtableCheck + 0x44);
    DWORD expectedMagic = *(DWORD*)0x776564;  // dword_776564 from the game
    if (magic != expectedMagic)
    {
        luaL_error(L, "MapObject.QueueUnit() - task magic mismatch (0x%08X != 0x%08X)", magic, expectedMagic);
        return 0;
    }

    // Call QueueAdd
    typedef void(__thiscall* QueueAdd_t)(void* task, U32 constructCrc);
    QueueAdd_t QueueAdd = (QueueAdd_t)0x62D690;
    QueueAdd(task, unitnameCrc);

    return 0;
}

/*static int Lua_MapObject_QueueUnit(lua_State* L)
{
    MapObject* obj = *(MapObject**)luaL_checkudata(L, 1, "MapObjectMeta");
    if (!obj) luaL_error(L, "invalid object");

    const char* unitnameStr = luaL_checkstring(L, 2);
    U32 unitnameCrc = Crc::CalcStr(unitnameStr);

    void* task = *(void**)((DWORD)obj + 0x38);
    if (!task) luaL_error(L, "task is NULL");

    typedef void(__thiscall* QueueAdd_t)(void*, U32);
    QueueAdd_t QueueAdd = (QueueAdd_t)0x62D690;
    QueueAdd(task, unitnameCrc);

    return 0;
}*/
static int Lua_MapObject_QueueUnit(lua_State* L)
{
    MapObject* obj = *(MapObject**)luaL_checkudata(L, 1, "MapObjectMeta");
    if (!obj)
    {
        luaL_error(L, "MapObject.QueueUnit() called on invalid object");
        return 0;
    }

    U32 operation = 0x0;// (U32)luaL_checkinteger(L, 2);

    const char* unitnameStr = luaL_checkstring(L, 2);
    U32 unitnameCrc = Crc::CalcStr(unitnameStr);

    DWORD player = *(DWORD*)0x76F9C8;
    Orders::Game::Constructor::Generate(player, (U32)obj->GetID(), 0x258B47CE, unitnameCrc);
    
    DWORD constructorTask = obj->GetTmp();

    //printf("swa\n");
    //printf("0x%08X\n", constructorTask);
    //printf("swaend\n");

    typedef void(__thiscall* QueueAdd_t)(void* task, U32 constructCrc);
    QueueAdd_t QueueAdd = (QueueAdd_t)(Memory::ScanAddress(0x62D690));


    //printf("0x%08X : 0x%08X\n", constructorTask, (obj->objectAddress - 3));

    //QueueAdd((void*)constructorTask, unitnameCrc);

    return 0;
}

/*static int Lua_MapObject_QueueUnit(lua_State* L)
{
    MapObject* obj = *(MapObject**)luaL_checkudata(L, 1, "MapObjectMeta");
    if (!obj)
    {
        luaL_error(L, "MapObject.QueueUnit() called on invalid object");
        return 0;
    }

    const char* unitnameStr = luaL_checkstring(L, 2);
    U32 unitnameCrc = Crc::CalcStr(unitnameStr);

    DWORD* unitPtr7F = *(DWORD**)((DWORD)obj + 0x1FC);
    if (!unitPtr7F)
    {
        luaL_error(L, "MapObject.QueueUnit() - unitPtr[0x7F] is NULL");
        return 0;
    }

    DWORD* result = *(DWORD**)((DWORD)unitPtr7F + 0x38);
    if (!result)
    {
        luaL_error(L, "MapObject.QueueUnit() - result is NULL");
        return 0;
    }

    DWORD* result4 = *(DWORD**)((DWORD)result + 0x04);
    if (!result4)
    {
        luaL_error(L, "MapObject.QueueUnit() - result[4] is NULL");
        return 0;
    }

    DWORD valueAt44 = *(DWORD*)((DWORD)result4 + 0x44);
    DWORD dword_776564 = *(DWORD*)0x776564;

    if (valueAt44 != dword_776564)
    {
        luaL_error(L, "MapObject.QueueUnit() - validation check failed");
        return 0;
    }

    typedef void(__thiscall* QueueAdd_t)(void* task, U32 constructCrc);
    QueueAdd_t QueueAdd = (QueueAdd_t)(Memory::ScanAddress(0x62D690));

    QueueAdd(result, unitnameCrc);

    return 0;
}*/

void RegisterMapObject(lua_State* L)
{
    luaL_newmetatable(L, "MapObjectMeta");

    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

    luaL_Reg funcs[] = {
        {"GetID", Lua_MapObject_GetID},
        {"GetName", Lua_MapObject_GetName},
        {"GetTeam", Lua_MapObject_GetTeam},
        {"GetHitpoints", Lua_MapObject_GetHitpoints},
        {"GetTotalHitpoints", Lua_MapObject_GetTotalHitpoints},
        {"SetHitpoints", Lua_MapObject_SetHitpoints},
        {"SetPosition", Lua_MapObject_SetPosition},
        {"GetPosition", Lua_MapObject_GetPosition},
        {"GetX", Lua_MapObject_GetX},
        {"GetY", Lua_MapObject_GetY},
        {"GetZ", Lua_MapObject_GetZ},
        {"SetX", Lua_MapObject_SetX},
        {"SetY", Lua_MapObject_SetY},
        {"SetZ", Lua_MapObject_SetZ},
        {"GetResourceCount", Lua_MapObject_GetResourceCount},
        {"IsResource", Lua_MapObject_IsResource},
        {"QueueUnit", Lua_MapObject_QueueUnit},
        {nullptr, nullptr}
    };

    luaL_setfuncs(L, funcs, 0);
    lua_pop(L, 1);
}