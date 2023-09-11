
#pragma once

#include <lib/libc/stdiox.h>

extern void mouse_enable(void);
extern void mouse_handler(void);

int mouse_test(void)
{

    mouse_enable();

    while(1)
    {

        // screen_clear();
        mouse_handler();
        msleep(5);
    }
    return XANIN_OK;

}