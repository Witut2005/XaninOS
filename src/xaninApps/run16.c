
#pragma once

#include <lib/stdiox.h>
#include <xin_fs/xin.c>
#include <lib/memory.h>
#include <terminal/interpreter.h>
#include <lib/exit_process.h>
#include <lib/string.h>


void run16(char* file_name)
{
    screen_clear();

    xin_entry* xin_file = xin_find_entry(file_name);
    
    if(xin_file == nullptr)
    {
        xprintf("%zNO SUCH FILE %s\n",set_output_color(red,white),program_parameters);

        while(1)
        {
            if((KeyInfo.scan_code == F4_KEY) || (KeyInfo.scan_code == ENTER))
                return;
            
        }   
    
    }
        
    else
    {
    
        if(xin_file->entry_type == XIN_DIRECTORY)
        {
            xprintf("%zYOU CANT RUN DIRECTORY\n",set_output_color(red,white));
            xprintf("%zuse F4 key to exit\n",set_output_color(red,white));
            while(KeyInfo.scan_code != F4_KEY);
            return;
        }
    
        //uint32_t address = (current_program->starting_sector * SECTOR_SIZE);

        uint8_t* dest = 0x10000;

        for(uint8_t* i = xin_file->starting_sector * SECTOR_SIZE; (uint32_t)i < xin_file->starting_sector * SECTOR_SIZE + SECTOR_SIZE; i++)
            *dest = *i;

        real_mode_enter(0x1000, 0x0); // <-- tmp.bin address in memory

        

    }

    return;
}