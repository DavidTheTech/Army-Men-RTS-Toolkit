#include "LuaSquadManager.h"

int Lua_SquadManagerCreate(lua_State* L)
{
    if (lua_gettop(L) != 1)
    {
        luaL_error(L, "SquadManager.Create() expects exactly 1 argument");
        return 0;
    }
    int squadNumber = luaL_checkinteger(L, 1);
    std::string cmd = "iface.sendnotifyevent Client::SquadManager.Squad" + std::to_string(squadNumber) + " SquadControl::Create";

    Console::ProcessCmd(cmd.c_str(), 0, 0);

    return 0;
}

int Lua_SquadManagerAdd(lua_State* L)
{
    if (lua_gettop(L) != 1)
    {
        luaL_error(L, "SquadManager.Add() expects exactly 1 argument");
        return 0;
    }
    int squadNumber = luaL_checkinteger(L, 1);
    std::string cmd = "iface.sendnotifyevent Client::SquadManager.Squad" + std::to_string(squadNumber) + " SquadControl::Add";

    Console::ProcessCmd(cmd.c_str(), 0, 0);

    return 0;
}

int Lua_SquadManagerSelect(lua_State* L)
{
    if (lua_gettop(L) != 1)
    {
        luaL_error(L, "SquadManager.Select() expects exactly 1 argument");
        return 0;
    }
    int squadNumber = luaL_checkinteger(L, 1);
    std::string cmd = "iface.sendnotifyevent Client::SquadManager.Squad" + std::to_string(squadNumber) + " SquadControl::Select";

    Console::ProcessCmd(cmd.c_str(), 0, 0);

    return 0;
}

int Lua_SquadManagerJumpTo(lua_State* L)
{
    if (lua_gettop(L) != 1)
    {
        luaL_error(L, "SquadManager.JumpTo() expects exactly 1 argument");
        return 0;
    }
    int squadNumber = luaL_checkinteger(L, 1);
    std::string cmd = "iface.sendnotifyevent Client::SquadManager.Squad" + std::to_string(squadNumber) + " SquadControl::JumpTo";

    Console::ProcessCmd(cmd.c_str(), 0, 0);

    return 0;
}