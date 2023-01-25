
#pragma once

#include <libc/stdiox.h>
#include <xin_fs/xin.h>
#include <libc/memory.h>
#include <terminal/interpreter.h>
#include <libc/string.h>


int run16(char* file_name)
{
    screen_clear();

    XinEntry* xin_file = fopen(file_name, "rw");
    
    if(xin_file == nullptr)
    {
        xprintf("%zNO SUCH FILE %s\n",set_output_color(red,white),program_parameters);

        while(1)
        {
            if((KeyInfo.scan_code == F4_KEY) || (KeyInfo.scan_code == ENTER))
                return XANIN_ERROR;
            
        }   
    
    }
        
    else
    {
    
        if(xin_file->entry_type == XIN_DIRECTORY)
        {
            xprintf("%zYOU CANT RUN DIRECTORY\n",set_output_color(red,white));
            xprintf("%zuse F4 key to exit\n",set_output_color(red,white));
            while(KeyInfo.scan_code != F4_KEY);
            return XANIN_ERROR;
        }
    
        //uint32_t address = (current_program->starting_sector * SECTOR_SIZE);

        uint8_t* dest = 0x10000;

        for(uint8_t* i = xin_file->FileInfo->base_address_memory; (uint32_t)i < (uint32_t)(xin_file->FileInfo->base_address_memory) + SECTOR_SIZE; i++, dest++)
            *dest = *i;

        real_mode_enter_no_return(0x1000, 0x0); // <-- tmp.bin address in memory

        

    }

    return XANIN_OK;
}