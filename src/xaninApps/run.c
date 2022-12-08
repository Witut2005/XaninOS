
#pragma once

#include <libc/stdiox.h>
#include <xin_fs/xin.h>
#include <libc/memory.h>
#include <terminal/interpreter.h>
#include <libc/string.h>


int run(char* file_name)
{
    //screen_clear();


    xin_entry* xin_file = fopen(file_name, "rw");
    
    if(xin_file == nullptr)
    {
        xprintf("%zNO SUCH FILE %s\n",set_output_color(red,white),program_parameters);

        while(1)
        {
            if((KeyInfo.scan_code == F4_KEY) || (KeyInfo.scan_code == ENTER))
            {
                return XANIN_ERROR;
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
            return XANIN_ERROR;
        }
    
        uint8_t* where_to_load = 0x10000;


        uint8_t* file_data = (uint8_t*)calloc(512);
        fread(xin_file, file_data, 512);

        for(uint8_t* i = file_data; i < (file_data + 512); i++, where_to_load++)
            *where_to_load = * i;
            

        void (*entry_point)(void) = 0x10000;

        asm(
            "call 0x10000"
            );

        return XANIN_OK;
        

    }

}
