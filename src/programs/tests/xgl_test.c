
#include <lib/xgl/xgl.h>
#include <lib/libc/stdiox.h>
#include <sys/devices/com/com.h>

//CANVAS_APP

int xgl_test(void)
{
    stdio_mode_set(STDIO_MODE_CANVAS);
    xgl_init(VGA_GRAPHICS_320x200x256, VGA_PALETTE_GRAYSCALE);

    uint8_t* vga_buf = (uint8_t*)vga_get_buffer_segment();
    
    for(int i = 0; i < 200; i++) {
        memset(vga_buf + (i * 320), i, 160);
    }
    
    dbg_info("XGL_TEST", "jeden");
    while(1);
    dbg_info("XGL_TEST", "ugabgua");
    return XANIN_OK;
}