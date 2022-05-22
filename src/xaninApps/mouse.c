
#pragma once



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