
#pragma once

#include <lib/stdlibx.h>

    uint32_t cpu_mhz = 0x0;
    uint32_t cpu_khz = 0x0;

void sleep(int seconds)
{
    for(uint32_t i = 0; i < seconds; i++)
    {
        for(uint32_t j = 0; j < (cpu_mhz * 15); j++);
        //for(uint32_t k = 0; k < ((cpu_khz / 1000) * 15); k++);
    }
}

void msleep(int mili_seconds)
{
    for(uint32_t i = 0; i < mili_seconds; i++)
    {
        for(uint32_t j = 0; j < ((cpu_khz / 1000) * 100); j++);
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