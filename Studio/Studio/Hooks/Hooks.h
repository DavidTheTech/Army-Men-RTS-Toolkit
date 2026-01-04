#pragma once

#include <string>

#include <Minhook\MinHook.h>
#include <LuaJIT\src\lua.hpp>

//#pragma comment(lib, "libMinHook.x86.lib")

class Hooks
{
public:
	static DWORD WINAPI MinHookStuff(HINSTANCE hModule);
};