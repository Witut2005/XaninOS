
#include <terminal/vty.h>
#include <fs/ustar.h>
#include <lib/stdiox.h>
#include <handlers/handlers.h>
#include <lib/signal.h>
#include <lib/stdiox.h>

#define F4_KEY 0x3e

void nano()
{

    KEYBOARD_SIG_OFF();

    no_enter = true;

    xprintf("\rwhich file do you want to edit:\n");

    keyboard_scan_code = 0x0;
    char* file_descriptor;

    while(1)
    {

        if(!index && keyboard_scan_code == ENTER)
        {
        
            erase_spaces(COMMAND);

            for(int i = 0; i < FileSystem.file_entries_number; i++)
            {
                if(cmpstr(COMMAND,fs_entry[i].entry_name))
                {
                    clearScr();
                    if(fs_entry[i].entry_type == DIRECTORY)
                    {
                        xprintf("%zyou cant edit directory\n",set_output_color(red,white));
                        xprintf("%zuse F4 key to exit\n",set_output_color(red,white));
                        while(keyboard_scan_code != F4_KEY);
                        goto end;
                    }

                    xprintf("%s\r",fs_entry[i].entry_data_pointer);
                    file_descriptor = fs_entry[i].entry_data_pointer;
                    goto edit;
                }
            }
            
            goto error;

        }

    }

    error:

    xprintf("%zno such file or directory\n",set_output_color(red,white));

    edit:
    y = 0;
    while(keyboard_scan_code != F4_KEY);


    uint32_t file_data_counter = 0x0;
    for(char* i = (char*)VGA_TEXT_MEMORY; i < (char*)(VGA_TEXT_MEMORY + 80 * 5 * 2); i+=2, file_data_counter++)
        file_descriptor[file_data_counter] = *i;

    
    end:
    
    for(int i = 0; i < sizeof(comBuf);i++)
        COMMAND[i] = '\0';

    index = 0x0; /* some problems with keyboard keyboard_input when index is no reseted */
    app_exited = true; 
    no_enter = false;


    return;

}