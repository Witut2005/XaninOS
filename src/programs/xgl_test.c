
#pragma once

#include <lib/xgl/xgl.h>
#include <lib/libc/stdiox.h>

int xgl_test(void)
{

    stdio_mode_set(STDIO_MODE_CANVAS);
    // xprintf("rgb2vga: %d\n", rgb2vga(0xFF, 0, 0));
    xgl_init(VGA_GRAPHICS_320x200x256_MODEX);

    // for(int i = 0; i < 320 * 100; i++)
    pixel_set(0, 0, 14);
    
    getchar();
    // while(1);

}
