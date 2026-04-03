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
        {nullptr, nullptr}
    };

    luaL_setfuncs(L, funcs, 0);
    lua_pop(L, 1);
}