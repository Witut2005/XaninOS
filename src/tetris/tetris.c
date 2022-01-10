#pragma once

#include <tetris/graphics.h>

void tetris(void)
{
    printCube();

    while(1)
    {
        update();
        if(keyboard_input == 'q')
            {app_exited = true;return;}
    }

}