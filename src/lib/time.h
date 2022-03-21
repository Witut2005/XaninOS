
#pragma once

#include <lib/stdlibx.h>

    uint32_t cpu_mhz = 0x0;
    uint32_t cpu_khz = 0x0;


void sleep(float seconds)
{
    float now = pit_time;

    while(pit_time < now + seconds);

}

void msleep(float miliseconds)
{

    miliseconds = miliseconds / 1000;

    float now = pit_time;

    while(pit_time < now + miliseconds);

}