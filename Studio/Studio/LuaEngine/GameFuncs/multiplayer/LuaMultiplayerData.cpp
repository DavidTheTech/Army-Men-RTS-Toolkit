#include "LuaMultiplayerData.h"

int Lua_MultiplayerDataSend(lua_State* L)
{
    if (lua_gettop(L) != 1)
    {
        luaL_error(L, "Multiplayer.Data.Send() needs exactly 1 argument (string)");
        return 0;
    }

    size_t msgLen;
    const char* msg = luaL_checklstring(L, 1, &msgLen);

    if (msgLen == 0)
    {
        return 0;
    }

    const unsigned char* data = reinterpret_cast<const unsigned char*>(msg);
    MultiPlayer::Data::Send(0xFEC65C5, (int)msgLen, data, 0);

    return 0;
}
