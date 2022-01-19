
#pragma once

#include <fs/ustar.h>
#include <headers/colors.h>
#include <handlers/handlers.c>

/* INTERRUPT */

void type()
{

    xprintf("\rplease type name file which you want to use\n");
    
    KEYBOARD_SIG_OFF();

    no_enter = true;

    keyboard_scan_code = 0x0;

    while(1)
    {

        if(!index && keyboard_scan_code == ENTER)
        {
            
            erase_spaces(keyboard_command);

            for(int i = 0; i < FileSystem.file_entries_number; i++)
            {
                if(cmpstr(keyboard_command,fs_entry[i].entry_name))
                {
                    if(fs_entry[i].entry_type == DIRECTORY)
                    {
                        xprintf("%zYOU CANT DISPLAY DIRECTORY DATA\n",set_output_color(red,white));
                        goto end;
                    }
                    xprintf("%s\n",fs_entry[i].entry_data_pointer);
                    goto end;
                }
            }
            
            goto error;

        }

    }

    error:
    xprintf("\r%z%s\n",set_output_color(red,white),keyboard_command);
    xprintf("\r%z%s\n",set_output_color(red,white),"NO SUCH FILE OR DIRECTORY");


    end:

    

    xprintf("\n\npress 'q' key to continue...");
    
    
    

    while(1)
    {
        if(keyboard_input == 'q')
        {
            for(int i = 0; i < sizeof(comBuf);i++)
                keyboard_command[i] = '\0';

            index = 0x0; /* some problems with keyboard keyboard_input when index is no reseted */
            app_exited = true; no_enter = false;break;
        }
    }

}