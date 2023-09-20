
#include "./bootio.h"

uint16_t* print_ptr = (uint16_t*)0xb8000;
uint32_t y = 0;
uint32_t x = 0;

void print(const char* str)
{
    while(*str != '\0')
    {
        print_ptr[y * 80 + x] = *str | (0x0F << 8);
        x++;
        str++;
    }

    x = 0;
    y++;
}

void print_decimal(uint32_t value)
{
    char buf[20] = {0};
    print(int_to_str(value, buf));
    x = 0;
    y++;
}

void print_hex(uint32_t value)
{
    char buf[20] = {0};
    print(int_to_hex_str(value, buf));

    x = 0;
    y++;
}

void vga_screen_buffer_clear(void)
{
    uint16_t* screen_cleaner = (uint16_t*)0xb8000;
    for(int i = 0; i < (80 * 25); i++)
    {
        *screen_cleaner = '\0';
        screen_cleaner++;
    }
    y = x = 0;
}
