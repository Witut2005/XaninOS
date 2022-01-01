#pragma once

#include "./graphics.h"

void tetris(void)
{
    printCube();

    while(1)
    {
        update();
        if(input == 'q')
            {exitApp = true;return;}
    }

}