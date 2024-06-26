
#include <lib/libc/stdiox.h>
#include <fs/xin.h>
#include <lib/libc/file.h>
#include <lib/libc/memory.h>
#include <sys/terminal/interpreter/interpreter.h>
#include <lib/libc/string.h>
#include <lib/libc/hal.h>
#include <sys/input/input.h>
#include <sys/devices/hda/disk.h>

// TERMINAL_APP

int run16(char* file_name)
{
    screen_clear();

    XinEntry* xin_file = __xin_find_entry(file_name);

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

        uint8_t* dest = (uint8_t*)0x10000;
        // fread(xin_file, dest, xin_file->size);
        // fread(xin_file, dest, SECTOR_SIZE);
        disk_sectors_read(ATA_FIRST_BUS, ATA_MASTER, xin_file->first_sector, 1, (uint16_t*)dest);

        // xprintf("nicho");
        // while(getxchar().scan_code == ENTER);

        real_mode_enter_no_return(0x1000, 0x0); // <-- tmp.bin address in memory
    }

    return XANIN_OK;
}