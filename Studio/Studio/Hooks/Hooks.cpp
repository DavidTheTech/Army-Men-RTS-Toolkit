#include "Hooks.h"
#include "..\Settings.h"
#include "..\GameFuncs\graphics\Vid.h"
#include "..\GameFuncs\system\Log.h"
#include "..\Patches\Patches.h"
#include "..\GameFuncs\main\Main.h"
#include "..\Handlers\Handlers.h"
#include "..\GameFuncs\graphics\Terrain.h"
#include "..\GameFuncs\system\defines.h"
#include "..\GameFuncs\coregame\Team.h"
#include "..\GameFuncs\coregame\releation.h"
#include "../Game/studio_brush_terrainGen.h"
#include "../Game/StudioGame.h"
#include "../GameFuncs/system/Debug.h"

typedef void(__fastcall* InitBuckets_t)(unsigned int count, unsigned int size, float ratio, int flush, unsigned int tcount, unsigned int tsize, float tratio);
static InitBuckets_t realInitBuckets = nullptr;
static InitBuckets_t hookInitBuckets = reinterpret_cast<InitBuckets_t>(0x432010);

typedef void(__fastcall* HeapInit_t)(unsigned long maxVtx, unsigned long maxIdx);
static HeapInit_t realHeapInit = nullptr;
static HeapInit_t hookHeapInit = reinterpret_cast<HeapInit_t>(0x418580);

typedef void(__fastcall* CoreGameInit_t)();
static CoreGameInit_t realCoreGameInit = nullptr;
static CoreGameInit_t hookCoreGameInit = reinterpret_cast<CoreGameInit_t>(0x53D710);

typedef bool(__cdecl* Vid_ToggleWindowedMode_t)();
static Vid_ToggleWindowedMode_t realVidToggleWindowedMode = nullptr;
static Vid_ToggleWindowedMode_t hookVidToggleWindowedMode = reinterpret_cast<Vid_ToggleWindowedMode_t>(0x41DD40);

typedef HWND(__fastcall* Main_CreateGameWindow_t)(LPCSTR lpWindowName, LPCSTR lpClassName);
static Main_CreateGameWindow_t realMainCreateGameWindow = nullptr;
static Main_CreateGameWindow_t hookMainCreateGameWindow = reinterpret_cast<Main_CreateGameWindow_t>(0x4B4770);

typedef int(__stdcall* WndProc_t)(HWND hWnd, UINT Msg, int wParam, unsigned int lParam);
static WndProc_t Main_WndProc = reinterpret_cast<WndProc_t>(0x4B44F0);

typedef HWND(__cdecl* CreateMainWindow_t)();
static CreateMainWindow_t realCreateMainWindow = nullptr;
static CreateMainWindow_t hookCreateMainWindow = reinterpret_cast<CreateMainWindow_t>(0x401CD0);

//main\runcodes
typedef void(__thiscall* RunCodesSet_U32_t)(DWORD* runCodes, U32 runcode);
static RunCodesSet_U32_t realRunCodesSet_U32 = nullptr;
static RunCodesSet_U32_t hookRunCodesSet_U32 = reinterpret_cast<RunCodesSet_U32_t>(0x4B6750);

typedef void(__thiscall* RunCodesSet_S_t)(DWORD* runCodes, const char *runcode);
static RunCodesSet_S_t realRunCodesSet_S = nullptr;
static RunCodesSet_S_t hookRunCodesSet_S = reinterpret_cast<RunCodesSet_S_t>(0x4B6700);
//END

//graphics\terrain
typedef void(__fastcall* TerrainCmdHandler_t)(U32 pathCrc);
static TerrainCmdHandler_t realTerrainCmdHandler = nullptr;
static TerrainCmdHandler_t hookTerrainCmdHandler = reinterpret_cast<TerrainCmdHandler_t>(0x434B40);
//END

//CRC
typedef U32(__fastcall* CrcCalcStr_t)(const char* str, U32 crc);
static CrcCalcStr_t realCrcCalcStr = nullptr;
static CrcCalcStr_t hookCrcCalcStr = reinterpret_cast<CrcCalcStr_t>(0x4D9A30);
//END

