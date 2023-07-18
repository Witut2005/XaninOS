
#include <devices/vga/vga.h>

inline static uint8_t* __vga_buffer_segment_get(void)
{
    return vga_get_buffer_segment();
}