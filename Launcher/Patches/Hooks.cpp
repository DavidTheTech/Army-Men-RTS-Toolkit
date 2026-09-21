#include "Hooks.h"
#include "memory.h"
#include "Settings.h"

typedef HWND(__fastcall* Main_CreateGameWindow_t)(LPCSTR lpWindowName, LPCSTR lpClassName);
static Main_CreateGameWindow_t realMainCreateGameWindow = nullptr;
static Main_CreateGameWindow_t hookMainCreateGameWindow = reinterpret_cast<Main_CreateGameWindow_t>(0x4B4770);

typedef HWND(__cdecl* CreateMainWindow_t)();
static CreateMainWindow_t realCreateMainWindow = nullptr;
static CreateMainWindow_t hookCreateMainWindow = reinterpret_cast<CreateMainWindow_t>(0x401CD0);

typedef int(__stdcall* WndProc_t)(HWND hWnd, UINT Msg, int wParam, unsigned int lParam);
static WndProc_t Main_WndProc = reinterpret_cast<WndProc_t>(0x4B44F0);

typedef HWND(__fastcall* CreateGameWindow2_t)(LPCSTR lpWindowName, LPCSTR lpClassName);
static CreateGameWindow2_t CreateGameWindowFn = (CreateGameWindow2_t)(Memory::ScanAddress(0x4B4770));

static HWND __cdecl detourCreateMainWindow()
{
    return CreateGameWindowFn("Army Men RTS", "AMRTS_STUDIO");
}

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
        //return CreateWindowExA(0, lpClassName, lpWindowName, 0xC00000u, 0, 0, 0, 0, 0, 0, hI, 0);
        //return CreateWindowExA(0, lpClassName, lpWindowName, WS_OVERLAPPEDWINDOW, 0, 0, 0, 0, 0, 0, hI, 0);
        return CreateWindowExA(0, lpClassName, lpWindowName, 0xC00000u | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, 0, 0, 0, 0, 0, 0, hI, 0);
    }

    return 0;

}

typedef void(__fastcall* InitBuckets_t)(unsigned int count, unsigned int size, float ratio, int flush, unsigned int tcount, unsigned int tsize, float tratio);
static InitBuckets_t realInitBuckets = nullptr;
static InitBuckets_t hookInitBuckets = reinterpret_cast<InitBuckets_t>(0x432010);

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

typedef void(__fastcall* HeapInit_t)(unsigned long maxVtx, unsigned long maxIdx);
static HeapInit_t realHeapInit = nullptr;
static HeapInit_t hookHeapInit = reinterpret_cast<HeapInit_t>(0x418580);

static void __fastcall detourHeapInit(unsigned long maxVtx, unsigned long maxIdx)
{
    //printf("maxVtx : %lu\tmaxIdx : %lu\n", maxVtx, maxIdx);
    maxVtx = g_settings.InitBucketVert * g_settings.InitBucketMulti;
    maxIdx = g_settings.InitBucketIndi * g_settings.InitBucketMulti;

    realHeapInit(maxVtx, maxIdx);
}


typedef int(__fastcall* Vid_SetMode_t)(unsigned long mode, unsigned long width, unsigned long height, bool force);
static Vid_SetMode_t realVidSetMode = nullptr;
static Vid_SetMode_t hookVidSetMode = reinterpret_cast<Vid_SetMode_t>(0x41DD50);

static int __fastcall detourVidSetMode(unsigned long mode, unsigned long width, unsigned long height, bool force)
{
    //Move to patches this skips over the if mode 22 check, if true it sets it back to 0
    //this skips over it
    //probs should NOP the area

    DWORD oldProtect;
    BYTE* patchAddr = (BYTE*)0x41DDAD;
    printf("Patch address: 0x%08X\n", patchAddr);

    VirtualProtect(patchAddr, 7, PAGE_EXECUTE_READWRITE, &oldProtect);
    *(BYTE*)patchAddr = 0xEB;

    VirtualProtect(patchAddr, 7, oldProtect, &oldProtect);


    printf("mode: %d, width: %d, height: %d, force : %d\n", mode, width, height, force);
    //width = 1920;
    //height = 1080;
    force = true;
    mode = 22;
    printf("UPDATED mode: %d, width: %d, height: %d, force : %d\n", mode, width, height, force);
    return realVidSetMode(mode, width, height, force);
}

typedef bool(__fastcall* Vid_SetMode2_t)(unsigned long mode, unsigned long width, unsigned long height, bool force);
static Vid_SetMode2_t SetMode_Fn = (Vid_SetMode2_t)(Memory::ScanAddress(0x41DD50));


typedef bool(__cdecl* Vid_ToggleWindowedMode_t)();
static Vid_ToggleWindowedMode_t realVidToggleWindowedMode = nullptr;
static Vid_ToggleWindowedMode_t hookVidToggleWindowedMode = reinterpret_cast<Vid_ToggleWindowedMode_t>(0x41DD40);

