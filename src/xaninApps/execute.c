

void execute()
{

    clearScr();

    xprintf("\rplease type name file which you want to execute\n");
    
    KEYBOARD_SIG_OFF();

    no_enter = true;

    keyboard_scan_code = 0x0;

    while(1)
    {

            xscanf("%s",file_name);
            

            for(int i = 0; i < FileSystem.file_entries_number; i++)
            {
                if(cmpstr(get_current_path(),fs_entry[i].entry_name))
                {
                    if(fs_entry[i].entry_type == DIRECTORY)
                    {
                        xprintf("%zYOU CANT EXECUTE DIRECTORY\n",set_output_color(red,white));
                        break;
                    }
                    void(*executable_program)(void) = (void(*)(void))fs_entry[i].entry_data_pointer;
                    asm("mov esi, ebx" :: "b"(fs_entry[i].entry_data_pointer));
                    executable_program(); /* calle must exectute RET instruction */
                    xprintf("%zprogram succefully returned. Press 'q' to exit\n",set_output_color(green,white));
                    goto finish;
                }
            }

            xprintf("%zNO SUCH FILE\n",set_output_color(red,white));
            goto finish;

        

    }

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