
#include <lib/libc/string.h>
#include <lib/libc/memory.h>
#include <lib/libc/stdlibx.h>
#include <sys/terminal/vty/vty.h>
#include <sys/devices/keyboard/scan_codes.h>
#include <stdarg.h>
#include <lib/libc/time.h>
#include <lib/libc/stdiox.h>
#include <sys/terminal/backend/backend.h>


bool stdio_legacy_canvas_is_buffer_full(void)
{
    return (Screen.y + (Screen.x / VGA_WIDTH)) >= VGA_HEIGHT;
}

bool stdio_legacy_cell_put(char character, uint8_t color, uint8_t* y, uint8_t* x)
{
    if((*x >= VGA_WIDTH) || (*y >= VGA_HEIGHT))
        return false;
    Screen.cursor[*y][*x] = character | (color << 8);

    return true;
}

void stdio_legacy_screen_object_restore_defaults(void)
{
    Screen.x = 0;
    Screen.y = 0;
}

bool stdio_legacy_cell_put_with_interpretation(char character, uint8_t color, uint8_t* y, uint8_t* x) // czy interpretowac
{
    if((*x >= VGA_WIDTH) || (*y >= VGA_HEIGHT))
        return false;

    if(character == '\r')
    {
        *x = 0;
    }

    else if(character == '\0')
        return true;
    
    else if(character == '\n')
    {            
        *x = 0;
        *y += 1;
    }

    else if(character == '\t')
    {
        for(int j = 0; j < 3; j++)
        {
            if(*x + j == 80)
            {
                Screen.x = 0;
                Screen.y++;
            }
            Screen.cursor[*y][*x + j] = (uint16_t)(' ' + (color << 8));
        }

        *x += 3;
    }


    else if(character == '\\')
        Screen.cursor[*y][*x] = (uint16_t)('\\' + (color << 8));

    else
    {
        Screen.cursor[*y][*x] = character | (color << 8);

        *x += 1;
        if(*x == VGA_WIDTH)
        {
            *y += 1;
            *x = 0;
        }
    }

    if(*x >= VGA_WIDTH)
    {
        *x = *x - VGA_WIDTH;
        *y += 1;
    }

    return true;
}

uint16_t* stdio_legacy_vga_position_get(uint8_t y, uint8_t x)
{
    return &Screen.cursor[y][x];
}