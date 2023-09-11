
#include <lib/libc/colors.h>
#include <sys/devices/keyboard/key_map.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/string.h>
#include <sys/screen/screen.h>

typedef void (*keyboard_handle_input)(void);

keyboard_handle_input keyboard_handle = NULL;

uint8_t logo_front_color = yellow;
uint8_t logo_back_color = black;
 
screen_t Screen;
uint16_t** screen_rows;

void __screen_init(void)
{
    // USE THIS -> screen_rows = (uint16_t**)kcalloc(VGA_HEIGHT * sizeof(uint16_t*));
    screen_rows = (uint16_t**)calloc(VGA_HEIGHT * sizeof(uint16_t*));
    Screen.cursor = screen_rows;

    for(int i = 0; i < VGA_HEIGHT; i++)
        Screen.cursor[i] = (uint16_t*)(VGA_TEXT_MEMORY + (VGA_WIDTH * i * 2));  

    Screen.x = 0;
    Screen.y = 0;
}

void __letters_refresh(uint16_t* cursor_current_positon)
{
    return;
    cursor_current_positon++;

    for(uint16_t* i = cursor_current_positon; (uint32_t)i < (uint32_t)(VGA_TEXT_MEMORY + VGA_SCREEN_RESOLUTION * 2); i++)
        *(i - 1) = *i;
}

void __letters_refresh_add(uint16_t* cursor_current_positon, char character_saved)
{
    return;
    char tmp;

    for(uint16_t* i = cursor_current_positon; (uint32_t)i < (uint32_t)(VGA_TEXT_MEMORY + VGA_SCREEN_RESOLUTION * 2); i++)
    {
        tmp = *(char*)(i);
        *i = (uint16_t)((char)(character_saved) + (((black << 4) | white) << 8));
        character_saved = tmp;
    }

    *cursor_current_positon = (uint16_t)((char)(*cursor_current_positon) + (((black << 4) | white) << 8));

}