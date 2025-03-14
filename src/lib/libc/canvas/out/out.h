#pragma once
#include <stdint.h>

#define stdio_canvas_cell_put stdio_legacy_cell_put
#define stdio_canvas_cell_put_with_interpretation stdio_legacy_cell_put_with_interpretation

#ifdef __cplusplus
extern "C" {
#endif

void canvas_putchar(char character);                      // ONLY AVAILABLE IN CANVAS MODE
void canvas_putchar_color(uint8_t color, char character); // ONLY AVAILABLE IN CANVAS MODE
void canvas_screen_clear(void);
void canvas_xprintf(char* str, ...);
void canvas_screen_background_color_set(uint8_t color);

#ifdef __cplusplus
}
#endif
