#include "LuaEngine.h"
#include <iostream>
#include "..\GameFuncs\system\Log.h"
#include "..\GameFuncs\util\VarSys.h"

LuaEngine::LuaEngine() : L(nullptr)
{
}

LuaEngine::~LuaEngine()
{
    if (L)
    {
        lua_close(L);
        L = nullptr;
    }
}

bool LuaEngine::Initialize()
{
    L = luaL_newstate();
    if (!L)
    {
        return false;
    }

    luaL_openlibs(L);
    RegisterFunctions();
    return true;
}

bool LuaEngine::LoadScript(const std::string& path)
{
    if (luaL_dofile(L, path.c_str()) != LUA_OK)
    {
        Log::Client::Write("[LUA ENGINE]: LoadScript Error %s ", lua_tostring(L, -1));
        lua_pop(L, 1);
        return false;
    }
    return true;
}

bool LuaEngine::ExecuteString(const std::string& code)
{
    if (luaL_dostring(L, code.c_str()) != LUA_OK)
    {
        std::cerr << "[Lua] " << lua_tostring(L, -1) << std::endl;
        Log::Client::Write("[LUA ENGINE]: ExecuteString Error %s ", lua_tostring(L, -1));
        lua_pop(L, 1);
        return false;
    }
    return true;
}

void LuaEngine::SetVariable(const std::string& name, int value)
{
    lua_pushinteger(L, value);
    lua_setglobal(L, name.c_str());
}

void LuaEngine::RegisterFunctions()
{
    //Log table
    lua_newtable(L);

    //Client table
    lua_newtable(L);

    //Client.Write
    lua_pushcfunction(L, Lua_LogClientWrite);
    lua_setfield(L, -2, "Write");
    
    //Log.Client
    lua_setfield(L, -2, "Client");
    //_G.Log
    lua_setglobal(L, "Log");

    //VarSys
    lua_newtable(L);
    lua_pushcfunction(L, Lua_VarSysCreateCmd);
    lua_setfield(L, -2, "CreateCmd");
    lua_setglobal(L, "VarSys");
}

int LuaEngine::Lua_LogClientWrite(lua_State* L)
{
    const char* msg = luaL_checkstring(L, 1);
    Log::Client::Write("%s", msg);
    return 0;
}

int LuaEngine::Lua_VarSysCreateCmd(lua_State* L)
{
    const char* name = luaL_checkstring(L, 1);
    Log::Client::Write("err %s ", name);
    VarSys::CreateCmd(name, 0, 0);
    return 0;
}