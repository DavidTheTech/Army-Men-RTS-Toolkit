#include "LuaEngine.h"
#include <iostream>
#include <vector>
#include "..\GameFuncs\system\Log.h"
#include "..\GameFuncs\util\VarSys.h"
#include "../GameFuncs/util/Console.h"

#include "LuaMapObject.h"
#include "LuaMapObjManager.h"
#include "../GameFuncs/graphics/Terrain.h"

MapObjManager manager;

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

    m_scriptPath = path;
    HANDLE hFile = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        GetFileTime(hFile, NULL, NULL, &m_lastModTime);
        CloseHandle(hFile);
    }

    return true;
}

bool LuaEngine::CheckAndReload()
{
    if (m_scriptPath.empty())
    {
        return false;
    }

    HANDLE hFile = CreateFileA(m_scriptPath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    FILETIME currentModTime;
    GetFileTime(hFile, NULL, NULL, &currentModTime);
    CloseHandle(hFile);

    if (CompareFileTime(&currentModTime, &m_lastModTime) != 0)
    {
        Log::Client::Write("[LUA ENGINE]: Reloading %s", m_scriptPath.c_str());
        return LoadScript(m_scriptPath);
    }

    return false;
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
    auto setFuncs = [](lua_State* L, const std::vector<std::pair<const char*, lua_CFunction>>& funcs)
    {
        lua_newtable(L);
        for (const std::pair<const char*, lua_CFunction>& p : funcs)
        {
            lua_pushcfunction(L, p.second);
            lua_setfield(L, -2, p.first);
        }
    };

    setFuncs(L, {});//Log table
    lua_newtable(L);//Client table
    setFuncs(L, {{"Write", Lua_LogClientWrite}});//Write to Client
    lua_setfield(L, -2, "Client");//Log.Client = Client
    lua_setglobal(L, "Log");//_G.Log = Log

    //Console
    setFuncs(L,
        {
            {"ProcessCmd", Lua_ConsoleProcessCmd},
            {"Message",    Lua_ConsoleMessage},
            {"SetType",    Lua_ConsoleSetType}
        });
    lua_setglobal(L, "Console");

    //VarSys
    setFuncs(L, { {"CreateCmd", Lua_VarSysCreateCmd} });
    lua_setglobal(L, "VarSys");

    lua_pushcfunction(L, Lua_Sleep);
    lua_setglobal(L, "sleep");

    lua_pushcfunction(L, Lua_TmpFn);
    lua_setglobal(L, "testterrain");

    RegisterMapObjManager(L, manager);
}

int LuaEngine::Lua_TmpFn(lua_State* L)
{
    Terrain::RenderTerrainMap("NewMap.tga", 1024, 0x1, 0x1);
    return 0;
}

int LuaEngine::Lua_ConsoleSetType(lua_State* L)
{
    //Stupid fix cause LuaJIT is "technically" 31bit not 32
    const char* str = luaL_checkstring(L, 1);
    U32 type = strtoul(str, nullptr, 0);
    Console::SetType(type);
    return 0;
}

int LuaEngine::Lua_ConsoleMessage(lua_State* L)
{
    const char* msg = luaL_checkstring(L, 1);
    Console::Message("%s", msg);
    return 0;
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
    VarSys::CreateCmd(name, 0, 0);
    return 0;
}

int LuaEngine::Lua_ConsoleProcessCmd(lua_State* L)
{
    const char* cmd = luaL_checkstring(L, 1);

    int a2 = 0;
    int a3 = 0;

    Console::ProcessCmd(cmd, a2, a3);
    return 0;
}

int LuaEngine::Lua_Sleep(lua_State* L)
{
    int ms = luaL_checkinteger(L, 1);
    Sleep(ms);
    return 0;
}