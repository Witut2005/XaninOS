
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

bool stdio_legacy_cell_put(char character, uint8_t color, uint8_t x, uint8_t y)
{
    if((x >= VGA_WIDTH) || (y >= VGA_HEIGHT))
        return false;
    Screen.cursor[y][x] = character | (color << 8);

    return true;
}

void stdio_legacy_screen_object_restore_defaults(void)
{
    Screen.x = Screen.y = 0;
}