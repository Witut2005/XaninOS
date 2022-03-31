
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
            {
                exit_process();
            }
        }   
    
    }
        
    else
    {
    
        if(xin_file->entry_type == XIN_DIRECTORY)
        {
            xprintf("%zYOU CANT RUN DIRECTORY\n",set_output_color(red,white));
            xprintf("%zuse F4 key to exit\n",set_output_color(red,white));
            while(KeyInfo.scan_code != F4_KEY);
            exit_process();
        }
    
        uint32_t address = (xin_file->starting_sector * SECTOR_SIZE);

        if(address > 0xFFFFF)
        {
            xprintf("%zENTRY POINT CANNOT BE HIGHER THAN 0xFFFFF\n", set_output_color(red,white));
            while(KeyInfo.scan_code != F4_KEY);
            exit_process();
        }


        uint16_t segment = (address >> 4) & 0xF000;
        uint16_t ip = address & 0xFFFF;

        real_mode_enter(segment, ip);

        

    }

    exit_process();
}