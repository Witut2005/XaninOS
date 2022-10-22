

#include <libc/stdiox.h>
#include <xin_fs/xin.h>
#include <libc/memory.h>
#include <libc/string.h>


static uint16_t* cursor;


int xin_xpaint(char* file_name)
{

    xin_entry* xin_file = fopen(file_name, "rw");

    if(xin_file == nullptr)
    {
        xprintf("Could not open file %s\n", file_name);
        while(KeyInfo.scan_code != ENTER);
        return XANIN_ERROR;
    }

    else
    {
    
        uint16_t* data_pointer = (uint16_t*)calloc(VGA_SCREEN_RESOLUTION * 4);
        read(xin_file, data_pointer, VGA_SCREEN_RESOLUTION * 4);
        screen_clear();

        uint16_t* screen_cell = (uint16_t*)VGA_TEXT_MEMORY;

        for(int i = 0; i < VGA_SCREEN_RESOLUTION * 2; i++)
            screen_cell[i] = (uint16_t) (data_pointer[i]);

        while(KeyInfo.scan_code != ENTER);

    }

    fclose(&xin_file);
    return XANIN_OK;

}

// 21:16:21