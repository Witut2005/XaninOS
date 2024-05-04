

#pragma once


#include <lib/libc/pair.h>
#include <lib/libc/memory.h>
#include <lib/libc/stdlibx.h>
#include <lib/screen/screen.h>
#include <sys/devices/keyboard/scan_codes.h>
#include <stdarg.h>
#include <lib/libc/time.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/stdiox_legacy.h>
#include <sys/terminal/backend/backend.h>

#ifdef __cplusplus
extern "C" {
#endif

    void canvas_putchar(char character); // ONLY AVAILABLE IN CANVAS MODE
    void canvas_putchar_color(uint8_t color, char character); // ONLY AVAILABLE IN CANVAS MODE
    void canvas_screen_clear(void);
    void canvas_xprintf(char* str, ...);
    void canvas_xscanf(char* str, ...);
    void canvas_xscan_range(char* string_buffer, uint32_t how_many_chars);
    void canvas_screen_background_color_set(color_t color);

#ifdef __cplusplus
}
#endif