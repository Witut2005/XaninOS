
#include "./bootio.h"
#include "./string.h"

#define VGA_HEIGHT 25
#define VGA_WIDTH 80

vga_cell_t* print_ptr = (uint16_t*)0xb8000;
uint32_t bootio_offset = 0;

void putchar(char c, color_t color)
{
    if(c == '\n')
        bootio_offset = bootio_offset + (VGA_WIDTH - (bootio_offset % 80));

    else
        print_ptr[bootio_offset++] = c | VGA_COLOR(color);
}

void print(const char* str, color_t color)
{
    while(*str != '\0')
    {
        putchar(*str, color);
        str++;
    }
}

void print_decimal(uint32_t value, color_t color)
{
    char buf[20] = {0};
    print(int_to_str(value, buf), color);
}

void print_hex(uint32_t value, color_t color)
{
    char buf[20] = {0};
    print(int_to_hex_str(value, buf), color);
}

void vga_screen_buffer_clear(void)
{
    vga_cell_t* screen_cleaner = (uint16_t*)0xb8000;
    for(int i = 0; i < (VGA_HEIGHT * VGA_WIDTH); i++)
    {
        *screen_cleaner = '\0';
        screen_cleaner++;
    }
    bootio_offset = 0;
}
