#include <windows.h>
#include <stdio.h>
#include <shlwapi.h>
//#pragma comment(lib, "shlwapi.lib")

HINSTANCE mHinst = 0, mHinstDLL = 0;
UINT_PTR mProcs[250] = {0};

HINSTANCE g_hStudioDll = nullptr;

LPCSTR mImportNames[] = {"DbgHelpCreateUserDump", "DbgHelpCreateUserDumpW", "EnumDirTree", "EnumDirTreeW", "EnumerateLoadedModules", "EnumerateLoadedModules64", "EnumerateLoadedModulesEx", "EnumerateLoadedModulesExW", "EnumerateLoadedModulesW64", "ExtensionApiVersion", "FindDebugInfoFile", "FindDebugInfoFileEx", "FindDebugInfoFileExW", "FindExecutableImage", "FindExecutableImageEx", "FindExecutableImageExW", "FindFileInPath", "FindFileInSearchPath", "GetSymLoadError", "GetTimestampForLoadedLibrary", "ImageDirectoryEntryToData", "ImageDirectoryEntryToDataEx", "ImageNtHeader", "ImageRvaToSection", "ImageRvaToVa", "ImagehlpApiVersion", "ImagehlpApiVersionEx", "MakeSureDirectoryPathExists", "MapDebugInformation", "MiniDumpReadDumpStream", "MiniDumpWriteDump", "RangeMapAddPeImageSections", "RangeMapCreate", "RangeMapFree", "RangeMapRead", "RangeMapRemove", "RangeMapWrite", "RemoveInvalidModuleList", "ReportSymbolLoadSummary", "SearchTreeForFile", "SearchTreeForFileW", "SetCheckUserInterruptShared", "SetSymLoadError", "StackWalk", "StackWalk64", "StackWalkEx", "SymAddSourceStream", "SymAddSourceStreamA", "SymAddSourceStreamW", "SymAddSymbol", "SymAddSymbolW", "SymAddrIncludeInlineTrace", "SymCleanup", "SymCompareInlineTrace", "SymDeleteSymbol", "SymDeleteSymbolW", "SymEnumLines", "SymEnumLinesW", "SymEnumProcesses", "SymEnumSourceFileTokens", "SymEnumSourceFiles", "SymEnumSourceFilesW", "SymEnumSourceLines", "SymEnumSourceLinesW", "SymEnumSym", "SymEnumSymbols", "SymEnumSymbolsEx", "SymEnumSymbolsExW", "SymEnumSymbolsForAddr", "SymEnumSymbolsForAddrW", "SymEnumSymbolsW", "SymEnumTypes", "SymEnumTypesByName", "SymEnumTypesByNameW", "SymEnumTypesW", "SymEnumerateModules", "SymEnumerateModules64", "SymEnumerateModulesW64", "SymEnumerateSymbols", "SymEnumerateSymbols64", "SymEnumerateSymbolsW", "SymEnumerateSymbolsW64", "SymFindDebugInfoFile", "SymFindDebugInfoFileW", "SymFindExecutableImage", "SymFindExecutableImageW", "SymFindFileInPath", "SymFindFileInPathW", "SymFreeDiaString", "SymFromAddr", "SymFromAddrW", "SymFromIndex", "SymFromIndexW", "SymFromInlineContext", "SymFromInlineContextW", "SymFromName", "SymFromNameW", "SymFromToken", "SymFromTokenW", "SymFunctionTableAccess", "SymFunctionTableAccess64", "SymFunctionTableAccess64AccessRoutines", "SymGetDiaSession", "SymGetFileLineOffsets64", "SymGetHomeDirectory", "SymGetHomeDirectoryW", "SymGetLineFromAddr", "SymGetLineFromAddr64", "SymGetLineFromAddrW64", "SymGetLineFromInlineContext", "SymGetLineFromInlineContextW", "SymGetLineFromName", "SymGetLineFromName64", "SymGetLineFromNameW64", "SymGetLineNext", "SymGetLineNext64", "SymGetLineNextW64", "SymGetLinePrev", "SymGetLinePrev64", "SymGetLinePrevW64", "SymGetModuleBase", "SymGetModuleBase64", "SymGetModuleInfo", "SymGetModuleInfo64", "SymGetModuleInfoW", "SymGetModuleInfoW64", "SymGetOmapBlockBase", "SymGetOmaps", "SymGetOptions", "SymGetScope", "SymGetScopeW", "SymGetSearchPath", "SymGetSearchPathW", "SymGetSourceFile", "SymGetSourceFileFromToken", "SymGetSourceFileFromTokenW", "SymGetSourceFileToken", "SymGetSourceFileTokenW", "SymGetSourceFileW", "SymGetSourceVarFromToken", "SymGetSourceVarFromTokenW", "SymGetSymFromAddr", "SymGetSymFromAddr64", "SymGetSymFromName", "SymGetSymFromName64", "SymGetSymNext", "SymGetSymNext64", "SymGetSymPrev", "SymGetSymPrev64", "SymGetSymbolFile", "SymGetSymbolFileW", "SymGetTypeFromName", "SymGetTypeFromNameW", "SymGetTypeInfo", "SymGetTypeInfoEx", "SymGetUnwindInfo", "SymInitialize", "SymInitializeW", "SymLoadModule", "SymLoadModule64", "SymLoadModuleEx", "SymLoadModuleExW", "SymMatchFileName", "SymMatchFileNameW", "SymMatchString", "SymMatchStringA", "SymMatchStringW", "SymNext", "SymNextW", "SymPrev", "SymPrevW", "SymQueryInlineTrace", "SymRefreshModuleList", "SymRegisterCallback", "SymRegisterCallback64", "SymRegisterCallbackW64", "SymRegisterFunctionEntryCallback", "SymRegisterFunctionEntryCallback64", "SymSearch", "SymSearchW", "SymSetContext", "SymSetHomeDirectory", "SymSetHomeDirectoryW", "SymSetOptions", "SymSetParentWindow", "SymSetScopeFromAddr", "SymSetScopeFromIndex", "SymSetScopeFromInlineContext", "SymSetSearchPath", "SymSetSearchPathW", "SymSrvDeltaName", "SymSrvDeltaNameW", "SymSrvGetFileIndexInfo", "SymSrvGetFileIndexInfoW", "SymSrvGetFileIndexString", "SymSrvGetFileIndexStringW", "SymSrvGetFileIndexes", "SymSrvGetFileIndexesW", "SymSrvGetSupplement", "SymSrvGetSupplementW", "SymSrvIsStore", "SymSrvIsStoreW", "SymSrvStoreFile", "SymSrvStoreFileW", "SymSrvStoreSupplement", "SymSrvStoreSupplementW", "SymUnDName", "SymUnDName64", "SymUnloadModule", "SymUnloadModule64", "UnDecorateSymbolName", "UnDecorateSymbolNameW", "UnmapDebugInformation", "WinDbgExtensionDllInit", "_EFN_DumpImage", "block", "chksym", "dbghelp", "dh", "fptr", "homedir", "inlinedbg", "itoldyouso", "lmi", "lminfo", "omap", "optdbgdump", "optdbgdumpaddr", "srcfiles", "stack_force_ebp", "stackdbg", "sym", "symsrv", "vc7fpo", (LPCSTR)1001, (LPCSTR)1002, (LPCSTR)1003, (LPCSTR)1004, (LPCSTR)1005, (LPCSTR)1006, (LPCSTR)1007, (LPCSTR)1008, (LPCSTR)1009, (LPCSTR)1010, (LPCSTR)1011, (LPCSTR)1012, (LPCSTR)1013, (LPCSTR)1014, (LPCSTR)1015, (LPCSTR)1016};

