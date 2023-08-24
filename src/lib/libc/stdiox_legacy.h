
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

bool stdio_legacy_canvas_is_buffer_full(void);
bool stdio_legacy_cell_put(char character, uint8_t color, uint8_t* y, uint8_t* x); // czy interpretowac
bool stdio_legacy_cell_put_with_interpretation(char character, uint8_t color, uint8_t* y, uint8_t* x); // czy interpretowac
void stdio_legacy_screen_object_restore_defaults(void);

#ifdef __cplusplus
}
#endif