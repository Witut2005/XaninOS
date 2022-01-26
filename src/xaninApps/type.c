
#pragma once

#include <stddef.h>

#include <fs/ustar.h>
#include <headers/colors.h>
#include <handlers/handlers.c>

/* INTERRUPT */

void type()
{

    FileSystemEntryStruct* file_descriptor = find_fs_entry(program_parameters);


    if(file_descriptor == nullptr)
        xprintf("%zno such file or directory %s\n",set_output_color(red,white),program_parameters);

    else
    {
        if(file_descriptor->entry_type == FILE)
            xprintf("%s\n",file_descriptor->entry_data_pointer);

        else
            xprintf("%zYOU CAN ONLY DISPLAY FILE DATA\n",set_output_color(red,white));
    }


    keyboard_scan_code = NULL;

    while(1)
    {


        if(keyboard_scan_code == ENTER)
        {
            for(int i = 0; i < 50; i++)
                keyboard_command[i] = '\0';

            index = 0x0; /* some problems with keyboard keyboard_input when index is no reseted */
            app_exited = true; 
    
            return;
        }

    }   



}