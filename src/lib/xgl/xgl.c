

#include <float.h>
#include <lib/xgl/xgl.h>
#include <lib/xgl/vga_rgb.h>

static uint8_t *vga_buffer;

void xgl_init(xgm_t mode)
{
    vga_mode_set(mode);
    vga_buffer = (uint8_t *)0xA0000;

    for (int i = 0; i < 200; i++)
    {
        for (int j = 0; j < 320; j++)
            pixel_set(j, i, 0);
    }
}

uint8_t color(uint8_t given_color)
{
    return given_color >> 2;
}

void pixel_set(uint32_t x, uint32_t y, uint8_t given_color)
{
    // uint8_t plane = given_color & 0x3;
    uint8_t first = given_color & 0x4;
    // uint8_t second = given_color >> 4;

    set_plane(0);
    vga_buffer[y * 320 + x] = first >> 2;

    set_plane(1);
    vga_buffer[y * 320 + x] = first & 3;
}

void pixel_set_rgb(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b)
{
    pixel_set(x, y, rgb2vga(r, g, b));
}

void rectangle_create(uint32_t x, uint32_t y, uint32_t x_size, uint32_t y_size, uint8_t given_color)
{

    for (int i = y; i < y + y_size; i++)
    {
        for (int j = x; j < x + x_size; j++)
            pixel_set(j, i, given_color);
    }
}

void square_create(uint32_t x, uint32_t y, uint32_t size, uint8_t given_color)
{
    for (int i = y; i < y + size; i++)
    {
        for (int j = x; j < x + size; j++)
            pixel_set(j, i, given_color);
    }
}

void line_horizontal_create(uint32_t x, uint32_t y, uint16_t lenght, uint8_t given_color)
{
    for (int i = 0; i < lenght; i++)
        pixel_set(x + i, y, given_color);
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

// sesciokat
//  void circle_create(uint32_t x, uint32_t y, uint32_t radius, uint8_t given_color)
//  {

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

// orignal code
// https://gist.github.com/harieamjari/509f665081f52b3dbdfc892b39cc3eab

uint8_t rgb2vga(int r, int g, int b)
{

    uint32_t best_value = UINT32_MAX;
    uint8_t best_index = 0;

    for (int i = 0; i < 256; i++)
    {
        uint32_t current_value = abs(r - vga_rgb_palette[i][0]) + abs(g - vga_rgb_palette[i][1]) + abs(b - vga_rgb_palette[i][2]);

        if (best_value > current_value)
        {
            best_value = current_value;
            best_index = i;
        }
    }

    return best_index;
}
