#include "LuaConsole.h"

int Lua_ConsoleProcessCmd(lua_State* L)
{
    const char* cmd = luaL_checkstring(L, 1);

    int a2 = 0;
    int a3 = 0;

    Console::ProcessCmd(cmd, a2, a3);
    return 0;
}

int Lua_ConsoleSetType(lua_State* L)
{
    //Stupid fix cause LuaJIT is "technically" 31bit not 32
    const char* str = luaL_checkstring(L, 1);
    U32 type = strtoul(str, nullptr, 0);
    Console::SetType(type);
    return 0;
}

int Lua_ConsoleMessage(lua_State* L)
{
    const char* msg = luaL_checkstring(L, 1);
    Console::Message("%s", msg);
    return 0;
}