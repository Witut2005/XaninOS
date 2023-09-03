#pragma once

#include <programs/tetris/graphics.h>
#include <sys/input/input.h>

int ptetris(void)
{
    stdio_mode_set(STDIO_MODE_CANVAS);
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