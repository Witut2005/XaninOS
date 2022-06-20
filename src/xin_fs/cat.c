

#include <libc/stdiox.h>
#include <xin_fs/xin.c>
#include <libc/memory.h>
#include <terminal/interpreter.h>
#include <libc/string.h>


void cat(char* file_name)
{
    screen_clear();
    use_backspace = true;

    xin_entry* xin_file = fopen(file_name, "rw");

    if(xin_file == nullptr)
    {
        xprintf("Could not open file: %s\n", file_name);

        while(KeyInfo.scan_code != ENTER);

    }
        
    else
    {

        char* data_pointer = xin_file->starting_sector * SECTOR_SIZE;

        uint16_t* bruh_moment = VGA_TEXT_MEMORY;
        
        for(int i = 0;  i < (VGA_SCREEN_RESOLUTION / 2); i++)
            bruh_moment[i] = (uint16_t) (data_pointer[i] + (((black << 4) | white) << 8));
        

        while(inputg().scan_code != F4_KEY && inputg().scan_code == ESC);

        uint32_t file_data_counter = 0x0;

        data_pointer = (char*)(xin_file->starting_sector * SECTOR_SIZE);

        //if(xin_file->os_specific != XIN_READ_ONLY)
        {

            uint16_t* screen_ptr = (uint16_t*)VGA_TEXT_MEMORY;

            for(int i = 0; i < VGA_SCREEN_RESOLUTION; i++, screen_ptr++)
            {
                fseek(xin_file, i);
                write(xin_file, (char*)screen_ptr, 1);
            }

            xin_file->entry_size = file_data_counter;
        }

    }


}
