
#include <fs/ustar.c>


void touch(void)
{

    FileSystemEntryStruct* file_descriptor = &fs_entry[FileSystem.file_entries_number];


    file_descriptor->entry_name = get_current_path(program_parameters);
    file_descriptor->entry_size = 0x0;
    file_descriptor->entry_size_pointer = (char*)(file_descriptor) + 0x9c;
    file_descriptor->entry_type = FILE;
    file_descriptor->owner = 0x0;
    file_descriptor->group = 0x0;
  
    FileSystem.file_entries_number++;

    for(int i = 0; i < 50; i++)
        keyboard_command[i] = '\0';

    index = 0x0; /* some problems with keyboard keyboard_input when index is no reseted */
    app_exited = true;
    


}