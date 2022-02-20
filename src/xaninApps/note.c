
#include <terminal/vty.h>
#include <fs/ustar.h>
#include <lib/stdiox.h>
#include <handlers/handlers.h>
#include <lib/signal.h>
#include <lib/stdiox.h>
#include <stddef.h>

#define F4_KEY 0x3e

void nano()
{

    KEYBOARD_SIG_OFF();

    no_enter = true;

    keyboard_scan_code = 0x0;

    FileSystemEntryStruct* file_descriptor = find_fs_entry(program_parameters);

    
    if(file_descriptor == nullptr)
    {
        xprintf("%zno such file or directory %s\n",set_output_color(red,white),program_parameters);
    
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
        
    else
    {

        clearScr();
        if(file_descriptor->entry_type == DIRECTORY)
        {
            xprintf("%zyou cant edit directory\n",set_output_color(red,white));
            xprintf("%zuse F4 key to exit\n",set_output_color(red,white));
            while(keyboard_scan_code != F4_KEY);
        }

        xprintf("%s\r",file_descriptor->entry_data_pointer);


        y = 0;
        while(keyboard_scan_code != F4_KEY);


        uint32_t file_data_counter = 0x0;
        for(char* i = (char*)VGA_TEXT_MEMORY; *i >= 0x20; i+=2, file_data_counter++)
            file_descriptor->entry_data_pointer[file_data_counter] = *i;

        file_descriptor->entry_size = file_data_counter;


    }

    
    for(int i = 0; i < sizeof(comBuf);i++)
        keyboard_command[i] = '\0';

    index = 0x0; /* some problems with keyboard keyboard_input when index is no reseted */
    app_exited = true; 
    no_enter = false;



}
