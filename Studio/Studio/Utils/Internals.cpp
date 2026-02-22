#include "Internals.h"

const char * Internals::GetDate(bool u)
{
    static char buffer[20];
    std::time_t now = std::time(nullptr);
    std::tm local_time = { 0 };
    localtime_s(&local_time, &now);
    if (u)
    {
        std::strftime(buffer, sizeof(buffer), "%Y_%m_%d_%H_%M_", &local_time);
    }
    else
    {
        std::strftime(buffer, sizeof(buffer), "%Y_%m_%d_%H_%M", &local_time);
    }

    return buffer;
}