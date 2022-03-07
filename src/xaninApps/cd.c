
#include <fs/ustar.c>

void cd(void)
{


    no_enter = true;

    FileSystemEntryStruct* file_descriptor = find_fs_entry(program_parameters);



    if(file_descriptor != nullptr)
    {
        if(file_descriptor->entry_type != DIRECTORY)
            xprintf("%zYOU CAN CHANGE ONLY YOUR DIRECTORY\n",set_output_color(red,white));

        else
        {
            //set_current_directory(get_current_path(file_descriptor->entry_name));
            set_current_directory(file_descriptor->entry_name);
            xprintf("\nyour new directory: %s\n", file_descriptor->entry_name);
        }

    }

    else
        xprintf("%zNO SUCH DIRECTORY\n",set_output_color(red,white));


    while(keyboard_scan_code != ENTER);
    exit_process();


}