
/*--------------------------------------------------------------------

#include <fs/ustar.c>
#include <stddef.h>

void rd(void)
{

    no_enter = true;

    FileSystemEntryStruct* file_descriptor = find_fs_entry(program_parameters);

    if(file_descriptor == nullptr || file_descriptor->entry_type != DIRECTORY)
    {
        xprintf("%zNO SUCH DIRECTORY\n",set_output_color(red,white));

        keyboard_scan_code = NULL;

        while(1)
        {
            if(keyboard_scan_code == ENTER)
            {
                for(int i = 0; i < 50; i++)
                    keyboard_command[i] = '\0';

                index = 0x0;  some problems with keyboard keyboard_input when index is no reseted 
                app_exited = true; 
                no_enter = false;
                return;
            }
        }
    }

    else
    {

    file_descriptor->entry_size = nullptr;
    file_descriptor->entry_type = nullptr;
    file_descriptor->owner = nullptr;
    file_descriptor->group = nullptr;
  
    FileSystem.file_entries_number--;

    xprintf("%zdirectory deleted: %s", set_output_color(green,white),file_descriptor->entry_name);


    //file_descriptor->entry_name = NULL;
    
    }
}


*/