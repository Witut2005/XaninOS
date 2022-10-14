

#include <libc/stdiox.h>
#include <xin_fs/xin.h>
#include <libc/memory.h>
#include <terminal/interpreter.h>
#include <libc/string.h>


void cat(char* file_name)
{

    xin_entry* xin_file = fopen(file_name, "rw");

    if(xin_file == nullptr)
    {
        xprintf("Could not open file: %s\n", file_name);

        while(KeyInfo.scan_code != ENTER);

    }
        
    else
    {
        uint8_t* file_data = (uint8_t*)calloc(VGA_SCREEN_RESOLUTION);

        read(xin_file, file_data, VGA_SCREEN_RESOLUTION);
        xprintf("%s", xin_file->file_info->base_address_memory);
        while(KeyInfo.scan_code != ENTER);
        free(file_data);

    }


}
