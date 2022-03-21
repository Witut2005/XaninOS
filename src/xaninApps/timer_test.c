#pragma once

#include <lib/stdiox.h>
#include <lib/stdlibx.h>
#include <lib/time.h>


void timer_input(void)
{
    if(getscan() == ENTER)
        app_exited = true;
}

void timer_test(void)
{

    keyboard_handle = timer_input;

    uint32_t current_time = 0;

    screen_clear();

    xprintf("Press 'a' to start...");
    while(getchar() != 'a');

    KeyInfo.scan_code = 0x0;

    while(!app_exited)
    {
        screen_clear();
        xprintf("%d", current_time);
        current_time++;

        
        msleep(1000);

    }
}