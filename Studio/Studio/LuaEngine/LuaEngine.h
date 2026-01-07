#pragma once
#include <LuaJIT/Src/lua.hpp>
#include <string>

class LuaEngine
{
public:
    LuaEngine();
    ~LuaEngine();

    bool Initialize();
    bool LoadScript(const std::string& path);
    bool ExecuteString(const std::string& code);

    void SetVariable(const std::string& name, int value);

private:
    lua_State* L;

private:
    void RegisterFunctions();

    static int Lua_LogClientWrite(lua_State* L);
};
