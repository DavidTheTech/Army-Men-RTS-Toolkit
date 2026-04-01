#include "LuaVarSys.h"

int Lua_VarSysCreateCmd(lua_State* L)
{
    const char* name = luaL_checkstring(L, 1);
    VarSys::CreateCmd(name, 0, 0);
    return 0;
}