static bool __cdecl detourToggleWindowedMode()
{
    static unsigned long* curMode = reinterpret_cast<unsigned long*>(0x6BD59C);
    static unsigned long* curDD = reinterpret_cast<unsigned long*>(0x6BD5A0);
    unsigned long VIDMODEWINDOW = 254;

    if (*curMode == VIDMODEWINDOW)
    {
        return SetMode_Fn(VIDMODEWINDOW, 800, 600, true);
    }

    return realVidToggleWindowedMode();
}


//RegRunCode(runCodes, a2, a3, a4, a5, a6, a7);

typedef int(*StudioProcess)();
typedef int(__cdecl* StudioInt)();
typedef void(__cdecl* StudioDN)();
typedef void(__cdecl* StudioPostInt)();

static StudioProcess StudioPrss = (StudioProcess)(Memory::ScanAddress(0x5D02B0));
static StudioInt StudioInitFn = (StudioInt)(Memory::ScanAddress(0x5D01E0));
static StudioDN StudioDoneFn = (StudioDN)(Memory::ScanAddress(0x5D0270));
static StudioPostInt StudioPostInitFn = (StudioPostInt)(Memory::ScanAddress(0x5D0260));

typedef void(__thiscall* RegisterRuncode_t)(DWORD* thisptr, const char* a2, int a3, int a4, int a5, int a6, int a7);
static RegisterRuncode_t RegRunCode = (RegisterRuncode_t)(Memory::ScanAddress(0x4B6520));

typedef int(__thiscall* SetRuncode2_t)(DWORD* runCodes, const char* s);
static SetRuncode2_t RunCodesSet2 = (SetRuncode2_t)(Memory::ScanAddress(0x4B6700));

void Hooks::LaunchStudio()
{

    DWORD runCodes = 0x7288E0;
    RegRunCode((DWORD*)runCodes, "Studio", (int)StudioPrss, (int)StudioInitFn, (int)StudioDoneFn, (int)StudioPostInitFn, 0);

    Sleep(1000);
    RunCodesSet2((DWORD*)runCodes, "Studio");

}

bool Hooks::Setup()
{

    MH_STATUS status = MH_Initialize();
    if (status != MH_OK)
    {
        std::string sStatus = MH_StatusToString(status);
        printf("Minhook init failed! \nERR:%s\n", sStatus.c_str());
        return false;
    }

    if (g_settings.MultipleInstances)
    {
        //Multiple Instances
        if (MH_CreateHook(reinterpret_cast<void*>(hookCreateMainWindow), &detourCreateMainWindow, reinterpret_cast<void**>(&realCreateMainWindow)) != MH_OK)
        {
            return false;
        }
        if (MH_CreateHook(reinterpret_cast<void*>(hookMainCreateGameWindow), &detourMainCreateGameWindow, reinterpret_cast<void**>(&realMainCreateGameWindow)) != MH_OK)
        {
            return false;
        }

        if (MH_EnableHook(reinterpret_cast<void*>(hookCreateMainWindow)) != MH_OK)
        {
            return false;
        }
        if (MH_EnableHook(reinterpret_cast<void*>(hookMainCreateGameWindow)) != MH_OK)
        {
            return false;
        }
    }

    if (g_settings.windowMode)
    {
        //Create hooks

        //window mode
        if (MH_CreateHook(reinterpret_cast<void*>(hookVidToggleWindowedMode), &detourToggleWindowedMode, reinterpret_cast<void**>(&realVidToggleWindowedMode)) != MH_OK)
        {
            return 1;
        }

        if (MH_CreateHook(reinterpret_cast<void*>(hookVidSetMode), &detourVidSetMode, reinterpret_cast<void**>(&realVidSetMode)) != MH_OK)
        {
            return 1;
        }

        //Enable hooks
        if (MH_EnableHook(reinterpret_cast<void*>(hookVidToggleWindowedMode)) != MH_OK)
        {
            return 1;
        }

        if (MH_EnableHook(reinterpret_cast<void*>(hookVidSetMode)) != MH_OK)
        {
            return 1;
        }

        detourToggleWindowedMode();
    }

    if (g_settings.higherGPULimits)
    {
        if (MH_CreateHook(reinterpret_cast<void*>(hookInitBuckets), &detourInitBuckets, reinterpret_cast<void**>(&realInitBuckets)) != MH_OK)
        {
            return 1;
        }

        if (MH_CreateHook(reinterpret_cast<void*>(hookHeapInit), &detourHeapInit, reinterpret_cast<void**>(&realHeapInit)) != MH_OK)
        {
            return 1;
        }

        if (MH_EnableHook(reinterpret_cast<void*>(hookInitBuckets)) != MH_OK)
        {
            return 1;
        }

        if (MH_EnableHook(reinterpret_cast<void*>(hookHeapInit)) != MH_OK)
        {
            return 1;
        }

    }

    return true;
}