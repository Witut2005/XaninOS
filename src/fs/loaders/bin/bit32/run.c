
#include <lib/libc/stdiox.h>
#include <fs/xin.h>
#include <lib/libc/file.h>
#include <lib/libc/memory.h>
#include <sys/terminal/interpreter/interpreter.h>
#include <lib/libc/string.h>
#include <sys/input/input.h>

// TERMINAL_APP

int run(char *file_name)
{
    // screen_clear();

    XinEntry *xin_file = fopen(file_name, "rw");

    if (xin_file == NULL)
    {
        xprintf("%zNO SUCH FILE %s\n", OUTPUT_COLOR_SET(red, white), argv[1]);

        while (getxchar().scan_code == ENTER)
            ;
        return XANIN_ERROR;
    }

    else
    {

        if (xin_file->type == XIN_DIRECTORY)
        {
            xprintf("%zYOU CANT RUN DIRECTORY\n", OUTPUT_COLOR_SET(red, white));
            xprintf("%zuse F4 key to exit\n", OUTPUT_COLOR_SET(red, white));
            while (getxchar().scan_code != ENTER)
                ;
            return XANIN_ERROR;
        }

        uint8_t *where_to_load = (uint8_t *)0x10000;

        fread(xin_file, where_to_load, xin_file->size);

        void (*ptr)(void) = (void (*)(void))where_to_load;
        ptr();

        return XANIN_OK;
    }
}