int loadStudio()
{
	char exePath[MAX_PATH] = { 0 };
	if (GetModuleFileNameA(nullptr, exePath, MAX_PATH) == 0)
	{
		return FALSE;
	}

	PathRemoveFileSpecA(exePath);
	char dllPath[MAX_PATH];
	int len = snprintf(dllPath, sizeof(dllPath), "%s\\studio.dll", exePath);
	if (len < 0 || len >= (int)sizeof(dllPath))
	{
		return FALSE;
	}

	g_hStudioDll = LoadLibraryA(dllPath);
	if (!g_hStudioDll)
	{
		DWORD err = GetLastError();
		char msg[512] = {};

		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), msg, sizeof(msg), nullptr);

		char out[1024] = {};
		sprintf_s(out, sizeof(out), "LoadLibrary failed\n\nError code: %lu\nMessage: %s", err, msg);

		MessageBoxA(nullptr, out, "DLL Load Error", MB_ICONERROR);

		return FALSE;
	}
	return TRUE;
}

BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved )
{
	char Filename[MAX_PATH] = {};

	mHinst = hinstDLL;
	if ( fdwReason == DLL_PROCESS_ATTACH )
	{
		if (MessageBoxA(nullptr, "Would you like to load Studio.dll?", "Studio Launcher", MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			loadStudio();
		}
		
		if (GetModuleFileNameA(hinstDLL, Filename, MAX_PATH) != 0)
		{
			PathRemoveFileSpecA(Filename);
			strcat_s(Filename, sizeof(Filename), "\\dbghelp_r.dll");

			mHinstDLL = LoadLibraryA(Filename);
		}

		if (!mHinstDLL)
		{
			DWORD err = GetLastError();
			char msg[512] = {};

			FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), msg, sizeof(msg), nullptr);

			char out[1024] = {};
			sprintf_s(out, sizeof(out), "LoadLibrary failed\n\nError code: %lu\nMessage: %s", err, msg);

			MessageBoxA(nullptr, out, "DLL Load Error", MB_ICONERROR);

			MessageBoxA(0, "oopsie i crashed :3", "Meow", 0);
			return (FALSE);
		}
		for (int i = 0; i < 250; i++)
		{
			mProcs[i] = (UINT_PTR)GetProcAddress(mHinstDLL, mImportNames[i]);
		}
	}
	else if ( fdwReason == DLL_PROCESS_DETACH )
	{
		FreeLibrary( mHinstDLL );
	}
	return ( TRUE );
}

