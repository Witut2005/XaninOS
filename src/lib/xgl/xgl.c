

#include <float.h>
#include <lib/xgl/xgl.h>
#include <lib/xgl/vga_rgb.h>

static uint8_t *vga_buffer;

void xgl_init(xgm_t mode, VGA_COLOR_PALETTE pallete)
{
    vga_mode_set(mode);
    // uint8_t* buf = vga_get_buffer_segment();

    // //TODO
    // for(int i = 0; i < 200; i++) {
    //     for(int j = 0; j < 320; j++) {
    //         pixel_linear_set(i * 320 + j, 0);
    //     }
    // }

    vga_load_palette(pallete);
}

//TODO
void pixel_set(uint32_t x, uint32_t y, uint8_t given_color)
{
    // uint8_t* buf = vga_get_buffer_segment();

}

void pixel_linear_set(uint32_t offset, uint8_t color)
{
    // Używamy wskaźnika 32-bitowego, aby zagwarantować poprawną arytmetykę
    // Adres docelowy to 32-bitowy adres fizyczny
    volatile uint8_t *vram_ptr = (volatile uint8_t*)0xA0000; 
    
    // Zapisujemy pod adresem VRAM_BASE + offset
    // To wymaga, aby kompilator poprawnie używał adresowania 32-bitowego 
    // dla *vram_ptr
    vram_ptr[offset] = color;
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

