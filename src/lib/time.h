
#pragma once

#include <lib/stdlibx.h>

void sleep(int seconds)
{
    for(uint32_t i = 0; i < seconds; i++)
    {
        for(uint32_t j = 0; j < (cpu_mhz * 15); j++);
    }
}


void sleep5(int seconds5)
{
    for(uint32_t i = 0; i < seconds5; i++)
    {
        for(uint32_t j = 0; j < (cpu_mhz * 3); j++);
    }
}



void sleepf(int seconds5)
{
    for(uint32_t i = 0; i < seconds5; i++)
    {
        for(uint32_t j = 0; j < (cpu_mhz / 100); j++);
    }
}