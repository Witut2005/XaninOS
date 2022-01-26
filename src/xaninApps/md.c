
#include <fs/ustar.c>
#include <stddef.h>

void md(void)
{

    no_enter = true;


    FileSystemEntryStruct* file_descriptor = &fs_entry[FileSystem.file_entries_number];

    xprintf("%d\n", FileSystem.file_entries_number);


    if(program_parameters[strlen(program_parameters)-1] != '/')
    {
        xprintf("%zINVALID DIRECTORY NAME (MISSING '/' AT THE END)\n",set_output_color(red,white));

        keyboard_scan_code = NULL;

        while(1)
        {

            if(keyboard_scan_code == ENTER)
            {
                for(int i = 0; i < 50; i++)
                    keyboard_command[i] = '\0';

                index = 0x0; /* some problems with keyboard keyboard_input when index is no reseted */
                app_exited = true; no_enter = false;return;
            }
        }   
    }


    
    get_program_parameter(file_descriptor->entry_name);
    


    char* fs_entry_name = get_current_path(program_parameters);


    for(int i = 0; i < 50; i++)
        file_descriptor->entry_name[i] = fs_entry_name[i];


    file_descriptor->entry_size = 0x0;
    file_descriptor->entry_size_pointer = file_descriptor->entry_size_pointer;
    file_descriptor->entry_type = DIRECTORY;
    file_descriptor->owner = 0x0;
    file_descriptor->group = 0x0;
  
    FileSystem.file_entries_number++;


    for(int i = 0; i < 50; i++)
        keyboard_command[i] = '\0';

    index = 0x0; /* some problems with keyboard keyboard_input when index is no reseted */
    app_exited = true; 
    no_enter = false;


}