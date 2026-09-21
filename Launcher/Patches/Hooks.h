#pragma once

#include <string>
#include "Minhook\MinHook.h"

class Hooks
{
public:
	static bool Setup();
	static void SetupDirs();
	static void LaunchStudio();
	static void CursorLock();
};