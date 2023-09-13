#pragma once

#include <programs/tetris/graphics.h>
#include <sys/input/input.h>

//CANVAS_APP

int ptetris(void)
{
    stdio_mode_set(STDIO_MODE_CANVAS);
    canvas_screen_clear();
    printCube();

    while(1)
    {
        update();
        if(KeyInfo.character == 'q')
            {app_exited = true;return XANIN_OK;}
    }
    return XANIN_OK;

}