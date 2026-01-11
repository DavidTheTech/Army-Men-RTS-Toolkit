#include "Vid.h"

typedef bool(__cdecl* Vid_ToggleWindowedMode_t)();
typedef bool(__fastcall* Vid_SetMode_t)(unsigned long mode, unsigned long width, unsigned long height, bool force);
typedef bool(__cdecl* Vid_InitSurfaces_t)();
typedef bool(__cdecl* Vid_SetCoopLevel_t)();
typedef void(__cdecl* Vid_OnActivate_t)();
typedef void(__cdecl* Vid_RestoreSurfaces_t)();
typedef void(__cdecl* Vid_Done_t)();
typedef bool(__cdecl* Vid_OnMove_t)();
typedef bool(__cdecl* Vid_OnSize_t)(UINT nType, signed long cx, signed long cy);
typedef bool(__cdecl* Vid_NextDriver_t)(signed long inc);
typedef unsigned long(__cdecl* Vid_FreeVidMem_t)(unsigned long report);
typedef DWORD(__cdecl* Vid_FlagsToBitDepth_t)(DWORD flags);
typedef bool(__cdecl* Vid_SetGamma_t)(signed long val);
typedef void(__cdecl* Vid_LogPerf_t)(const char* format, ...);
typedef void(__cdecl* Vid_ReportMode_t)(unsigned long report);
typedef unsigned long(__cdecl* Vid_Report_t)();

static Vid_ToggleWindowedMode_t ToggleWindowedMode_Fn = (Vid_ToggleWindowedMode_t)(Memory::ScanAddress(0x41DD40));
static Vid_SetMode_t SetMode_Fn = (Vid_SetMode_t)(Memory::ScanAddress(0x41DD50));
static Vid_InitSurfaces_t InitSurfaces_Fn = (Vid_InitSurfaces_t)(Memory::ScanAddress(0x41F4C0));
static Vid_SetCoopLevel_t SetCoopLevel_Fn = (Vid_SetCoopLevel_t)(Memory::ScanAddress(0x41F9F0));
static Vid_OnActivate_t OnActivate_Fn = (Vid_OnActivate_t)(Memory::ScanAddress(0x41FA90));
static Vid_RestoreSurfaces_t RestoreSurfaces_Fn = (Vid_RestoreSurfaces_t)(Memory::ScanAddress(0x41FB00));
static Vid_Done_t Done_Fn = (Vid_Done_t)(Memory::ScanAddress(0x41FE90));
static Vid_OnMove_t OnMove_Fn = (Vid_OnMove_t)(Memory::ScanAddress(0x4201E0));
static Vid_OnSize_t OnSize_Fn = (Vid_OnSize_t)(Memory::ScanAddress(0x420200));
static Vid_NextDriver_t NextDriver_Fn = (Vid_NextDriver_t)(Memory::ScanAddress(0x4202A0));
static Vid_FreeVidMem_t FreeVidMem_Fn = (Vid_FreeVidMem_t)(Memory::ScanAddress(0x4202E0));
static Vid_FlagsToBitDepth_t FlagsToBitDepth_Fn = (Vid_FlagsToBitDepth_t)(Memory::ScanAddress(0x420520));
static Vid_SetGamma_t SetGamma_Fn = (Vid_SetGamma_t)(Memory::ScanAddress(0x420640));
static Vid_LogPerf_t LogPerf_Fn = (Vid_LogPerf_t)(Memory::ScanAddress(0x4206E0));
static Vid_ReportMode_t ReportMode_Fn = (Vid_ReportMode_t)(Memory::ScanAddress(0x420730));
static Vid_Report_t Report_Fn = (Vid_Report_t)(Memory::ScanAddress(0x420B80));

bool Vid::ToggleWindowedMode()
{
    return ToggleWindowedMode_Fn();
}

bool Vid::SetMode(unsigned long mode, unsigned long width, unsigned long height, bool force)
{
    return SetMode_Fn(mode, width, height, force);
}

bool Vid::InitSurfaces()
{
    return InitSurfaces_Fn();
}

bool Vid::SetCoopLevel()
{
    return SetCoopLevel_Fn();
}

void Vid::OnActivate()
{
    OnActivate_Fn();
}

void Vid::RestoreSurfaces()
{
    RestoreSurfaces_Fn();
}

void Vid::Done()
{
    Done_Fn();
}

bool Vid::OnMove()
{
    return OnMove_Fn();
}

bool Vid::OnSize(UINT nType, signed long cx, signed long cy)
{
    return OnSize_Fn(nType, cx, cy);
}

bool Vid::NextDriver(signed long inc)
{
    return NextDriver_Fn(inc);
}

unsigned long Vid::FreeVidMem(unsigned long report)
{
    return FreeVidMem_Fn(report);
}

DWORD Vid::FlagsToBitDepth(DWORD flags)
{
    return FlagsToBitDepth_Fn(flags);
}

bool Vid::SetGamma(signed long val)
{
    return SetGamma_Fn(val);
}

void Vid::LogPerf(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    LogPerf_Fn(format, args);
    va_end(args);
}

void Vid::ReportMode(unsigned long report)
{
    ReportMode_Fn(report);
}

unsigned long Vid::Report()
{
    return Report_Fn();
}