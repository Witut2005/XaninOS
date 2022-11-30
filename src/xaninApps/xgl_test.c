
#pragma once

#include <devices/VGA/vga.c>

int xgl_test(void)
{
    vga_mode_set(VGA_GRAPHICS_320x200x256);
    uint8_t* vga_pointer = vga_get_buffer_segment();

    for(int i = 0; i < 320; i++)
    {
        for(int j = 0; j < 200; j++)
        {
            vga_pointer[i * 200 + j] = ((0xFF & 0xFF) >> 2);
        }
    }

}
