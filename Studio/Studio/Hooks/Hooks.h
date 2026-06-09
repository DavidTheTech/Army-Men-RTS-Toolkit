//ONLY NEEDS Setup as a external call
//everything else is on the fly calls
#pragma once

#include <string>
#include <Minhook\MinHook.h>
#include <LuaJIT\src\lua.hpp>

#include "..\GameFuncs\util\VarSys.h"

//#pragma comment(lib, "libMinHook.x86.lib")

class Hooks
{
public:
	static bool Setup();
	static void SetupDirs();
};