

#include <lib/libc/stdiox.h>
#include <fs/xin.h>
#include <lib/libc/file.h>
#include <lib/libc/memory.h>
#include <sys/terminal/interpreter/interpreter.h>
#include <lib/libc/string.h>

// TERMINAL_APP

int cat(char *file_name)
{

    XinEntry *xin_file = fopen(file_name, "rw");

    if (xin_file == NULL)
    {
        xprintf("Could not open file: %s\n", file_name);
        return XANIN_ERROR;
    }

    else
    {
        uint8_t *file_data = (uint8_t *)calloc(VGA_SCREEN_RESOLUTION);

        fread(xin_file, file_data, VGA_SCREEN_RESOLUTION);
        xprintf("%s", xin_file->FileInfo->buffer);
        free(file_data);
        return XANIN_OK;
    }
}
