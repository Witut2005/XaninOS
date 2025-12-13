
#include <lib/xgl/xgl.h>
#include <lib/libc/stdiox.h>
#include <sys/devices/com/com.h>

//CANVAS_APP

int xgl_test(void)
{
    stdio_mode_set(STDIO_MODE_CANVAS);
    xgl_init(VGA_GRAPHICS_320x200x256);

    uint8_t* vga_buf = (uint8_t*)0xA0000;
    
    for(int i = 0; i < 128; i++) {
        memset(vga_buf + (i * 320), i, 320);
    }
    
    dbg_info("XGL_TEST", "jeden");
    while(1);
    dbg_info("XGL_TEST", "ugabgua");
    return XANIN_OK;
}