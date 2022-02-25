
#include <fs/ustar.h>
#include <lib/stdiox.h>

void touch(void)
{

    FileSystemEntryStruct* file_descriptor = &fs_entry[FileSystem.file_entries_number];

    xprintf("%d\n", FileSystem.file_entries_number);

    if(program_parameters[strlen(program_parameters)-1] == '/')
    {
        xprintf("%zINVALID FILE NAME ('/' AT THE END)\n",set_output_color(red,white));

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




    char* fs_entry_name = get_current_path(program_parameters);

    for(int i = 0; i < 50; i++)
        file_descriptor->entry_name[i] = fs_entry_name[i];

    file_descriptor->entry_size = 0x0;
    file_descriptor->entry_size_pointer = (char*)(file_descriptor) + 0x9c;
    file_descriptor->entry_type = FILE;
    file_descriptor->owner = 0x0;
    file_descriptor->group = 0x0;
    file_descriptor->entry_data_pointer = (char*)(file_descriptor) + USTAR_SECTOR_SIZE;
  
    FileSystem.file_entries_number++;

    for(int i = 0; i < 50; i++)
        keyboard_command[i] = '\0';

    index = 0x0; /* some problems with keyboard keyboard_input when index is no reseted */
    app_exited = true;
    
    xprintf("%zTOUCH BREAK\n",set_output_color(red,white));

    keyboard_scan_code = NULL;

}
