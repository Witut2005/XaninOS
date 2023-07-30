
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