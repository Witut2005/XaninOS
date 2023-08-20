
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

bool stdio_legacy_canvas_is_buffer_full(void);
bool stdio_legacy_cell_put(char character, uint8_t color, uint8_t x, uint8_t y);
void stdio_legacy_screen_object_restore_defaults(void);

#ifdef __cplusplus
}
#endif