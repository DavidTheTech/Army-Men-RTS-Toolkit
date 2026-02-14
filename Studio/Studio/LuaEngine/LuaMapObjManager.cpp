#include "LuaMapObjManager.h"
#include "LuaMapObject.h"

static MapObjManager* g_manager = nullptr;

static int Lua_MapObjManager_Update(lua_State* L)
{
    if (g_manager)
    {
        g_manager->Update();
    }
    return 0;
}

static int Lua_MapObjManager_GetObjectByID(lua_State* L)
{
    DWORD id = (DWORD)luaL_checkinteger(L, 1);
    if (!g_manager)
    {
        lua_pushnil(L);
        return 1;
    }

    MapObject* obj = g_manager->GetObjectByID(id);
    if (!obj)
    {
        lua_pushnil(L);
        return 1;
    }

    MapObject** ud = (MapObject**)lua_newuserdata(L, sizeof(MapObject*));
    *ud = obj;

    luaL_getmetatable(L, "MapObjectMeta");
    lua_setmetatable(L, -2);
    return 1;
}

struct IterState
{
    MapObjManager* manager;
    size_t index = 0;
};

static int Lua_MapObjManager_IterNext(lua_State* L)
{
    IterState* state = (IterState*)lua_touserdata(L, lua_upvalueindex(1));
    auto& objs = state->manager->GetObjects();

    if (state->index >= objs.size())
    {
        return 0;
    }

    MapObject** ud = (MapObject**)lua_newuserdata(L, sizeof(MapObject*));
    *ud = &objs[state->index];

    luaL_getmetatable(L, "MapObjectMeta");
    lua_setmetatable(L, -2);

    state->index++;

    return 1;
}

static int Lua_MapObjManager_Objects(lua_State* L)
{
    IterState* state = (IterState*)lua_newuserdata(L, sizeof(IterState));
    state->manager = g_manager;
    state->index = 0;

    lua_pushcclosure(L, Lua_MapObjManager_IterNext, 1);
    return 1;
}

void RegisterMapObjManager(lua_State* L, MapObjManager& manager)
{
    g_manager = &manager;

    RegisterMapObject(L);

    lua_newtable(L);

    lua_pushcfunction(L, Lua_MapObjManager_Update);
    lua_setfield(L, -2, "Update");

    lua_pushcfunction(L, Lua_MapObjManager_GetObjectByID);
    lua_setfield(L, -2, "GetObjectByID");

    lua_pushcfunction(L, Lua_MapObjManager_Objects);
    lua_setfield(L, -2, "Objects");

    lua_setglobal(L, "MapObjManager");
}