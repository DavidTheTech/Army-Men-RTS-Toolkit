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

    g_manager->Update();
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

static int Lua_MapObjManager_GetObjectByName(lua_State* L)
{
    const char* name = luaL_checkstring(L, 1);
    if (!g_manager)
    {
        lua_pushnil(L);
        return 1;
    }

    g_manager->Update();
    MapObject* obj = g_manager->GetObjectByName(name);
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

static int Lua_MapObjManager_GetObjectsByName(lua_State* L)
{
    const char* name = luaL_checkstring(L, 1);
    if (!g_manager)
    {
        lua_newtable(L);
        return 1;
    }

    g_manager->Update();
    std::vector<MapObject*> matches = g_manager->GetObjectsByName(name);

    lua_newtable(L);
    for (size_t i = 0; i < matches.size(); i++)
    {
        MapObject** ud = (MapObject**)lua_newuserdata(L, sizeof(MapObject*));
        *ud = matches[i];
        luaL_getmetatable(L, "MapObjectMeta");
        lua_setmetatable(L, -2);
        lua_rawseti(L, -2, i + 1);
    }
    return 1;
}

static int Lua_MapObjManager_GetObjectsByTeam(lua_State* L)
{
    const char* team = luaL_checkstring(L, 1);
    if (!g_manager)
    {
        lua_newtable(L); return 1;
    }

    g_manager->Update();
    auto matches = g_manager->GetObjectsByTeam(team);
    lua_newtable(L);

    for (size_t i = 0; i < matches.size(); i++)
    {
        MapObject** ud = (MapObject**)lua_newuserdata(L, sizeof(MapObject*));
        *ud = matches[i];
        luaL_getmetatable(L, "MapObjectMeta");
        lua_setmetatable(L, -2);
        lua_rawseti(L, -2, i + 1);
    }
    return 1;
}

static int Lua_MapObjManager_Objects(lua_State* L)
{
    if (!g_manager)
    {
        lua_newtable(L);
        return 1;
    }

    g_manager->Update();
    auto& objects = g_manager->GetObjects();

    lua_newtable(L);
    for (size_t i = 0; i < objects.size(); i++)
    {
        MapObject** ud = (MapObject**)lua_newuserdata(L, sizeof(MapObject*));
        *ud = &objects[i];
        luaL_getmetatable(L, "MapObjectMeta");
        lua_setmetatable(L, -2);
        lua_rawseti(L, -2, i + 1);
    }
    return 1;
}

static int Lua_MapObjManager_GetResourcesObjects(lua_State* L)
{
    if (!g_manager)
    {
        lua_newtable(L);
        return 1;
    }

    g_manager->Update();
    auto& objects = g_manager->GetObjects();

    lua_newtable(L);
    int index = 1;
    for (size_t i = 0; i < objects.size(); i++)
    {
        if (objects[i].IsResource())
        {
            MapObject** ud = (MapObject**)lua_newuserdata(L, sizeof(MapObject*));
            *ud = &objects[i];
            luaL_getmetatable(L, "MapObjectMeta");
            lua_setmetatable(L, -2);
            lua_rawseti(L, -2, index++);
        }
    }
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

    lua_pushcfunction(L, Lua_MapObjManager_GetObjectByName);
    lua_setfield(L, -2, "GetObjectByName");

    lua_pushcfunction(L, Lua_MapObjManager_GetObjectsByName);
    lua_setfield(L, -2, "GetObjectsByName");

    lua_pushcfunction(L, Lua_MapObjManager_GetObjectsByTeam);
    lua_setfield(L, -2, "GetObjectsByTeam");

    lua_pushcfunction(L, Lua_MapObjManager_GetResourcesObjects);
    lua_setfield(L, -2, "GetResourceObjects");

    lua_pushcfunction(L, Lua_MapObjManager_Objects);
    lua_setfield(L, -2, "Objects");

    lua_setglobal(L, "MapObjManager");
}