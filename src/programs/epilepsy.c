

#include <lib/libc/stdiox.h>
#include <lib/libc/stdlibx.h>

int epilepsy(void)
{
    uint16_t* color_picker = (uint16_t*)VGA_TEXT_MEMORY;
    uint8_t color;

    stdio_mode_set(STDIO_MODE_CANVAS);

    while(true)
    {
        if((uint32_t)color_picker >= VGA_TEXT_MEMORY + (80 * 28 * 2))
            color_picker = (uint16_t*)VGA_TEXT_MEMORY;

        color = rand() % 16;    
        *color_picker = (uint16_t) ('\0' + (((color << 4) | color) << 8));

        color_picker++;

        if(getchar() == 'q' || getscan() == ENTER || getscan() == ESC)
        {    
            return XANIN_OK;
        }
    }


}