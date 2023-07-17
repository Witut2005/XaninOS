
#pragma once

#include <lib/xgl/xgl.h>

int xgl_test(void)
{

    xgl_init(VGA_GRAPHICS_320x200x256);

    for(int i = 0; i < 10; i++)
        rectangle_create(0, i * 20, 100, 20, rand());

    // for(int i = 0; i < 200; i++)
    //     line_horizontal_create(0, i, 320, rand());


    while(1);

}
