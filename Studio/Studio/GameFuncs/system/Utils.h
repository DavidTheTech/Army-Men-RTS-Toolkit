#pragma once
#include <Windows.h>
#include "..\..\Memory\Memory.h"

class Utils
{
public:
    static unsigned long Strlen(const char* str);
    static char* Strmcpy(char* Destination, char* Source, size_t Count);
};

