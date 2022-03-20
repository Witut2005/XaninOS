#pragma once

#include <lib/stdiox.h>
#include <lib/stdlibx.h>
#include <lib/time.h>


void timer_test(void)
{

    uint32_t current_time = 0;

    xprintf("Press ENTER to start...");
    while(getscan() != ENTER);

    while(getscan() != ENTER)
    {
        screen_clear();
        xprintf("%d", current_time);
        current_time++;

        
        msleep(1000);

    }
}