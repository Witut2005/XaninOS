#pragma once

#include <lib/libc/stdiox.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/time.h>


void timer_input(void)
{
    if(getscan() == ENTER)
        app_exited = true;
}

int timer_test(void)
{

    keyboard_handle = timer_input;

    uint32_t current_time = 0;

    screen_clear();

    xprintf("Press 'a' to start...");
    while(inputc() != 'a');

    while(!app_exited)
    {
        screen_clear();
        xprintf("%d", current_time);
        current_time++;
        msleep(1000);
    }

    return XANIN_OK;

}