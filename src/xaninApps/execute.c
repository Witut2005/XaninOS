

void execute()
{

    xprintf("\rplease type name file which you want to execute\n");
    
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
                    if(fs_entry[i].entry_type == DIRECTORY)
                    {
                        xprintf("%zYOU CANT EXECUTE DIRECTORY\n",set_output_color(red,white));
                        break;
                    }
                    void(*executable_program)(void) = (void(*)(void))fs_entry[i].entry_data_pointer;
                    asm("mov esi, ebx" :: "b"(fs_entry[i].entry_data_pointer));
                    executable_program(); /* calle must exectute RET instruction */
                    goto finish;
                }
            }

            xprintf("%zNO SUCH FILE\n",set_output_color(red,white));
            break;

        }

    }

    finish:

    xprintf("%zprogram succefully returned. Press 'q' to exit\n",set_output_color(green,white));

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