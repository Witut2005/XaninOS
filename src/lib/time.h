
#pragma once

#include <lib/stdlibx.h>

void sleep(int seconds)
{
    for(uint32_t i = 0; i < seconds; i++)
    {
        for(uint32_t j = 0; j < (cpu_mhz * 15); j++);
    }
}