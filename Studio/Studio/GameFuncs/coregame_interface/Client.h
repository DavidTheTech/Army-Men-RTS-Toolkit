#pragma once
#include <Windows.h>
#include "..\..\Memory\Memory.h"

class Client
{
public:
    static DWORD GetMouse();
    static DWORD GetPlayer();

    class Cmd
    {
    public:
        static void LogTasks(DWORD* obj);
    };
};