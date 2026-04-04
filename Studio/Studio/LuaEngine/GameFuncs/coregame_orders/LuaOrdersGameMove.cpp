#include "LuaOrdersGameMove.h"

int Lua_OrdersGameMoveGenerate(lua_State* L)
{
    DWORD player = *(DWORD*)0x76F9C8;

    float x = (float)luaL_checknumber(L, 1);
    float y = (float)luaL_checknumber(L, 2);
    float z = (float)luaL_checknumber(L, 3);
    bool attack = lua_toboolean(L, 4) != 0;
    int modifier = (int)luaL_optinteger(L, 5, 1);
    char a5 = (char)luaL_optinteger(L, 6, 1);

    Internals::Math::Vector3 pos = { x, y, z };
    
    DWORD mouse = *(DWORD*)0x773194;
    DWORD* mouse2 = *(DWORD**)0x773194;
    DWORD* UnitObjList2 = (DWORD*)(mouse + 0xA8);

    Client::Events::UpdateSelectedLists(mouse);
    Orders::Game::Move::Generate(player, pos, attack, modifier, a5);
    Client::Events::TriggerResponse(mouse2, UnitObjList2, 0x3381FB36);
    Client::Events::TriggerClientMode(mouse2, 0);
    return 0;
}