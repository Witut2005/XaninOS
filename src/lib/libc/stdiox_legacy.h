
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <lib/screen/screen.h>

#ifdef __cplusplus
extern "C" {
#endif

bool stdio_legacy_canvas_is_buffer_full(void);
bool stdio_legacy_cell_put(char character, uint8_t color, uint8_t* y, uint8_t* x); // czy interpretowac
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

#define stdio_canvas_is_buffer_full stdio_legacy_canvas_is_buffer_full
#define stdio_canvas_cell_put stdio_legacy_cell_put
#define stdio_canvas_cell_put_with_interpretation stdio_legacy_cell_put_with_interpretation
#define stdio_canvas_screen_object_restore_defaults stdio_legacy_screen_object_restore_defaults
#define stdio_canvas_vga_position_get stdio_legacy_vga_position_get
#define stdio_canvas_move_to_y stdio_legacy_move_to_y
#define stdio_canvas_move_to_x stdio_legacy_move_to_x
#define stdio_canvas_get_last_valid_y stdio_legacy_get_last_valid_y
#define stdio_canvas_get_last_valid_x stdio_legacy_get_last_valid_x

#ifdef __cplusplus
}
#endif