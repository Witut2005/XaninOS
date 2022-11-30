

#include <devices/VGA/vga.h>
#include <xgl/xgl.h>

void xgl_init(void)
{
   vga_mode_set(VGA_GRAPHICS_320x200x256); 
}

void pixel_set(uint32_t x, uint32_t y, uint8_t given_color) 
{
    uint8_t* vga_pointer = vga_get_buffer_segment();
    vga_pointer[y * 320 + x] = color(given_color);
}

void rectangle_create(uint32_t x, uint32_t y, uint32_t x_size, uint32_t y_size, uint8_t given_color)
{


    for(int i = y; i < y + y_size; i++)
    {
        for(int j = x; j < x + x_size; j++)
            pixel_set(j, i, given_color);
    }

}