extern "C" __declspec(naked) void __stdcall DbgHelpCreateUserDump_wrapper(){__asm{jmp mProcs[0*4]}}
extern "C" __declspec(naked) void __stdcall DbgHelpCreateUserDumpW_wrapper(){__asm{jmp mProcs[1*4]}}
extern "C" __declspec(naked) void __stdcall EnumDirTree_wrapper(){__asm{jmp mProcs[2*4]}}
extern "C" __declspec(naked) void __stdcall EnumDirTreeW_wrapper(){__asm{jmp mProcs[3*4]}}
extern "C" __declspec(naked) void __stdcall EnumerateLoadedModules_wrapper(){__asm{jmp mProcs[4*4]}}
extern "C" __declspec(naked) void __stdcall EnumerateLoadedModules64_wrapper(){__asm{jmp mProcs[5*4]}}
extern "C" __declspec(naked) void __stdcall EnumerateLoadedModulesEx_wrapper(){__asm{jmp mProcs[6*4]}}
extern "C" __declspec(naked) void __stdcall EnumerateLoadedModulesExW_wrapper(){__asm{jmp mProcs[7*4]}}
extern "C" __declspec(naked) void __stdcall EnumerateLoadedModulesW64_wrapper(){__asm{jmp mProcs[8*4]}}
extern "C" __declspec(naked) void __stdcall ExtensionApiVersion_wrapper(){__asm{jmp mProcs[9*4]}}
extern "C" __declspec(naked) void __stdcall FindDebugInfoFile_wrapper(){__asm{jmp mProcs[10*4]}}
extern "C" __declspec(naked) void __stdcall FindDebugInfoFileEx_wrapper(){__asm{jmp mProcs[11*4]}}
extern "C" __declspec(naked) void __stdcall FindDebugInfoFileExW_wrapper(){__asm{jmp mProcs[12*4]}}
extern "C" __declspec(naked) void __stdcall FindExecutableImage_wrapper(){__asm{jmp mProcs[13*4]}}
extern "C" __declspec(naked) void __stdcall FindExecutableImageEx_wrapper(){__asm{jmp mProcs[14*4]}}
extern "C" __declspec(naked) void __stdcall FindExecutableImageExW_wrapper(){__asm{jmp mProcs[15*4]}}
extern "C" __declspec(naked) void __stdcall FindFileInPath_wrapper(){__asm{jmp mProcs[16*4]}}
extern "C" __declspec(naked) void __stdcall FindFileInSearchPath_wrapper(){__asm{jmp mProcs[17*4]}}
extern "C" __declspec(naked) void __stdcall GetSymLoadError_wrapper(){__asm{jmp mProcs[18*4]}}
extern "C" __declspec(naked) void __stdcall GetTimestampForLoadedLibrary_wrapper(){__asm{jmp mProcs[19*4]}}
extern "C" __declspec(naked) void __stdcall ImageDirectoryEntryToData_wrapper(){__asm{jmp mProcs[20*4]}}
extern "C" __declspec(naked) void __stdcall ImageDirectoryEntryToDataEx_wrapper(){__asm{jmp mProcs[21*4]}}
extern "C" __declspec(naked) void __stdcall ImageNtHeader_wrapper(){__asm{jmp mProcs[22*4]}}
extern "C" __declspec(naked) void __stdcall ImageRvaToSection_wrapper(){__asm{jmp mProcs[23*4]}}
extern "C" __declspec(naked) void __stdcall ImageRvaToVa_wrapper(){__asm{jmp mProcs[24*4]}}
extern "C" __declspec(naked) void __stdcall ImagehlpApiVersion_wrapper(){__asm{jmp mProcs[25*4]}}
extern "C" __declspec(naked) void __stdcall ImagehlpApiVersionEx_wrapper(){__asm{jmp mProcs[26*4]}}
extern "C" __declspec(naked) void __stdcall MakeSureDirectoryPathExists_wrapper(){__asm{jmp mProcs[27*4]}}
extern "C" __declspec(naked) void __stdcall MapDebugInformation_wrapper(){__asm{jmp mProcs[28*4]}}
extern "C" __declspec(naked) void __stdcall MiniDumpReadDumpStream_wrapper(){__asm{jmp mProcs[29*4]}}
extern "C" __declspec(naked) void __stdcall MiniDumpWriteDump_wrapper(){__asm{jmp mProcs[30*4]}}
extern "C" __declspec(naked) void __stdcall RangeMapAddPeImageSections_wrapper(){__asm{jmp mProcs[31*4]}}
extern "C" __declspec(naked) void __stdcall RangeMapCreate_wrapper(){__asm{jmp mProcs[32*4]}}
extern "C" __declspec(naked) void __stdcall RangeMapFree_wrapper(){__asm{jmp mProcs[33*4]}}
extern "C" __declspec(naked) void __stdcall RangeMapRead_wrapper(){__asm{jmp mProcs[34*4]}}
extern "C" __declspec(naked) void __stdcall RangeMapRemove_wrapper(){__asm{jmp mProcs[35*4]}}
extern "C" __declspec(naked) void __stdcall RangeMapWrite_wrapper(){__asm{jmp mProcs[36*4]}}
extern "C" __declspec(naked) void __stdcall RemoveInvalidModuleList_wrapper(){__asm{jmp mProcs[37*4]}}
extern "C" __declspec(naked) void __stdcall ReportSymbolLoadSummary_wrapper(){__asm{jmp mProcs[38*4]}}
extern "C" __declspec(naked) void __stdcall SearchTreeForFile_wrapper(){__asm{jmp mProcs[39*4]}}
extern "C" __declspec(naked) void __stdcall SearchTreeForFileW_wrapper(){__asm{jmp mProcs[40*4]}}
extern "C" __declspec(naked) void __stdcall SetCheckUserInterruptShared_wrapper(){__asm{jmp mProcs[41*4]}}
extern "C" __declspec(naked) void __stdcall SetSymLoadError_wrapper(){__asm{jmp mProcs[42*4]}}
extern "C" __declspec(naked) void __stdcall StackWalk_wrapper(){__asm{jmp mProcs[43*4]}}
extern "C" __declspec(naked) void __stdcall StackWalk64_wrapper(){__asm{jmp mProcs[44*4]}}
extern "C" __declspec(naked) void __stdcall StackWalkEx_wrapper(){__asm{jmp mProcs[45*4]}}
extern "C" __declspec(naked) void __stdcall SymAddSourceStream_wrapper(){__asm{jmp mProcs[46*4]}}
extern "C" __declspec(naked) void __stdcall SymAddSourceStreamA_wrapper(){__asm{jmp mProcs[47*4]}}
extern "C" __declspec(naked) void __stdcall SymAddSourceStreamW_wrapper(){__asm{jmp mProcs[48*4]}}
extern "C" __declspec(naked) void __stdcall SymAddSymbol_wrapper(){__asm{jmp mProcs[49*4]}}
extern "C" __declspec(naked) void __stdcall SymAddSymbolW_wrapper(){__asm{jmp mProcs[50*4]}}
extern "C" __declspec(naked) void __stdcall SymAddrIncludeInlineTrace_wrapper(){__asm{jmp mProcs[51*4]}}
extern "C" __declspec(naked) void __stdcall SymCleanup_wrapper(){__asm{jmp mProcs[52*4]}}
extern "C" __declspec(naked) void __stdcall SymCompareInlineTrace_wrapper(){__asm{jmp mProcs[53*4]}}
extern "C" __declspec(naked) void __stdcall SymDeleteSymbol_wrapper(){__asm{jmp mProcs[54*4]}}
extern "C" __declspec(naked) void __stdcall SymDeleteSymbolW_wrapper(){__asm{jmp mProcs[55*4]}}
extern "C" __declspec(naked) void __stdcall SymEnumLines_wrapper(){__asm{jmp mProcs[56*4]}}
extern "C" __declspec(naked) void __stdcall SymEnumLinesW_wrapper(){__asm{jmp mProcs[57*4]}}
extern "C" __declspec(naked) void __stdcall SymEnumProcesses_wrapper(){__asm{jmp mProcs[58*4]}}
extern "C" __declspec(naked) void __stdcall SymEnumSourceFileTokens_wrapper(){__asm{jmp mProcs[59*4]}}
extern "C" __declspec(naked) void __stdcall SymEnumSourceFiles_wrapper(){__asm{jmp mProcs[60*4]}}
extern "C" __declspec(naked) void __stdcall SymEnumSourceFilesW_wrapper(){__asm{jmp mProcs[61*4]}}
extern "C" __declspec(naked) void __stdcall SymEnumSourceLines_wrapper(){__asm{jmp mProcs[62*4]}}
extern "C" __declspec(naked) void __stdcall SymEnumSourceLinesW_wrapper(){__asm{jmp mProcs[63*4]}}
extern "C" __declspec(naked) void __stdcall SymEnumSym_wrapper(){__asm{jmp mProcs[64*4]}}
extern "C" __declspec(naked) void __stdcall SymEnumSymbols_wrapper(){__asm{jmp mProcs[65*4]}}
extern "C" __declspec(naked) void __stdcall SymEnumSymbolsEx_wrapper(){__asm{jmp mProcs[66*4]}}
extern "C" __declspec(naked) void __stdcall SymEnumSymbolsExW_wrapper(){__asm{jmp mProcs[67*4]}}
extern "C" __declspec(naked) void __stdcall SymEnumSymbolsForAddr_wrapper(){__asm{jmp mProcs[68*4]}}
extern "C" __declspec(naked) void __stdcall SymEnumSymbolsForAddrW_wrapper(){__asm{jmp mProcs[69*4]}}
extern "C" __declspec(naked) void __stdcall SymEnumSymbolsW_wrapper(){__asm{jmp mProcs[70*4]}}
extern "C" __declspec(naked) void __stdcall SymEnumTypes_wrapper(){__asm{jmp mProcs[71*4]}}
extern "C" __declspec(naked) void __stdcall SymEnumTypesByName_wrapper(){__asm{jmp mProcs[72*4]}}
extern "C" __declspec(naked) void __stdcall SymEnumTypesByNameW_wrapper(){__asm{jmp mProcs[73*4]}}
extern "C" __declspec(naked) void __stdcall SymEnumTypesW_wrapper(){__asm{jmp mProcs[74*4]}}
extern "C" __declspec(naked) void __stdcall SymEnumerateModules_wrapper(){__asm{jmp mProcs[75*4]}}
extern "C" __declspec(naked) void __stdcall SymEnumerateModules64_wrapper(){__asm{jmp mProcs[76*4]}}
extern "C" __declspec(naked) void __stdcall SymEnumerateModulesW64_wrapper(){__asm{jmp mProcs[77*4]}}
extern "C" __declspec(naked) void __stdcall SymEnumerateSymbols_wrapper(){__asm{jmp mProcs[78*4]}}
extern "C" __declspec(naked) void __stdcall SymEnumerateSymbols64_wrapper(){__asm{jmp mProcs[79*4]}}
extern "C" __declspec(naked) void __stdcall SymEnumerateSymbolsW_wrapper(){__asm{jmp mProcs[80*4]}}
extern "C" __declspec(naked) void __stdcall SymEnumerateSymbolsW64_wrapper(){__asm{jmp mProcs[81*4]}}
extern "C" __declspec(naked) void __stdcall SymFindDebugInfoFile_wrapper(){__asm{jmp mProcs[82*4]}}
extern "C" __declspec(naked) void __stdcall SymFindDebugInfoFileW_wrapper(){__asm{jmp mProcs[83*4]}}
extern "C" __declspec(naked) void __stdcall SymFindExecutableImage_wrapper(){__asm{jmp mProcs[84*4]}}
extern "C" __declspec(naked) void __stdcall SymFindExecutableImageW_wrapper(){__asm{jmp mProcs[85*4]}}
extern "C" __declspec(naked) void __stdcall SymFindFileInPath_wrapper(){__asm{jmp mProcs[86*4]}}
extern "C" __declspec(naked) void __stdcall SymFindFileInPathW_wrapper(){__asm{jmp mProcs[87*4]}}
extern "C" __declspec(naked) void __stdcall SymFreeDiaString_wrapper(){__asm{jmp mProcs[88*4]}}
extern "C" __declspec(naked) void __stdcall SymFromAddr_wrapper(){__asm{jmp mProcs[89*4]}}
extern "C" __declspec(naked) void __stdcall SymFromAddrW_wrapper(){__asm{jmp mProcs[90*4]}}
extern "C" __declspec(naked) void __stdcall SymFromIndex_wrapper(){__asm{jmp mProcs[91*4]}}
extern "C" __declspec(naked) void __stdcall SymFromIndexW_wrapper(){__asm{jmp mProcs[92*4]}}
extern "C" __declspec(naked) void __stdcall SymFromInlineContext_wrapper(){__asm{jmp mProcs[93*4]}}
extern "C" __declspec(naked) void __stdcall SymFromInlineContextW_wrapper(){__asm{jmp mProcs[94*4]}}
extern "C" __declspec(naked) void __stdcall SymFromName_wrapper(){__asm{jmp mProcs[95*4]}}
extern "C" __declspec(naked) void __stdcall SymFromNameW_wrapper(){__asm{jmp mProcs[96*4]}}
extern "C" __declspec(naked) void __stdcall SymFromToken_wrapper(){__asm{jmp mProcs[97*4]}}
extern "C" __declspec(naked) void __stdcall SymFromTokenW_wrapper(){__asm{jmp mProcs[98*4]}}
extern "C" __declspec(naked) void __stdcall SymFunctionTableAccess_wrapper(){__asm{jmp mProcs[99*4]}}
extern "C" __declspec(naked) void __stdcall SymFunctionTableAccess64_wrapper(){__asm{jmp mProcs[100*4]}}
extern "C" __declspec(naked) void __stdcall SymFunctionTableAccess64AccessRoutines_wrapper(){__asm{jmp mProcs[101*4]}}
extern "C" __declspec(naked) void __stdcall SymGetDiaSession_wrapper(){__asm{jmp mProcs[102*4]}}
extern "C" __declspec(naked) void __stdcall SymGetFileLineOffsets64_wrapper(){__asm{jmp mProcs[103*4]}}
extern "C" __declspec(naked) void __stdcall SymGetHomeDirectory_wrapper(){__asm{jmp mProcs[104*4]}}
extern "C" __declspec(naked) void __stdcall SymGetHomeDirectoryW_wrapper(){__asm{jmp mProcs[105*4]}}
extern "C" __declspec(naked) void __stdcall SymGetLineFromAddr_wrapper(){__asm{jmp mProcs[106*4]}}
extern "C" __declspec(naked) void __stdcall SymGetLineFromAddr64_wrapper(){__asm{jmp mProcs[107*4]}}
extern "C" __declspec(naked) void __stdcall SymGetLineFromAddrW64_wrapper(){__asm{jmp mProcs[108*4]}}
extern "C" __declspec(naked) void __stdcall SymGetLineFromInlineContext_wrapper(){__asm{jmp mProcs[109*4]}}
extern "C" __declspec(naked) void __stdcall SymGetLineFromInlineContextW_wrapper(){__asm{jmp mProcs[110*4]}}
extern "C" __declspec(naked) void __stdcall SymGetLineFromName_wrapper(){__asm{jmp mProcs[111*4]}}
extern "C" __declspec(naked) void __stdcall SymGetLineFromName64_wrapper(){__asm{jmp mProcs[112*4]}}
extern "C" __declspec(naked) void __stdcall SymGetLineFromNameW64_wrapper(){__asm{jmp mProcs[113*4]}}
extern "C" __declspec(naked) void __stdcall SymGetLineNext_wrapper(){__asm{jmp mProcs[114*4]}}
extern "C" __declspec(naked) void __stdcall SymGetLineNext64_wrapper(){__asm{jmp mProcs[115*4]}}
extern "C" __declspec(naked) void __stdcall SymGetLineNextW64_wrapper(){__asm{jmp mProcs[116*4]}}
extern "C" __declspec(naked) void __stdcall SymGetLinePrev_wrapper(){__asm{jmp mProcs[117*4]}}
extern "C" __declspec(naked) void __stdcall SymGetLinePrev64_wrapper(){__asm{jmp mProcs[118*4]}}
extern "C" __declspec(naked) void __stdcall SymGetLinePrevW64_wrapper(){__asm{jmp mProcs[119*4]}}
extern "C" __declspec(naked) void __stdcall SymGetModuleBase_wrapper(){__asm{jmp mProcs[120*4]}}
extern "C" __declspec(naked) void __stdcall SymGetModuleBase64_wrapper(){__asm{jmp mProcs[121*4]}}
extern "C" __declspec(naked) void __stdcall SymGetModuleInfo_wrapper(){__asm{jmp mProcs[122*4]}}
extern "C" __declspec(naked) void __stdcall SymGetModuleInfo64_wrapper(){__asm{jmp mProcs[123*4]}}
extern "C" __declspec(naked) void __stdcall SymGetModuleInfoW_wrapper(){__asm{jmp mProcs[124*4]}}
extern "C" __declspec(naked) void __stdcall SymGetModuleInfoW64_wrapper(){__asm{jmp mProcs[125*4]}}
extern "C" __declspec(naked) void __stdcall SymGetOmapBlockBase_wrapper(){__asm{jmp mProcs[126*4]}}
extern "C" __declspec(naked) void __stdcall SymGetOmaps_wrapper(){__asm{jmp mProcs[127*4]}}
extern "C" __declspec(naked) void __stdcall SymGetOptions_wrapper(){__asm{jmp mProcs[128*4]}}
extern "C" __declspec(naked) void __stdcall SymGetScope_wrapper(){__asm{jmp mProcs[129*4]}}
extern "C" __declspec(naked) void __stdcall SymGetScopeW_wrapper(){__asm{jmp mProcs[130*4]}}
extern "C" __declspec(naked) void __stdcall SymGetSearchPath_wrapper(){__asm{jmp mProcs[131*4]}}
extern "C" __declspec(naked) void __stdcall SymGetSearchPathW_wrapper(){__asm{jmp mProcs[132*4]}}
extern "C" __declspec(naked) void __stdcall SymGetSourceFile_wrapper(){__asm{jmp mProcs[133*4]}}
extern "C" __declspec(naked) void __stdcall SymGetSourceFileFromToken_wrapper(){__asm{jmp mProcs[134*4]}}
extern "C" __declspec(naked) void __stdcall SymGetSourceFileFromTokenW_wrapper(){__asm{jmp mProcs[135*4]}}
extern "C" __declspec(naked) void __stdcall SymGetSourceFileToken_wrapper(){__asm{jmp mProcs[136*4]}}
extern "C" __declspec(naked) void __stdcall SymGetSourceFileTokenW_wrapper(){__asm{jmp mProcs[137*4]}}
extern "C" __declspec(naked) void __stdcall SymGetSourceFileW_wrapper(){__asm{jmp mProcs[138*4]}}
extern "C" __declspec(naked) void __stdcall SymGetSourceVarFromToken_wrapper(){__asm{jmp mProcs[139*4]}}
extern "C" __declspec(naked) void __stdcall SymGetSourceVarFromTokenW_wrapper(){__asm{jmp mProcs[140*4]}}
extern "C" __declspec(naked) void __stdcall SymGetSymFromAddr_wrapper(){__asm{jmp mProcs[141*4]}}
extern "C" __declspec(naked) void __stdcall SymGetSymFromAddr64_wrapper(){__asm{jmp mProcs[142*4]}}
extern "C" __declspec(naked) void __stdcall SymGetSymFromName_wrapper(){__asm{jmp mProcs[143*4]}}
extern "C" __declspec(naked) void __stdcall SymGetSymFromName64_wrapper(){__asm{jmp mProcs[144*4]}}
extern "C" __declspec(naked) void __stdcall SymGetSymNext_wrapper(){__asm{jmp mProcs[145*4]}}
extern "C" __declspec(naked) void __stdcall SymGetSymNext64_wrapper(){__asm{jmp mProcs[146*4]}}
extern "C" __declspec(naked) void __stdcall SymGetSymPrev_wrapper(){__asm{jmp mProcs[147*4]}}
extern "C" __declspec(naked) void __stdcall SymGetSymPrev64_wrapper(){__asm{jmp mProcs[148*4]}}
extern "C" __declspec(naked) void __stdcall SymGetSymbolFile_wrapper(){__asm{jmp mProcs[149*4]}}
extern "C" __declspec(naked) void __stdcall SymGetSymbolFileW_wrapper(){__asm{jmp mProcs[150*4]}}
extern "C" __declspec(naked) void __stdcall SymGetTypeFromName_wrapper(){__asm{jmp mProcs[151*4]}}
extern "C" __declspec(naked) void __stdcall SymGetTypeFromNameW_wrapper(){__asm{jmp mProcs[152*4]}}
extern "C" __declspec(naked) void __stdcall SymGetTypeInfo_wrapper(){__asm{jmp mProcs[153*4]}}
extern "C" __declspec(naked) void __stdcall SymGetTypeInfoEx_wrapper(){__asm{jmp mProcs[154*4]}}
extern "C" __declspec(naked) void __stdcall SymGetUnwindInfo_wrapper(){__asm{jmp mProcs[155*4]}}
extern "C" __declspec(naked) void __stdcall SymInitialize_wrapper(){__asm{jmp mProcs[156*4]}}
extern "C" __declspec(naked) void __stdcall SymInitializeW_wrapper(){__asm{jmp mProcs[157*4]}}
extern "C" __declspec(naked) void __stdcall SymLoadModule_wrapper(){__asm{jmp mProcs[158*4]}}
extern "C" __declspec(naked) void __stdcall SymLoadModule64_wrapper(){__asm{jmp mProcs[159*4]}}
extern "C" __declspec(naked) void __stdcall SymLoadModuleEx_wrapper(){__asm{jmp mProcs[160*4]}}
extern "C" __declspec(naked) void __stdcall SymLoadModuleExW_wrapper(){__asm{jmp mProcs[161*4]}}
extern "C" __declspec(naked) void __stdcall SymMatchFileName_wrapper(){__asm{jmp mProcs[162*4]}}
extern "C" __declspec(naked) void __stdcall SymMatchFileNameW_wrapper(){__asm{jmp mProcs[163*4]}}
extern "C" __declspec(naked) void __stdcall SymMatchString_wrapper(){__asm{jmp mProcs[164*4]}}
extern "C" __declspec(naked) void __stdcall SymMatchStringA_wrapper(){__asm{jmp mProcs[165*4]}}
extern "C" __declspec(naked) void __stdcall SymMatchStringW_wrapper(){__asm{jmp mProcs[166*4]}}
extern "C" __declspec(naked) void __stdcall SymNext_wrapper(){__asm{jmp mProcs[167*4]}}
extern "C" __declspec(naked) void __stdcall SymNextW_wrapper(){__asm{jmp mProcs[168*4]}}
extern "C" __declspec(naked) void __stdcall SymPrev_wrapper(){__asm{jmp mProcs[169*4]}}
extern "C" __declspec(naked) void __stdcall SymPrevW_wrapper(){__asm{jmp mProcs[170*4]}}
extern "C" __declspec(naked) void __stdcall SymQueryInlineTrace_wrapper(){__asm{jmp mProcs[171*4]}}
extern "C" __declspec(naked) void __stdcall SymRefreshModuleList_wrapper(){__asm{jmp mProcs[172*4]}}
extern "C" __declspec(naked) void __stdcall SymRegisterCallback_wrapper(){__asm{jmp mProcs[173*4]}}
extern "C" __declspec(naked) void __stdcall SymRegisterCallback64_wrapper(){__asm{jmp mProcs[174*4]}}
extern "C" __declspec(naked) void __stdcall SymRegisterCallbackW64_wrapper(){__asm{jmp mProcs[175*4]}}
extern "C" __declspec(naked) void __stdcall SymRegisterFunctionEntryCallback_wrapper(){__asm{jmp mProcs[176*4]}}
extern "C" __declspec(naked) void __stdcall SymRegisterFunctionEntryCallback64_wrapper(){__asm{jmp mProcs[177*4]}}
extern "C" __declspec(naked) void __stdcall SymSearch_wrapper(){__asm{jmp mProcs[178*4]}}
extern "C" __declspec(naked) void __stdcall SymSearchW_wrapper(){__asm{jmp mProcs[179*4]}}
extern "C" __declspec(naked) void __stdcall SymSetContext_wrapper(){__asm{jmp mProcs[180*4]}}
extern "C" __declspec(naked) void __stdcall SymSetHomeDirectory_wrapper(){__asm{jmp mProcs[181*4]}}
extern "C" __declspec(naked) void __stdcall SymSetHomeDirectoryW_wrapper(){__asm{jmp mProcs[182*4]}}
extern "C" __declspec(naked) void __stdcall SymSetOptions_wrapper(){__asm{jmp mProcs[183*4]}}
extern "C" __declspec(naked) void __stdcall SymSetParentWindow_wrapper(){__asm{jmp mProcs[184*4]}}
extern "C" __declspec(naked) void __stdcall SymSetScopeFromAddr_wrapper(){__asm{jmp mProcs[185*4]}}
extern "C" __declspec(naked) void __stdcall SymSetScopeFromIndex_wrapper(){__asm{jmp mProcs[186*4]}}
extern "C" __declspec(naked) void __stdcall SymSetScopeFromInlineContext_wrapper(){__asm{jmp mProcs[187*4]}}
extern "C" __declspec(naked) void __stdcall SymSetSearchPath_wrapper(){__asm{jmp mProcs[188*4]}}
extern "C" __declspec(naked) void __stdcall SymSetSearchPathW_wrapper(){__asm{jmp mProcs[189*4]}}
extern "C" __declspec(naked) void __stdcall SymSrvDeltaName_wrapper(){__asm{jmp mProcs[190*4]}}
extern "C" __declspec(naked) void __stdcall SymSrvDeltaNameW_wrapper(){__asm{jmp mProcs[191*4]}}
extern "C" __declspec(naked) void __stdcall SymSrvGetFileIndexInfo_wrapper(){__asm{jmp mProcs[192*4]}}
extern "C" __declspec(naked) void __stdcall SymSrvGetFileIndexInfoW_wrapper(){__asm{jmp mProcs[193*4]}}
extern "C" __declspec(naked) void __stdcall SymSrvGetFileIndexString_wrapper(){__asm{jmp mProcs[194*4]}}
extern "C" __declspec(naked) void __stdcall SymSrvGetFileIndexStringW_wrapper(){__asm{jmp mProcs[195*4]}}
extern "C" __declspec(naked) void __stdcall SymSrvGetFileIndexes_wrapper(){__asm{jmp mProcs[196*4]}}
extern "C" __declspec(naked) void __stdcall SymSrvGetFileIndexesW_wrapper(){__asm{jmp mProcs[197*4]}}
extern "C" __declspec(naked) void __stdcall SymSrvGetSupplement_wrapper(){__asm{jmp mProcs[198*4]}}
extern "C" __declspec(naked) void __stdcall SymSrvGetSupplementW_wrapper(){__asm{jmp mProcs[199*4]}}
extern "C" __declspec(naked) void __stdcall SymSrvIsStore_wrapper(){__asm{jmp mProcs[200*4]}}
extern "C" __declspec(naked) void __stdcall SymSrvIsStoreW_wrapper(){__asm{jmp mProcs[201*4]}}
extern "C" __declspec(naked) void __stdcall SymSrvStoreFile_wrapper(){__asm{jmp mProcs[202*4]}}
extern "C" __declspec(naked) void __stdcall SymSrvStoreFileW_wrapper(){__asm{jmp mProcs[203*4]}}
extern "C" __declspec(naked) void __stdcall SymSrvStoreSupplement_wrapper(){__asm{jmp mProcs[204*4]}}
extern "C" __declspec(naked) void __stdcall SymSrvStoreSupplementW_wrapper(){__asm{jmp mProcs[205*4]}}
extern "C" __declspec(naked) void __stdcall SymUnDName_wrapper(){__asm{jmp mProcs[206*4]}}
extern "C" __declspec(naked) void __stdcall SymUnDName64_wrapper(){__asm{jmp mProcs[207*4]}}
extern "C" __declspec(naked) void __stdcall SymUnloadModule_wrapper(){__asm{jmp mProcs[208*4]}}
extern "C" __declspec(naked) void __stdcall SymUnloadModule64_wrapper(){__asm{jmp mProcs[209*4]}}
extern "C" __declspec(naked) void __stdcall UnDecorateSymbolName_wrapper(){__asm{jmp mProcs[210*4]}}
extern "C" __declspec(naked) void __stdcall UnDecorateSymbolNameW_wrapper(){__asm{jmp mProcs[211*4]}}
extern "C" __declspec(naked) void __stdcall UnmapDebugInformation_wrapper(){__asm{jmp mProcs[212*4]}}
extern "C" __declspec(naked) void __stdcall WinDbgExtensionDllInit_wrapper(){__asm{jmp mProcs[213*4]}}
extern "C" __declspec(naked) void __stdcall _EFN_DumpImage_wrapper(){__asm{jmp mProcs[214*4]}}
extern "C" __declspec(naked) void __stdcall block_wrapper(){__asm{jmp mProcs[215*4]}}
extern "C" __declspec(naked) void __stdcall chksym_wrapper(){__asm{jmp mProcs[216*4]}}
extern "C" __declspec(naked) void __stdcall dbghelp_wrapper(){__asm{jmp mProcs[217*4]}}
extern "C" __declspec(naked) void __stdcall dh_wrapper(){__asm{jmp mProcs[218*4]}}
extern "C" __declspec(naked) void __stdcall fptr_wrapper(){__asm{jmp mProcs[219*4]}}
extern "C" __declspec(naked) void __stdcall homedir_wrapper(){__asm{jmp mProcs[220*4]}}
extern "C" __declspec(naked) void __stdcall inlinedbg_wrapper(){__asm{jmp mProcs[221*4]}}
extern "C" __declspec(naked) void __stdcall itoldyouso_wrapper(){__asm{jmp mProcs[222*4]}}
extern "C" __declspec(naked) void __stdcall lmi_wrapper(){__asm{jmp mProcs[223*4]}}
extern "C" __declspec(naked) void __stdcall lminfo_wrapper(){__asm{jmp mProcs[224*4]}}
extern "C" __declspec(naked) void __stdcall omap_wrapper(){__asm{jmp mProcs[225*4]}}
extern "C" __declspec(naked) void __stdcall optdbgdump_wrapper(){__asm{jmp mProcs[226*4]}}
extern "C" __declspec(naked) void __stdcall optdbgdumpaddr_wrapper(){__asm{jmp mProcs[227*4]}}
extern "C" __declspec(naked) void __stdcall srcfiles_wrapper(){__asm{jmp mProcs[228*4]}}
extern "C" __declspec(naked) void __stdcall stack_force_ebp_wrapper(){__asm{jmp mProcs[229*4]}}
extern "C" __declspec(naked) void __stdcall stackdbg_wrapper(){__asm{jmp mProcs[230*4]}}
extern "C" __declspec(naked) void __stdcall sym_wrapper(){__asm{jmp mProcs[231*4]}}
extern "C" __declspec(naked) void __stdcall symsrv_wrapper(){__asm{jmp mProcs[232*4]}}
extern "C" __declspec(naked) void __stdcall vc7fpo_wrapper(){__asm{jmp mProcs[233*4]}}
extern "C" __declspec(naked) void __stdcall ExportByOrdinal1001(){__asm{jmp mProcs[234*4]}}
extern "C" __declspec(naked) void __stdcall ExportByOrdinal1002(){__asm{jmp mProcs[235*4]}}
extern "C" __declspec(naked) void __stdcall ExportByOrdinal1003(){__asm{jmp mProcs[236*4]}}
extern "C" __declspec(naked) void __stdcall ExportByOrdinal1004(){__asm{jmp mProcs[237*4]}}
extern "C" __declspec(naked) void __stdcall ExportByOrdinal1005(){__asm{jmp mProcs[238*4]}}
extern "C" __declspec(naked) void __stdcall ExportByOrdinal1006(){__asm{jmp mProcs[239*4]}}
extern "C" __declspec(naked) void __stdcall ExportByOrdinal1007(){__asm{jmp mProcs[240*4]}}
extern "C" __declspec(naked) void __stdcall ExportByOrdinal1008(){__asm{jmp mProcs[241*4]}}
extern "C" __declspec(naked) void __stdcall ExportByOrdinal1009(){__asm{jmp mProcs[242*4]}}
extern "C" __declspec(naked) void __stdcall ExportByOrdinal1010(){__asm{jmp mProcs[243*4]}}
extern "C" __declspec(naked) void __stdcall ExportByOrdinal1011(){__asm{jmp mProcs[244*4]}}
extern "C" __declspec(naked) void __stdcall ExportByOrdinal1012(){__asm{jmp mProcs[245*4]}}
extern "C" __declspec(naked) void __stdcall ExportByOrdinal1013(){__asm{jmp mProcs[246*4]}}
extern "C" __declspec(naked) void __stdcall ExportByOrdinal1014(){__asm{jmp mProcs[247*4]}}
extern "C" __declspec(naked) void __stdcall ExportByOrdinal1015(){__asm{jmp mProcs[248*4]}}
extern "C" __declspec(naked) void __stdcall ExportByOrdinal1016(){__asm{jmp mProcs[249*4]}}
