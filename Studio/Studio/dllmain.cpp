#include "main.h"

HANDLE SetupEverythingHandle = NULL;
HANDLE MinHookHandle = NULL;
HANDLE LuaEngineHandle = NULL;
HANDLE MultiplayerServerHandle = NULL;
HANDLE CursorLockHandle = NULL;
HANDLE HandlersHandle = NULL;
HANDLE TestingHandle = NULL;

const char* StudioDLL::credits = R"(Army Men RTS Studio Toolkit created by @DavidTheTech

Development help from
@TommyCD1
@Minty

The source code is available at https://github.com/DavidTheTech/Army-Men-RTS-Toolkit
)";

void SetupLuaEngine()
{
    Internals::WaitForTrue(GameGod::IsInitialized());

    LuaEngine lua;
    lua.Initialize();

    lua.SetVariable("testvar", 42);
    lua.LoadScript("main.lua");

    while (true)
    {
        lua.CheckAndReload();
        lua.LuaLoop();
        Sleep(100);
    }
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

void LockCursor(int cursorLockTimer)
{
    EnumWindows(EnumWindowsProc, 0);
    if (targetWindow == NULL)
    {
        return; 
    }

    while (true)
    {
        GetWindowDimensions(targetWindow);
        if (IsWindowOnTop(targetWindow))
        {
            LockCursorInWindow();
        }
        Sleep(cursorLockTimer);
    }
}

void SetupEverything()
{
    g_settings.LoadJson();

    GameConsole gConsole;
    gConsole.Start();    

    Internals::WaitForTrue(GameGod::IsInitialized());

    //Studio
    DWORD runCodes = 0x7288E0;
    RunCodes::Register((DWORD*)runCodes, "Studio", (int)Studio::Process, (int)Studio::Init, (int)Studio::Done, (int)Studio::PostInit, 0);

    if (g_settings.AutoLaunch)
    {
        Sleep(2000);
        RunCodes::Set((DWORD*)runCodes, "Studio");
    }

    if (g_settings.DoWeLockCursor)
    {
        CursorLockHandle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)LockCursor, (LPVOID)(size_t)g_settings.CursorLockTimer, NULL, NULL);
    }

    //Add missing cmds
    VarSys::CreateCmd("team.list");
    VarSys::CreateCmd("terrain.toggle.shroud");

    //these exist but their handler doesnt contain their code
    //TODO: hook 0x490030 check if crc matches arg n call our own handler if not continue
    VarSys::CreateCmd("iface.setalpha");
    VarSys::CreateCmd("iface.fadeup");
    VarSys::CreateCmd("iface.testmodechange");
    VarSys::CreateCmd("iface.testmsgbox");

    //DWORD* varTest = (DWORD*)VarSys::CreateString("sys.date", "swag", 0, &TommyTime, 0);

    //Custom vars
    VarSys::CreateString("studio.credits", StudioDLL::credits, VarSys::NOEDIT, nullptr, 0);
    //VarSys::CreateInteger("studio.numtest", 6458, 0, &studioNum, 0);
    
    //Connected regions are completely removed from amrts :(
    //VarSys::CreateCmd("coregame.cre.info");
    //VarSys::CreateInteger("coregame.cre.info.region", 0, 0, &varRegion, 0);

    //VarSys::CreateCmd("terrain.render.render");

    //setup Game stuff with gameloop
    g_game.Setup();
}

void HandlersFn()
{
    Handlers handlers;
    handlers.LoadHandlers();
}

void Testing()
{
    Testing::Test();

    /*

    MapObjManager manager;

    while (true)
    {
        manager.Update();

        for (auto& obj : manager.GetObjects())
        {
            printf("========\n");

            printf("ID: %d\n", obj.GetID());
            printf("Name: %s\n", obj.GetName());
            printf("Position: X: %.3f, Y: %.3f, Z: %.3f\n", obj.GetX(), obj.GetY(), obj.GetZ());
            printf("HP: %d/%d\n", obj.GetHitpoints(), obj.GetTotalHitpoints());

            obj.SetX(obj.GetX() + 1.0f);

            obj.SetHitpoints(1);

            auto resources = obj.GetResources();
            for (auto& res : resources)
            {
                std::cout << "Resource: " << res.type
                    << " Amount: " << res.amount << std::endl;
            }
            printf("========\n");
        }

        /*MapObject* myObj = manager.GetObjectByID(6458);
        if (myObj)
        {
            printf("Found object: %s", myObj->GetName());
            myObj->SetHitpoints(1000);
        }
        Sleep(2000);
    }*/
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(hModule);
        //IMPORTANT HOOKS ARE THE FIRST THING TO BE SETUP
        Log::Client::Write("[STUDIO DLL]: Hooks::Setup");
        Hooks::Setup();

        Log::Client::Write("[STUDIO DLL]: Patches::ApplyAll");
        Patches::ApplyAll();

        Log::Client::Write("[STUDIO DLL]: SetupEverything");
        SetupEverythingHandle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SetupEverything, NULL, NULL, NULL);

        Log::Client::Write("[STUDIO DLL]: Handlers");
        HandlersHandle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)HandlersFn, NULL, NULL, NULL);

        Log::Client::Write("[STUDIO DLL]: Lua Engine");
        LuaEngineHandle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SetupLuaEngine, NULL, NULL, NULL);

        Log::Client::Write("[STUDIO DLL]: Multiplayer Controls");
        MultiplayerServerHandle = CreateThread(NULL, 0, MultiplayerStartup, NULL, 0, NULL);
        SetThreadPriority(MultiplayerServerHandle, THREAD_PRIORITY_BELOW_NORMAL);

        Log::Client::Write("[STUDIO DLL]: TESTING");
        TestingHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Testing, NULL, NULL, NULL);
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}