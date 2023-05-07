

#include <sys/devices/vga/vga.h>
#include <lib/xgl/xgl.h>

void xgl_init(void)
{
   vga_mode_set(VGA_GRAPHICS_320x200x256); 

    for(int i = 0; i < 200; i++)
    {
        for(int j = 0; j < 320; j++)
            pixel_set(j, i, 0);            
    }
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

void square_create(uint32_t x, uint32_t y, uint32_t size, uint8_t given_color)
{
    for(int i = y; i < y + size; i++)
    {
        for(int j = x; j < x + size; j++)
            pixel_set(j, i, given_color);
    }

}

void line_horizontal_create(uint32_t x, uint32_t y, uint16_t lenght, uint8_t given_color)
{
    for(int i = 0; i < lenght; i++)
        pixel_set(x+i, y, given_color);
}

// void circle_create(uint32_t x, uint32_t y, uint32_t radius, uint8_t given_color)
// {

//     if(radius % 2 == 0)
//         radius--;

//     uint32_t number_of_line_calls = (radius / 2);

//     line_horizontal_create(x - (radius / 2), y, radius, given_color);
//     radius = radius - 2;

//     uint32_t y_up = y;
//     uint32_t y_down = y;

//     for(int i = number_of_line_calls; i > 0; i--)
//     {
//         line_horizontal_create(x - (radius / 2), --y_up, radius, given_color);
//         line_horizontal_create(x - (radius / 2), ++y_down, radius, given_color);
        
//         if()
//             radius = radius - 4;
//     }
// }

//sesciokat
// void circle_create(uint32_t x, uint32_t y, uint32_t radius, uint8_t given_color)
// {

//     if(radius % 2 == 0)
//         radius--;

//     uint32_t number_of_line_calls = (radius / 2);

//     line_horizontal_create(x - (radius / 2), y, radius, given_color);
//     radius = radius - 2;

//     uint32_t y_up = y;
//     uint32_t y_down = y;

//     for(int i = number_of_line_calls; i > 0; i--)
//     {
//         line_horizontal_create(x - (radius / 2), --y_up, radius, given_color);
//         line_horizontal_create(x - (radius / 2), ++y_down, radius, given_color);
//         if(i % 2 == 0)
//             radius = radius - 2;
//     }
// }