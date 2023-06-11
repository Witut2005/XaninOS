
#include <lib/libc/string.h>
#include <lib/libc/stdiox.h>

int screen_saver(char* filename)
{

    stdio_mode_set(STDIO_MODE_CANVAS);

    char* buf = (char*)calloc(VGA_SCREEN_RESOLUTION * sizeof(terminal_cell));

    XinEntry* File = fopen(filename, "r");
    fread(File, buf, File->size);

    screen_clear();

    xprintf("%h%s", OUTPUT_POSITION_SET(VGA_MAX_Y - 10, 80), buf);

    StringRectangle* StringRect = string_rectangle_create(buf, VGA_WIDTH / 2, VGA_HEIGHT / 2);

    while(KeyInfo.scan_code != ENTER)
    {

        terminal_cell* tmp = (terminal_cell*)VGA_TEXT_MEMORY;
        if((char)*tmp != '\0')
        {
            screen_clear();
            xprintf("%h%s", OUTPUT_POSITION_SET(VGA_MAX_Y - 7, 80), buf);
        }

        for(int i = 0; i < VGA_SCREEN_RESOLUTION; i++)
            tmp[i] = (char)(tmp[i + 1]) | (OUTPUT_COLOR_SET(black, rand()) << 8);

        msleep(200);
    }

    return XANIN_OK;

}