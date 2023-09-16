
#include <lib/tui/tui.h>
#include <lib/libc/canvas.h>
#include <sys/flow/exit_codes.h>

//CANVAS_APP

__STATUS xgl_mode_set(void)
{
    stdio_mode_set(STDIO_MODE_CANVAS);
    canvas_screen_clear();
    table_t* Table = table_create(0, 5, 5, 30, black, white, 1, "320x200x4", "640x480x16", "720x480x16", "320x200x256", "320x200x256_modex");

    table_row_select(Table);
    return XANIN_OK;
}