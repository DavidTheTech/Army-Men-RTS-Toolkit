#include "LuaMapObject.h"

static int Lua_MapObject_GetID(lua_State* L)
{
    MapObject* obj = *(MapObject**)luaL_checkudata(L, 1, "MapObjectMeta");
    lua_pushinteger(L, obj->GetID());
    return 1;
}

static int Lua_MapObject_GetName(lua_State* L)
{
    MapObject* obj = *(MapObject**)luaL_checkudata(L, 1, "MapObjectMeta");
    lua_pushstring(L, obj->GetName());
    return 1;
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

void RegisterMapObject(lua_State* L)
{
    luaL_newmetatable(L, "MapObjectMeta");

    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

    luaL_Reg funcs[] = {
        {"GetID", Lua_MapObject_GetID},
        {"GetName", Lua_MapObject_GetName},
        {"GetX", Lua_MapObject_GetX},
        {"GetY", Lua_MapObject_GetY},
        {"GetZ", Lua_MapObject_GetZ},
        {nullptr, nullptr}
    };

    luaL_setfuncs(L, funcs, 0);
    lua_pop(L, 1);
}