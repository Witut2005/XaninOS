
#include <lib/libc/colors.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/string.h>
#include <lib/screen/screen.h>
#include <sys/devices/keyboard/key_map.h>

typedef void (*keyboard_handle_input)(void);

uint8_t logo_front_color = lgray;
uint8_t logo_back_color = black;

screen_t Screen;
uint16_t** screen_rows;

void screen_init(void)
{
    // USE THIS -> screen_rows = (uint16_t**)kcalloc(VGA_HEIGHT * sizeof(uint16_t*));
    screen_rows = (uint16_t**)calloc(VGA_HEIGHT * sizeof(uint16_t*));
    Screen.cursor = screen_rows;

    for (int i = 0; i < VGA_HEIGHT; i++)
        Screen.cursor[i] = (uint16_t*)(VGA_TEXT_MEMORY + (VGA_WIDTH * i * 2));

    Screen.x = 0;
    Screen.y = 0;
}
