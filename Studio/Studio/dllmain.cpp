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

//Move these & gameloop to its own class
VarSys::VarStringFake TommyTime;
VarSys::VarStringFake TommyTimeU;
VarSys::VarStringFake GGMissionName;
VarSys::VarStringFake GGMissionNameDate;
VarSys::VarStringFake GGGroup;

void GameLoop()
{
    Internals::WaitForTrue(GameGod::IsInitialized());
    Sleep(5000);
    while (true)
    {
        Sleep(10);
        //VarSys::SetString(&TommyTime, get_current_datetime());
        VarSys::SetString((DWORD*)&TommyTime, Internals::GetDate());
        VarSys::SetString((DWORD*)&TommyTimeU, Internals::GetDate(1));

        DWORD selected = Missions::GetSelected();

        if (selected)
        {
            const char* mapName = (const char*)(selected + 0x18);
            DWORD ptr = *(DWORD*)(selected + 0x14);
            const char* missionPath = nullptr;
            if (ptr)
            {
                missionPath = (const char*)(ptr + 0xC);
                VarSys::SetString((DWORD*)&GGGroup, missionPath);
            }

            //printf("Selected : [%s] [%s]\n", mapName ? mapName : "null", missionPath ? missionPath : "null");
        }
        //printf("addy mgs 0x%08x, %s\n", (addy + 0x18), reinterpret_cast<const char*>(addy + 0x18));
        //printf("addy mgs %s # %s\n", (const char*)((DWORD)selected + 0x18), (const char*)(*(DWORD*)((DWORD)selected + 0x14) + 0xC));
        //VarSys::SetString((DWORD*)&GGGroup, GameGod::IsInitialized);
        
        const char* MissionName = Missions::GetName();
        VarSys::SetString((DWORD*)&GGMissionName, MissionName);

        const char* MissionNameDate = Missions::GetNameDate();
        VarSys::SetString((DWORD*)&GGMissionNameDate, MissionNameDate);
    }
}

void SetupEverything()
{
    Settings settings;
    settings.LoadJson();

    GameConsole gConsole;
    gConsole.Start();    

    Internals::WaitForTrue(GameGod::IsInitialized());

    //Studio
    DWORD runCodes = 0x7288E0;
    RunCodes::Register((DWORD*)runCodes, "Studio", (int)Studio::Process, (int)Studio::Init, (int)Studio::Done, (int)Studio::PostInit, 0);

    if (settings.AutoLaunch)
    {
        Sleep(2000);
        RunCodes::Set((DWORD*)runCodes, "Studio");
    }

    if (settings.DoWeLockCursor)
    {
        CursorLockHandle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)LockCursor, (LPVOID)(size_t)settings.CursorLockTimer, NULL, NULL);
    }

    //Add missing cmds
    VarSys::CreateCmd("team.list", 0, 0);
    VarSys::CreateCmd("terrain.toggle.shroud", 0, 0);

    //these exist but their handler doesnt contain their code
    //TODO: hook 0x490030 check if crc matches arg n call our own handler if not continue
    VarSys::CreateCmd("iface.setalpha");
    VarSys::CreateCmd("iface.fadeup");
    VarSys::CreateCmd("iface.testmodechange");
    VarSys::CreateCmd("iface.testmsgbox");

    //DWORD* varTest = (DWORD*)VarSys::CreateString("sys.date", "swag", 0, &TommyTime, 0);

    //Custom vars
    VarSys::CreateString("sys.date", "swag", 0, &TommyTime, 0);
    VarSys::CreateString("sys.dateu", "swag", 0, &TommyTimeU, 0);
    VarSys::CreateString("gamegod.missions.name", "swag", 0, &GGMissionName, 0);
    VarSys::CreateString("gamegod.missions.namewithdate", "swag", 0, &GGMissionNameDate, 0);

    VarSys::CreateString("gamegod.missions.group", "N/A", 0, &GGGroup, 0);


    VarSys::CreateString("studio.credits", StudioDLL::credits, 0, nullptr, 0);

    //VarSys::CreateCmd("terrain.render.render");

    GameLoop();
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