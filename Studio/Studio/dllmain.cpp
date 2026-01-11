#include <windows.h>

#include "Hooks\Hooks.h"
#include "Patches\Patches.h"
#include "LuaEngine\LuaEngine.h"
#include "Settings.h"

#include "GameFuncs\system\defines.h"

#include "GameFuncs\game\GameGod.h"
#include "GameFuncs\main\Runcodes.h"
#include "GameFuncs\coregame_interface\Studio.h"
#include "GameFuncs\system\Log.h"
#include "GameFuncs\system\Utils.h"
#include "GameFuncs\graphics\Vid.h"
#include "GameFuncs\graphics\Terrain.h"
#include "GameFuncs\multiplayer\MultiPlayer_Data.h"
#include "GameFuncs\multiplayer\MultiPlayer_Host.h"
#include "GameFuncs\system\Crc.h"
#include "GameFuncs\styxnet\Styxnet_Client.h"
#include "GameFuncs\util\Console.h"
#include "GameFuncs\multiplayer\MultiPlayer_Host.h"
#include "Server\Server.h"
#include "GameFuncs\system\Debug.h"
#include "GameFuncs\interface\IFace_Util.h"

HANDLE SetupEverythingHandle = NULL;
HANDLE MinHookHandle = NULL;
HANDLE LuaEngineHandle = NULL;
HANDLE MultiplayerServerHandle = NULL;

inline void WaitForTrue(volatile bool* flag, DWORD sleepMs = 1)
{
    while (!*flag)
    {
        Sleep(sleepMs);
    }
}

void SetupLuaEngine()
{
    WaitForTrue(GameGod::IsInitialized());

    LuaEngine lua;
    lua.Initialize();

    lua.SetVariable("testvar", 42);
    lua.LoadScript("main.lua");
}

DWORD WINAPI MultiplayerStartup(LPVOID lpParam)
{
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

    int result = MessageBoxA(NULL, "Do you want to start the server launcher?", "Server Initialization", MB_YESNO | MB_ICONQUESTION);
    if (result == IDYES)
    {
        Server* server = new Server();
        server->init();
        delete server;
    }

    CoUninitialize();
    return 0;
}

void SetupEverything()
{
    Settings settings;
    settings.LoadJson();

    Patches::ApplyAll();

    WaitForTrue(GameGod::IsInitialized());

    //Studio
    DWORD runCodes = 0x7288E0;
    RunCodes::Register((DWORD*)runCodes, "Studio", (int)Studio::Process, (int)Studio::Init, (int)Studio::Done, (int)Studio::PostInit, 0);

    if (settings.AutoLaunch)
    {
        Sleep(2000);
        RunCodes::Set((DWORD*)runCodes, "Studio");
    }

    //Add missing cmds
    VarSys::CreateCmd("team.list", 0, 0);

    //these exist but their handler doesnt contain their code
    //TODO: hook 0x490030 check if crc matches arg n call our own handler if not continue
    VarSys::CreateCmd("iface.setalpha");
    VarSys::CreateCmd("iface.fadeup");
    VarSys::CreateCmd("iface.testmodechange");
    VarSys::CreateCmd("iface.testmsgbox");

    IFace::ScreenDump();
}

void LaunchMP()
{
    Sleep(10000);
    Console::ProcessCmd("multiplayer.session.createdownload TestServ", 0, 0);
    Console::ProcessCmd("multiplayer.server.start", 0, 0);
    Sleep(5000);
    Console::ProcessCmd("multiplayer.setup.setrandommission missions\\mp", 0, 0);

    const unsigned char msg[] = "Locking in 15 seconds";
    MultiPlayer::Data::Send(0x0FEC65C5, sizeof(msg) + 1, msg, false);
    Sleep(15000);
    StyxNet::Client::LockSession();

    MultiPlayer::Host::FillAITeams();
    MultiPlayer::Data::Send(0x19AA2502, 0, NULL, TRUE);
    while (!MultiPlayer::Host::CheckLaunch())
    {
        const unsigned char msg2[] = "Ready up, retrying in 5 seconds";
        MultiPlayer::Data::Send(0x0FEC65C5, sizeof(msg2) + 1, msg2, false);
        Sleep(5000);
    }
    Console::ProcessCmd("multiplayer.setup.launch", 0, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        //IMPORTANT HOOKS ARE THE FIRST THING TO BE SETUP
        Log::Client::Write("[STUDIO DLL]: Hooks::Setup");
        MinHookHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Hooks::Setup, NULL, 0, NULL);

        Log::Client::Write("[STUDIO DLL]: SetupEverything");
        SetupEverythingHandle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SetupEverything, NULL, NULL, NULL);

        Log::Client::Write("[STUDIO DLL]: Lua Engine");
        LuaEngineHandle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SetupLuaEngine, NULL, NULL, NULL);

        Log::Client::Write("[STUDIO DLL]: Multiplayer Controls");
        MultiplayerServerHandle = CreateThread(NULL, 0, MultiplayerStartup, NULL, 0, NULL);
        SetThreadPriority(MultiplayerServerHandle, THREAD_PRIORITY_BELOW_NORMAL);
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}