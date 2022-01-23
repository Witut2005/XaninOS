
#include <fs/ustar.c>


void md(void)
{

    no_enter = true;


    FileSystemEntryStruct* file_descriptor = &fs_entry[FileSystem.file_entries_number];


    //if(program_parameters[strlen(program_parameters)-1] != '/')
    //    program_parameters[strlen(program_parameters)] = '/';        


    file_descriptor->entry_name = get_current_path(program_parameters);
    file_descriptor->entry_size = 0x0;
    file_descriptor->entry_type = DIRECTORY;
    file_descriptor->owner = 0x0;
    file_descriptor->group = 0x0;
  
    FileSystem.file_entries_number++;

    xprintf("%zyour directory name: %s", set_output_color(green,white),file_descriptor->entry_name);

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