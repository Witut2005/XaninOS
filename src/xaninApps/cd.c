
#include <fs/ustar.h>

void cd()
{

    xprintf("\rplease type path\n");
    
    KEYBOARD_SIG_OFF();

    no_enter = true;
    keyboard_scan_code = 0x0;

    while(1)
    {

        if(!index && keyboard_scan_code == ENTER)
        {
            
            erase_spaces(COMMAND);

            for(int i = 0; i < FileSystem.file_entries_number; i++)
            {
                if(cmpstr(COMMAND,fs_entry[i].entry_name))
                {
                    if(fs_entry[i].entry_type != DIRECTORY)
                        xprintf("%zYOU CAN CHANGE ONLY YOUR DIRECTORY\n",set_output_color(red,white));
                    else
                    {
                        set_current_directory(COMMAND);
                        xprintf("%zyour new directory: %s\n",set_output_color(green,white),current_directory);
                    }

                    goto finish;
                }
            }

            xprintf("%zNO SUCH DIRECTORY\n",set_output_color(red,white));
            goto finish;

        }

    }

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