
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
    
    if(xin_file == NULL)
    {
        xprintf("%zNO SUCH FILE %s\n",OUTPUT_COLOR_SET(red,white),program_parameters);

        while(inputg().scan_code == ENTER);
        return XANIN_ERROR;
            
    
    }
        
    else
    {
    
        if(xin_file->type == XIN_DIRECTORY)
        {
            xprintf("%zYOU CANT RUN DIRECTORY\n",OUTPUT_COLOR_SET(red,white));
            xprintf("%zuse F4 key to exit\n",OUTPUT_COLOR_SET(red,white));
            while(inputg().scan_code != ENTER);
            return XANIN_ERROR;
        }
    
        //uint32_t address = (current_program->first_sector * SECTOR_SIZE);

        uint8_t* dest = 0x10000;

        fread(xin_file, dest, xin_file->size);
        // for(uint8_t* i = xin_file->FileInfo->buffer; (uint32_t)i < (uint32_t)(xin_file->FileInfo->buffer) + SECTOR_SIZE; i++, dest++)
        //     *dest = *i;

        real_mode_enter_no_return(0x1000, 0x0); // <-- tmp.bin address in memory

        

    }

    return XANIN_OK;
}