typedef int(__fastcall* Vid_SetMode_t)(U32 mode, U32 width, U32 height, bool force);
static Vid_SetMode_t realVidSetMode = nullptr;
static Vid_SetMode_t hookVidSetMode = reinterpret_cast<Vid_SetMode_t>(0x41DD50);


//Studio

//brushes
typedef void(__cdecl* Studio_Brush_t)();
static Studio_Brush_t realStudioBrushCreateBrushes = nullptr;
static Studio_Brush_t hookStudioBrushCreateBrushes = reinterpret_cast<Studio_Brush_t>(0x5BD450);

//
typedef void* (__cdecl* AllocMemOPNew2_t)(size_t bytes);
static AllocMemOPNew2_t AllocMemOPNew2 = (AllocMemOPNew2_t)Memory::ScanAddress(0x4CFAB0);

typedef void(__thiscall* CreateBrush_t)(void* mem);
static CreateBrush_t CreateBrush_Fn = (CreateBrush_t)(Memory::ScanAddress(0x5BD6D0));

static void __cdecl detourCreateBrushes()
{
    realStudioBrushCreateBrushes();

    void* mem = Debug::Memory::NewOperator(0xFF); //we'll just use this size for now
    if (mem)
    {
        Studio::Brush::TerrainGen* brush = new (mem) Studio::Brush::TerrainGen(mem, "TerrainGen");
        CreateBrush_Fn(brush);
    }
}

typedef void(__cdecl* StudioCmdInit_t)();
static StudioCmdInit_t realStudioCmdInit = nullptr;
static StudioCmdInit_t hookStudioCmdInit = reinterpret_cast<StudioCmdInit_t>(0x5E49D0);

typedef void(__cdecl* StudioCmdDone_t)();
static StudioCmdDone_t realStudioCmdDone = nullptr;
static StudioCmdDone_t hookStudioCmdDone = reinterpret_cast<StudioCmdDone_t>(0x5E50F0);


//amrts.log hook
typedef void(__cdecl* LOG_DIAG_t)(const char* format, ...);
static LOG_DIAG_t realLOG_DIAG = nullptr;
static LOG_DIAG_t hookLOG_DIAG = reinterpret_cast<LOG_DIAG_t>(0x4CFD00);

static HANDLE g_hLogFile = INVALID_HANDLE_VALUE;
static CRITICAL_SECTION g_logCriticalSection;

static void WriteToLogFile(const char* buffer, size_t len)
{
    if (g_hLogFile == INVALID_HANDLE_VALUE)
        return;

    DWORD bytesWritten;
    WriteFile(g_hLogFile, buffer, (DWORD)len, &bytesWritten, NULL);
}

