
#include <fs/ustar.c>

void execute()
{



    KEYBOARD_SIG_OFF();

    no_enter = true;

    keyboard_scan_code = 0x0;

    xprintf("\n");
    
    FileSystemEntryStruct* file_descriptor = find_fs_entry(program_parameters);

    if(file_descriptor != nullptr)
    {
        void(*executable_program)(void) = (void(*)(void))file_descriptor->entry_data_pointer;
        asm("mov esi, ebx" :: "b"(file_descriptor->entry_data_pointer));
        executable_program(); /* calle must execute RET instruction */
        xprintf("\n%zprogram succefully returned. Press 'q' to exit\n\n",set_output_color(green,white));
        goto finish;
    }

    xprintf("%zNO SUCH FILE: %s\n",set_output_color(red,white), file_descriptor->entry_name);

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