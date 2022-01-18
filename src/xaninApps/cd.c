
#include <fs/ustar.h>

void cd()
{


    no_enter = true;


    xprintf("Entering...\n");

    for(int i = 0; i < FileSystem.file_entries_number; i++)
    {   
        if(cmpstr(program_parameters,fs_entry[i].entry_name))
        {
            if(fs_entry[i].entry_type != DIRECTORY)
                xprintf("%zYOU CAN CHANGE ONLY YOUR DIRECTORY\n",set_output_color(red,white));
            
            else
            {
                set_current_directory(program_parameters);
                xprintf("your new directory: %s\n", program_parameters);
            }

            goto finish;
        }

    }

    xprintf("%zNO SUCH DIRECTORY\n",set_output_color(red,white));

    

    finish:

    
    while(1)
    {
        if(keyboard_input == 'q')
        {
            for(int i = 0; i < sizeof(comBuf);i++)
                COMMAND[i] = '\0';

            index = 0x0; /* some problems with keyboard keyboard_input when index is no reseted */
            app_exited = true; no_enter = false;break;
        }
    }

}