static void InitLogFile()
{
    if (g_hLogFile != INVALID_HANDLE_VALUE)
        return;

    CreateDirectoryA(".\\logs", NULL);

    SYSTEMTIME st;
    GetLocalTime(&st);

    char timestamp[64];
    sprintf_s(timestamp, sizeof(timestamp), "%04d-%02d-%02d_%02d.%02d.%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

    char path[256];
    sprintf_s(path, sizeof(path), ".\\logs\\amrts-%s.log", timestamp);

    g_hLogFile = CreateFileA(path, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (g_hLogFile != INVALID_HANDLE_VALUE)
    {
        const char bom[] = { (char)0xEF, (char)0xBB, (char)0xBF };
        WriteToLogFile(bom, 3);
    }
}

static void __cdecl detourLOG_DIAG(const char* format, ...)
{
    static bool bInitialized = false;
    if (!bInitialized)
    {
        InitializeCriticalSection(&g_logCriticalSection);
        InitLogFile();
        bInitialized = true;
    }

    EnterCriticalSection(&g_logCriticalSection);

    char buffer[2048];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer) - 2, format, args);
    va_end(args);

    SYSTEMTIME st;
    GetLocalTime(&st);

    char timestamp[32];
    sprintf_s(timestamp, sizeof(timestamp), "[%02d:%02d:%02d.%03d] ", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    if (g_hLogFile != INVALID_HANDLE_VALUE)
    {
        WriteToLogFile(timestamp, (DWORD)strlen(timestamp));
        WriteToLogFile(buffer, (DWORD)strlen(buffer));
        WriteToLogFile("\r\n", 2);
        FlushFileBuffers(g_hLogFile);
    }

    if (IsDebuggerPresent())
    {
        char debugBuffer[2112];
        sprintf_s(debugBuffer, sizeof(debugBuffer), "%s%s\r\n", timestamp, buffer);
        OutputDebugStringA(debugBuffer);
    }

    LeaveCriticalSection(&g_logCriticalSection);
}

static void __cdecl detourStudioCmdInit()
{
    realStudioCmdInit();
    StudioGame::SetupVars();
    //return 1;
}

static void __cdecl detourStudioCmdDone()
{
    realStudioCmdDone();
}

//test shit x9999
struct Vector
{
    float x, y, z;
};

typedef void(__thiscall* Blip_t)(void* pThis, Color color, F32 persistTime);
static Blip_t realBlip = nullptr;
static Blip_t hookBlip = reinterpret_cast<Blip_t>(0x005BB160);

static void __fastcall detourBlip(void* pThis, int /*edx*/, Color color, F32 persistTime)
{
    printf("Blip: this=%p, color=(%d,%d,%d,%d), persistTime=%f\n", pThis, color.r, color.g, color.b, color.a, persistTime);
    realBlip(pThis, color, persistTime);
}
//end

typedef DWORD* (__fastcall* Team_GetRelationColor_t)(DWORD* pThis, void* /*edx*/, DWORD* outColor, DWORD* team2);
static Team_GetRelationColor_t realGetRelationColor = nullptr;
static Team_GetRelationColor_t hookGetRelationColor = reinterpret_cast<Team_GetRelationColor_t>(0x547710);

static DWORD* __fastcall detourGetRelationColor(DWORD* pThis, void* /*edx*/, DWORD* outColor, DWORD* team2)
{
    DWORD relation = Team::GetRelation(pThis, team2);

    ColorConfig* cfg = nullptr;

    switch (relation)
    {
    case Relation::ALLY:
        cfg = &g_settings.allyColor;
        break;

    case Relation::ENEMY:
        cfg = &g_settings.enemyColor;
        break;

    default:
        cfg = &g_settings.neutralColor;
        break;
    }

    Color color;
    color.r = cfg->r;
    color.g = cfg->g;
    color.b = cfg->b;
    color.a = cfg->a;

    *outColor = color.color;

    return outColor;
}


static int __fastcall detourVidSetMode(U32 mode, U32 width, U32 height, bool force)
{
    printf("mode: %d, width: %d, height: %d, force : %d\n", mode, width, height, force);
    width = 1920;
    force = true;
    printf("UPDATED mode: %d, width: %d, height: %d, force : %d\n", mode, width, height, force);
    return realVidSetMode(mode, width, height, force);
}

static U32 __fastcall detourCrcCalcStr(const char* str, U32 crc)
{
    U32 result = realCrcCalcStr(str, crc);
    if (result == 0x5C007B75)
    {
        U32 result2 = realCrcCalcStr("s", 0);
        printf("%s : [OLD : 0x%08X] : [NEW 0x%08X]\n", str, result, result2);
        result = 0x5C007B75;
    }
    else
    {
        result = realCrcCalcStr(str, crc);
    }
    //printf("%s : 0x%08X\n", str, crc2);
    return result;
}

//Render the minimap using a diff func
//RES GOOD, quality is poor - needs work
static void __fastcall detourTerrainCmdHandler(U32 pathCrc)
{
    //printf("%02X\n", pathCrc);
    switch (pathCrc)
    {
        case 0xB56030F2:
        {
            Terrain::RenderTerrainMap("NewMap.tga", 1024, 0x1, 0x1);
            //break;
        }
        default:
            break;
    }
    return realTerrainCmdHandler(pathCrc);
}

static void detourRunCodesSet_U32(DWORD* runCodes, U32 runcode)
{
    Handlers handler;
    handler.LoadHandlers();
    return realRunCodesSet_U32(runCodes, runcode);
}

static void detourRunCodesSet_S(DWORD* runCodes, const char* runcode)
{
    Handlers handler;
    handler.LoadHandlers();
    return realRunCodesSet_S(runCodes, runcode);
}

static HWND __cdecl detourCreateMainWindow()
{
    return Main::CreateGameWindow("Army Men RTS Studio", "AMRTS_STUDIO");
}

//allows multiple instances
static HWND __fastcall detourMainCreateGameWindow(LPCSTR lpWindowName, LPCSTR lpClassName)
{
    HWND WindowA;
    HMODULE ModuleHandleA;
    WNDCLASSA wc = {};

    HINSTANCE hI = *reinterpret_cast<HINSTANCE*>(Memory::ScanAddress(0x728A94));

    //if (dword_728AB0 || (WindowA = FindWindowA(lpClassName, 0)) == 0) //Old logic

    wc.style = 3;
    wc.lpfnWndProc = reinterpret_cast<WNDPROC>(Main_WndProc);
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hI;
    ModuleHandleA = GetModuleHandleA(0);
    wc.hIcon = LoadIconA(ModuleHandleA, (LPCSTR)0x65);
    wc.hCursor = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(4);
    wc.lpszMenuName = 0;
    wc.lpszClassName = lpClassName;
    
    if (RegisterClassA(&wc))
    {
        return CreateWindowExA(0, lpClassName, lpWindowName, 0xC00000u, 0, 0, 0, 0, 0, 0, hI, 0);
    }
    
    return 0;
    
}

//need to do more DX stuff to force window mode dynamically without tools like dgVoodoo or DXWIN
static bool __cdecl detourToggleWindowedMode()
{
    static U32* curMode = reinterpret_cast<U32*>(0x6BD59C);
    static U32* curDD = reinterpret_cast<U32*>(0x6BD5A0);
    U32 VIDMODEWINDOW = 254;

    if (*curMode == VIDMODEWINDOW)
    {
        //Log::Client::Write("ab");
        //Vid::InitDD(true);
        //Log::Client::Write("bb");

        //Attempting to RaiseException "FATAL" msg:SetMode; ddx->SetDisplayMode module:D:\Src\code\graphics\vid.cpp line:489

        //return Vid::SetMode(VIDMODEWINDOW, 800, 600, true);
    }

    return realVidToggleWindowedMode();
}

//gpu memory limits
//uses Settings.cpp/.h for custom values
static void __fastcall detourInitBuckets(unsigned long count, unsigned long size, float ratio, int flush, unsigned long tcount, unsigned long tsize, float tratio)
{
    unsigned long DEF_BUCKET_COUNT = 444 * g_settings.InitBucketMulti;
    unsigned long DEF_BUCKET_SIZE = 16000 * g_settings.InitBucketMulti;
    unsigned long DEF_TRAN_BUCKET_COUNT = 200 * g_settings.InitBucketMulti;
    unsigned long DEF_TRAN_BUCKET_SIZE = 16000 * g_settings.InitBucketMulti;
    float DEF_BUCKET_RATIO = 0.95f;
    bool bflush = true;

    realInitBuckets(DEF_BUCKET_COUNT, DEF_BUCKET_SIZE, DEF_BUCKET_RATIO, bflush, DEF_TRAN_BUCKET_COUNT, DEF_TRAN_BUCKET_SIZE, DEF_BUCKET_RATIO);
}

//^^^^^^
static void __fastcall detourHeapInit(unsigned long maxVtx, unsigned long maxIdx)
{
    //printf("maxVtx : %lu\tmaxIdx : %lu\n", maxVtx, maxIdx);
    maxVtx = g_settings.InitBucketVert * g_settings.InitBucketMulti;
    maxIdx = g_settings.InitBucketIndi * g_settings.InitBucketMulti;

    realHeapInit(maxVtx, maxIdx);
}

//Create missing Cmds on each call of Coregame::Init() as coregame.* gets deleted
static void __fastcall detourCoreGameInit()
{
    //printf("coregame.* init\n");
    VarSys::CreateCmd("coregame.createobj");
    VarSys::CreateCmd("coregame.listobjs");
    VarSys::CreateCmd("coregame.listoffmap");
    VarSys::CreateCmd("coregame.listbyclassid");
    VarSys::CreateCmd("coregame.listclusters");
    VarSys::CreateCmd("coregame.listunits");
    VarSys::CreateCmd("coregame.removeobj");
    VarSys::CreateCmd("coregame.addobj");
    //printf("coregame.* init done\n");

    realCoreGameInit();
}


//mainly for amrts.log
//maybe for extra stuff after
void Hooks::SetupDirs()
{
    //for amrts.log handler
    CreateDirectoryA("./logs", NULL);
    //CreateDirectoryA("./logs/amrts", NULL);
    //CreateDirectoryA("./logs/studio", NULL);
}


//HOOKS NAMING CONVENTION
//detours SHOULD be named detourFuncName
//hook SHOULD be the typedef to the funcs address IN-GAME
//real SHOULD be the return to the real func IN-GAME (if needed)
//
// EXAMPLE
// creating a hook copy the following as a template
// if (MH_CreateHook(reinterpret_cast<void*>(hookFUNCNAME), &detourFUNCNAME, reinterpret_cast<void**>(&realFUNCNAME)) != MH_OK) return 1;
//
// enabling the hook copy the following as a template
// if (MH_EnableHook(reinterpret_cast<void*>(hookFUNCNAME)) != MH_OK) return 1;
//
// Any errors SHOULD be printed to the console instead of Log::Client::Write Due to if a hook fails you'll still get the error output
// Later versions will have studio logs (ERROR/INFO) will be put in /logs/studio

bool Hooks::Setup()
{
    MH_STATUS status = MH_Initialize();
    if (status != MH_OK)
    {
        std::string sStatus = MH_StatusToString(status);
        //log to console just incase of failure of hooks
        printf("Minhook init failed! \nERR:%s\n", sStatus.c_str());
        return 0;
    }

    //amrts.log + others
    SetupDirs();

    //Fix the order of hooks, fastest hook at top, low prio at bottom
    //same for enable hooks

    //anrts.log
    if (MH_CreateHook(reinterpret_cast<void*>(hookLOG_DIAG), &detourLOG_DIAG, reinterpret_cast<void**>(&realLOG_DIAG)) != MH_OK) return 1;

    //multiple instances + custom window title
    if (MH_CreateHook(reinterpret_cast<void*>(hookCreateMainWindow), &detourCreateMainWindow, reinterpret_cast<void**>(&realCreateMainWindow)) != MH_OK) return 1;
    if (MH_CreateHook(reinterpret_cast<void*>(hookMainCreateGameWindow), &detourMainCreateGameWindow, reinterpret_cast<void**>(&realMainCreateGameWindow)) != MH_OK) return 1;
    
    //missing cmds
    if (MH_CreateHook(reinterpret_cast<void*>(hookCoreGameInit), &detourCoreGameInit, reinterpret_cast<void**>(&realCoreGameInit)) != MH_OK) return 1;
    
    //Memory/GPU limits
    if (MH_CreateHook(reinterpret_cast<void*>(hookInitBuckets), &detourInitBuckets, reinterpret_cast<void**>(&realInitBuckets)) != MH_OK) return 1;
    if (MH_CreateHook(reinterpret_cast<void*>(hookHeapInit), &detourHeapInit, reinterpret_cast<void**>(&realHeapInit)) != MH_OK) return 1;

    //Not done
    if (MH_CreateHook(reinterpret_cast<void*>(hookVidToggleWindowedMode), &detourToggleWindowedMode, reinterpret_cast<void**>(&realVidToggleWindowedMode)) != MH_OK) return 1;
    
    //test stuff
    //if (MH_CreateHook(reinterpret_cast<void*>(hookRunCodesSet_U32), &detourRunCodesSet_U32, reinterpret_cast<void**>(&realRunCodesSet_U32)) != MH_OK) return 1;
    //if (MH_CreateHook(reinterpret_cast<void*>(hookRunCodesSet_S), &detourRunCodesSet_S, reinterpret_cast<void**>(&realRunCodesSet_S)) != MH_OK) return 1;
    
    //Better studio minimap rendering
    if (MH_CreateHook(reinterpret_cast<void*>(hookTerrainCmdHandler), &detourTerrainCmdHandler, reinterpret_cast<void**>(&realTerrainCmdHandler)) != MH_OK) return 1;
    //if (MH_CreateHook(reinterpret_cast<void*>(hookCrcCalcStr), &detourCrcCalcStr, reinterpret_cast<void**>(&realCrcCalcStr)) != MH_OK) return 1;

    //window mode stuff
    //if (MH_CreateHook(reinterpret_cast<void*>(hookVidSetMode), &detourVidSetMode, reinterpret_cast<void**>(&realVidSetMode)) != MH_OK) return 1;
    //if (MH_CreateHook(reinterpret_cast<void*>(hookUnitConstructor), &detourQueueAdd, reinterpret_cast<void**>(&realUnitConstructor)) != MH_OK) return 1;
    
    //if (MH_CreateHook(reinterpret_cast<void*>(hookBlip), &detourBlip, reinterpret_cast<void**>(&realBlip)) != MH_OK) return 1;

    //minimap blip color (testing)
    //if (MH_CreateHook(reinterpret_cast<void*>(hookGetRelationColor), &detourGetRelationColor, reinterpret_cast<void**>(&realGetRelationColor)) != MH_OK) return 1;

    //hook for adding new custom brushes
    if (MH_CreateHook(reinterpret_cast<void*>(hookStudioBrushCreateBrushes), &detourCreateBrushes, reinterpret_cast<void**>(&realStudioBrushCreateBrushes)) != MH_OK) return 1;
    
    //add new custom studio vars (like custom vars for the new custom brushes)
    if (MH_CreateHook(reinterpret_cast<void*>(hookStudioCmdInit), &detourStudioCmdInit, reinterpret_cast<void**>(&realStudioCmdInit)) != MH_OK) return 1;
    if (MH_CreateHook(reinterpret_cast<void*>(hookStudioCmdDone), &detourStudioCmdDone, reinterpret_cast<void**>(&realStudioCmdDone)) != MH_OK) return 1;

    //amrts.log
    if (MH_EnableHook(reinterpret_cast<void*>(hookLOG_DIAG)) != MH_OK) return 1;

    if (MH_EnableHook(reinterpret_cast<void*>(hookCreateMainWindow)) != MH_OK) return 1;
    if (MH_EnableHook(reinterpret_cast<void*>(hookMainCreateGameWindow)) != MH_OK) return 1;
    if (MH_EnableHook(reinterpret_cast<void*>(hookCoreGameInit)) != MH_OK) return 1;
    if (MH_EnableHook(reinterpret_cast<void*>(hookInitBuckets)) != MH_OK) return 1;
    if (MH_EnableHook(reinterpret_cast<void*>(hookHeapInit)) != MH_OK) return 1;
    if (MH_EnableHook(reinterpret_cast<void*>(hookVidToggleWindowedMode)) != MH_OK) return 1;
    //if (MH_EnableHook(reinterpret_cast<void*>(hookRunCodesSet_U32)) != MH_OK) return 1;
    //if (MH_EnableHook(reinterpret_cast<void*>(hookRunCodesSet_S)) != MH_OK) return 1;
    if (MH_EnableHook(reinterpret_cast<void*>(hookTerrainCmdHandler)) != MH_OK) return 1;
    //if (MH_EnableHook(reinterpret_cast<void*>(hookCrcCalcStr)) != MH_OK) return 1;

    //if (MH_EnableHook(reinterpret_cast<void*>(hookVidSetMode)) != MH_OK) return 1;
    //if (MH_EnableHook(reinterpret_cast<void*>(hookUnitConstructor)) != MH_OK) return 1;
    
    //if (MH_EnableHook(reinterpret_cast<void*>(hookBlip)) != MH_OK) return 1;
    if (MH_EnableHook(reinterpret_cast<void*>(hookGetRelationColor)) != MH_OK) return 1;

    if (MH_EnableHook(reinterpret_cast<void*>(hookStudioBrushCreateBrushes)) != MH_OK) return 1;
    
    if (MH_EnableHook(reinterpret_cast<void*>(hookStudioCmdInit)) != MH_OK) return 1;
    if (MH_EnableHook(reinterpret_cast<void*>(hookStudioCmdDone)) != MH_OK) return 1;



    /*
    if (MH_CreateHook(reinterpret_cast<void**>(hookVidRenderBegin), &detourVidRenderBegin, reinterpret_cast<void**>(&realVidRenderBegin)) != MH_OK) return 1;
    if (MH_CreateHook(reinterpret_cast<void**>(hookVidRenderEnd), &detourVidRenderEnd, reinterpret_cast<void**>(&realVidRenderEnd)) != MH_OK) return 1;
    */

    return true;
}