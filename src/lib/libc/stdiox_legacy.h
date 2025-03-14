
#pragma once

#include <lib/screen/screen.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

bool stdio_legacy_cell_compare(uint8_t y, uint8_t x, vga_screen_cell_t color);
bool stdio_legacy_cell_color_compare(uint8_t y, uint8_t x, color_t color);
bool stdio_legacy_cell_foreground_color_compare(uint8_t y, uint8_t x, color_t fcolor);
bool stdio_legacy_cell_background_color_compare(uint8_t y, uint8_t x, color_t bcolor);

bool stdio_legacy_canvas_is_buffer_full(void);
bool stdio_legacy_cell_put(char character, uint8_t color, uint8_t y, uint8_t x);                       // czy interpretowac
bool stdio_legacy_cell_put_with_interpretation(char character, uint8_t color, uint8_t* y, uint8_t* x); // czy interpretowac
void stdio_legacy_screen_object_restore_defaults(void);
uint16_t* stdio_legacy_vga_position_get(uint8_t y, uint8_t x);
void stdio_legacy_move_to_y(uint8_t y);
void stdio_legacy_move_to_x(uint8_t x);

static inline uint8_t stdio_legacy_get_last_valid_y(void)
{
    return VGA_HEIGHT - 1;
}

static inline uint8_t stdio_legacy_get_last_valid_x(void)
{
    return VGA_WIDTH - 1;
}

#ifdef __cplusplus
}
#endif
