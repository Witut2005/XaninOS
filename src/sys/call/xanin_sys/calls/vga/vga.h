
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

uint8_t* __vga_buffer_segment_get(void);
uint32_t __vga_text_mode_width_get(void);
uint32_t __vga_text_mode_height_get(void);

#ifdef __cplusplus
}
#endif

#define VGA_WIDTH (__vga_text_mode_width_get())
#define VGA_HEIGHT (__vga_text_mode_height_get())
#define VGA_TEXT_MEMORY (__vga_buffer_segment_get())
#define VGA_SCREEN_RESOLUTION (VGA_WIDTH * VGA_HEIGHT)
#define BLANK_SCREEN_CELL 0
#define VGA_MAX_Y (VGA_HEIGHT - 1)
#define VGA_SCREEN_CELL_SIZE 2