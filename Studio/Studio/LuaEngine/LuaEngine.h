#pragma once
#include <LuaJIT/Src/lua.hpp>
#include <string>
#include <Windows.h>

#include "GameFuncs\util\LuaVarSys.h"
#include "GameFuncs\util\LuaConsole.h"
#include "GameFuncs\coregame_interface\LuaClientEvents.h"
#include "GameFuncs\coregame_orders\LuaOrdersGameMove.h"
#include "GameFuncs\multiplayer\LuaMultiplayerData.h"

class LuaEngine
{
public:
    LuaEngine();
    ~LuaEngine();
    lua_State* L;

    bool Initialize();
    bool LoadScript(const std::string& path);
    bool ExecuteString(const std::string& code);

    void SetVariable(const std::string& name, int value);
    bool CheckAndReload();

private:
    
    std::string m_scriptPath;
    FILETIME m_lastModTime;

private:
    void RegisterFunctions();

    //Log::Client
    static int Lua_LogClientWrite(lua_State* L);

    //In house funcs
    static int Lua_Sleep(lua_State* L);
    static int Lua_TmpFn(lua_State* L);
};
