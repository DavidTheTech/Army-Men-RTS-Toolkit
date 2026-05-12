#pragma once
#include <Windows.h>
#include "..\..\Memory\Memory.h"

class MBEvent;

namespace IFace
{
    void MsgBox(const char* title, const char* message, const char* buttonText = nullptr);
    void MsgBox(const char* title, const char* message, MBEvent* btn1, MBEvent* btn2 = nullptr, MBEvent* btn3 = nullptr);
}