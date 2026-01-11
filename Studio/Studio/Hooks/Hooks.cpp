#include "Hooks.h"
#include "../GameFuncs/graphics/Vid.h"
#include "../GameFuncs/system/Log.h"

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

static bool __cdecl detourToggleWindowedMode()
{
    static U32* curMode = reinterpret_cast<U32*>(0x6BD59C);
    static U32* curDD = reinterpret_cast<U32*>(0x6BD5A0);
    U32 VIDMODEWINDOW = 254;

    if (*curMode != VIDMODEWINDOW)
    {
        //Log::Client::Write("ab");
        //Vid::InitDD(true);
        //Log::Client::Write("bb");

        //Attempting to RaiseException "FATAL" msg:SetMode; ddx->SetDisplayMode module:D:\Src\code\graphics\vid.cpp line:489

        //return Vid::SetMode(VIDMODEWINDOW, 800, 600, true);
    }

    return realVidToggleWindowedMode();
}

static void __fastcall detourInitBuckets(unsigned long count, unsigned long size, float ratio, int flush, unsigned long tcount, unsigned long tsize, float tratio)
{
    unsigned long newCount = 444 * 8;
    unsigned long newSize = 16000 * 8;
    unsigned long newtCount = 200 * 8;
    unsigned long newtSize = 16000 * 8;

    realInitBuckets(newCount, newSize, 0.95f, 1, newtCount, newtSize, 0.95f);
}

static void __fastcall detourHeapInit(unsigned long maxVtx, unsigned long maxIdx)
{
    //printf("maxVtx : %lu\tmaxIdx : %lu\n", maxVtx, maxIdx);
    maxVtx = 1450 * 8;
    maxIdx = 4350 * 8;

    realHeapInit(maxVtx, maxIdx);
}

static void __fastcall detourCoreGameInit()
{
    VarSys::CreateCmd("coregame.createobj", 0, 0);
    VarSys::CreateCmd("coregame.listobjs", 0, 0);
    VarSys::CreateCmd("coregame.listoffmap", 0, 0);
    VarSys::CreateCmd("coregame.listbyclassid", 0, 0);
    VarSys::CreateCmd("coregame.listclusters", 0, 0);
    VarSys::CreateCmd("coregame.listunits", 0, 0);
    VarSys::CreateCmd("coregame.removeobj", 0, 0);
    VarSys::CreateCmd("coregame.addobj", 0, 0);

    realCoreGameInit();
}

DWORD WINAPI Hooks::Setup(HINSTANCE hModule)
{
    MH_STATUS status = MH_Initialize();
    if (status != MH_OK)
    {
        std::string sStatus = MH_StatusToString(status);
        printf("Minhook init failed!\n");
        return 0;
    }

    if (MH_CreateHook(reinterpret_cast<void**>(hookCoreGameInit), &detourCoreGameInit, reinterpret_cast<void**>(&realCoreGameInit)) != MH_OK) return 1;
    if (MH_CreateHook(reinterpret_cast<void**>(hookInitBuckets), &detourInitBuckets, reinterpret_cast<void**>(&realInitBuckets)) != MH_OK) return 1;
    if (MH_CreateHook(reinterpret_cast<void**>(hookHeapInit), &detourHeapInit, reinterpret_cast<void**>(&realHeapInit)) != MH_OK) return 1;
    if (MH_CreateHook(reinterpret_cast<void**>(hookVidToggleWindowedMode), &detourToggleWindowedMode, reinterpret_cast<void**>(&realVidToggleWindowedMode)) != MH_OK) return 1;

    if (MH_EnableHook(reinterpret_cast<void**>(hookCoreGameInit)) != MH_OK) return 1;
    if (MH_EnableHook(reinterpret_cast<void**>(hookInitBuckets)) != MH_OK) return 1;
    if (MH_EnableHook(reinterpret_cast<void**>(hookHeapInit)) != MH_OK) return 1;
    if (MH_EnableHook(reinterpret_cast<void**>(hookVidToggleWindowedMode)) != MH_OK) return 1;

    /*
    if (MH_CreateHook(reinterpret_cast<void**>(hookVidRenderBegin), &detourVidRenderBegin, reinterpret_cast<void**>(&realVidRenderBegin)) != MH_OK) return 1;
    if (MH_CreateHook(reinterpret_cast<void**>(hookVidRenderEnd), &detourVidRenderEnd, reinterpret_cast<void**>(&realVidRenderEnd)) != MH_OK) return 1;
    */
}