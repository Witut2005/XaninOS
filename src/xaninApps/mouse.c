
#pragma once

extern void mouse_enable(void);
extern void mouse_handler(void);

#include <lib/stdiox.h>

void mouse_test(char *x)
{

    while(1)
    {

        screen_clear();
        mouse_handler();

        msleep(5);

    }

}