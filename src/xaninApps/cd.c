
#include <fs/ustar.c>

void cd()
{


    no_enter = true;

    FileSystemEntryStruct* file_descriptor = find_fs_entry(program_parameters);

    if(file_descriptor != nullptr)
    {
        if(file_descriptor->entry_type != DIRECTORY)
            xprintf("%zYOU CAN CHANGE ONLY YOUR DIRECTORY\n",set_output_color(red,white));

        else
        {
            set_current_directory(program_parameters);
            xprintf("your new directory: %s\n", program_parameters);
        }

        goto finish;
    }

    xprintf("%zNO SUCH DIRECTORY\n",set_output_color(red,white));

    

    finish:

    
    while(1)
    {
        if(keyboard_input == 'q')
        {
            for(int i = 0; i < 50; i++)
                COMMAND[i] = '\0';

            index = 0x0; /* some problems with keyboard keyboard_input when index is no reseted */
            app_exited = true; no_enter = false;break;
        }
    }

}