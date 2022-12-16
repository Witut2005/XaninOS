#pragma once

#include <xaninApps/tetris/graphics.h>

int ptetris(void)
{
    screen_clear();
    printCube();

    while(1)
    {
        update();
        if(KeyInfo.character == 'q')
            {app_exited = true;return XANIN_OK;}
    }
    return XANIN_OK;

}