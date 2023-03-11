
#pragma once

#include <libc/stdiox.h>
#include <xin_fs/xin.h>
#include <libc/memory.h>
#include <terminal/interpreter.h>
#include <libc/string.h>


int run(char* file_name)
{
    //screen_clear();


    XinEntry* xin_file = fopen(file_name, "rw");
    
    if(xin_file == NULL)
    {
        xprintf("%zNO SUCH FILE %s\n",set_output_color(red,white),program_parameters);

        while(inputg().scan_code == ENTER);
        return XANIN_ERROR;
    }
        
    else
    {
    
        if(xin_file->type == XIN_DIRECTORY)
        {
            xprintf("%zYOU CANT RUN DIRECTORY\n",set_output_color(red,white));
            xprintf("%zuse F4 key to exit\n",set_output_color(red,white));
            while(inputg().scan_code != ENTER);
            return XANIN_ERROR;
        }
    
        uint8_t* where_to_load = 0x10000;

        fread(xin_file, where_to_load, xin_file->size);

        return XANIN_OK;
        

    }

}
