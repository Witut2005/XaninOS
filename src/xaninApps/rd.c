
#include <fs/ustar.c>
#include <stddef.h>

void rd(void)
{

    no_enter = true;

    if(program_parameters[strlen(program_parameters)] != '/')
        program_parameters[strlen(program_parameters)] = '/';        

    FileSystemEntryStruct* file_descriptor = find_fs_entry(program_parameters);

    if(file_descriptor == nullptr)
    {
        xprintf("%zNO SUCH DIRECTORY\n",set_output_color(white,red));
    }

    else
    {

    file_descriptor->entry_size = nullptr;
    file_descriptor->entry_type = nullptr;
    file_descriptor->owner = nullptr;
    file_descriptor->group = nullptr;
  
    FileSystem.file_entries_number--;

    xprintf("%zdirectory deleted: %s", set_output_color(green,white),file_descriptor->entry_name);

    file_descriptor->entry_name = NULL;
    }

    while(1)
    {
        if(keyboard_input == 'q')
        {
            for(int i = 0; i < 50; i++)
                keyboard_command[i] = '\0';

            index = 0x0; /* some problems with keyboard keyboard_input when index is no reseted */
            app_exited = true; no_enter = false;break;
        }
    }


}