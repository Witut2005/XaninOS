

#include <lib/libc/stdiox.h>
#include <lib/libc/stdlibx.h>

//CANVAS_APP

int epilepsy(void)
{
    uint16_t* color_picker = (uint16_t*)VGA_TEXT_MEMORY;
    uint8_t color;

    stdio_mode_set(STDIO_MODE_CANVAS);

    key_info_t EpilepsyKeyInfo;

    while(true)
    {
        if((uint32_t)color_picker >= (uint32_t)(VGA_TEXT_MEMORY + (80 * 28 * 2)))
            color_picker = (uint16_t*)VGA_TEXT_MEMORY;

        color = rand() % 16;    
        *color_picker = (uint16_t) ('\0' + (((color << 4) | color) << 8));

        color_picker++;
        __sys_keyinfo_get(&EpilepsyKeyInfo);

        if(EpilepsyKeyInfo.scan_code != ESC)
            return XANIN_OK;
    }


}