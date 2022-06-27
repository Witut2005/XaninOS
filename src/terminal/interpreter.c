#pragma once

#include <terminal/interpreter.h>

char scanf_str[40] = "%s %s %s %s %s";

#define XANIN_ADD_APP_ENTRY0(app_name, exec_name) else if(strcmp(program_name, app_name)) {exec_name();}
#define XANIN_ADD_APP_ENTRY1(app_name, exec_name, arg0) else if(strcmp(program_name, app_name)) {exec_name(arg0);}
#define XANIN_ADD_APP_ENTRY2(app_name, exec_name, arg0, arg1) else if(strcmp(program_name, app_name)) {exec_name(arg0, arg1);}
#define XANIN_ADD_APP_ENTRY3(app_name, exec_name, arg0, arg1, arg2) else if(strcmp(program_name, app_name)) {exec_name(arg0, arg1, arg2);}


void scan(void)
{
    for(int i = 0; i < 40; i++)
        program_name[i] = '\0';
    
    for(int i = 0; i < 40; i++)
        program_parameters[i] = '\0';

    xscanf(scanf_str,program_name, program_parameters, program_parameters1, program_parameters2, program_parameters3);



    erase_spaces(program_name);
    erase_spaces(program_parameters);
    
    for(int i = 0; i < sizeof(program_name); i++)
        last_used_commands[i] = program_name[i];


    for(int i = 0; i < sizeof(program_parameters); i++)
        last_used_parameters[i] = program_parameters[i]; 

    character_blocked = '\0';
    keyboard_handle = nullptr;

    KeyInfo.character = 0x0;
    KeyInfo.scan_code = 0x0;


    if(strcmp(program_name,"zsk"))
    {
        zsk(program_parameters);
    }

    XANIN_ADD_APP_ENTRY0("timer_test", timer_test)
    XANIN_ADD_APP_ENTRY1("cat", cat, program_parameters)
    XANIN_ADD_APP_ENTRY0("epilepsy", epilepsy)
    XANIN_ADD_APP_ENTRY1("elft", elfreader, program_parameters)
    XANIN_ADD_APP_ENTRY1("mouse", mouse_test, program_parameters)
    XANIN_ADD_APP_ENTRY1("hexeditor", hexeditor, program_parameters)
    XANIN_ADD_APP_ENTRY1("run16", run16, program_parameters)
    XANIN_ADD_APP_ENTRY1("run", run, program_parameters)
    XANIN_ADD_APP_ENTRY0("test", cpp_test)
    XANIN_ADD_APP_ENTRY2("stack_test", stack_frame_test, program_parameters, program_parameters1)
    XANIN_ADD_APP_ENTRY1("cpu_info", cpu_info, program_parameters)
    XANIN_ADD_APP_ENTRY1("calc", calc, program_parameters)
    XANIN_ADD_APP_ENTRY1("paint", xin_paint, program_parameters)
    XANIN_ADD_APP_ENTRY0("reg_dump", reg_dump)
    XANIN_ADD_APP_ENTRY1("load", load, program_parameters)
    XANIN_ADD_APP_ENTRY0("install", install)
    XANIN_ADD_APP_ENTRY3("disk_load", disk_load, strtoi(program_parameters,16), strtoi(program_parameters1, 16), strtoi(program_parameters2, 16)) 
    XANIN_ADD_APP_ENTRY3("disk_write", disk_write_data, strtoi(program_parameters,16), strtoi(program_parameters1, 16), strtoi(program_parameters2, 16)) 


    XANIN_ADD_APP_ENTRY1("execute_addr", execute_addr, strtoi(program_parameters, 16))
    XANIN_ADD_APP_ENTRY0("key-test", keyboard_test)
    XANIN_ADD_APP_ENTRY0("pong", pong)
    XANIN_ADD_APP_ENTRY1("loadch", loadch, program_parameters)
    XANIN_ADD_APP_ENTRY0("dev-info", get_device_info)
    XANIN_ADD_APP_ENTRY0("reboot", reboot)
    XANIN_ADD_APP_ENTRY1("create", create_file, program_parameters)
    XANIN_ADD_APP_ENTRY1("mkdir", xin_create_directory, program_parameters)
    XANIN_ADD_APP_ENTRY0("shutdown", shutdown)
    XANIN_ADD_APP_ENTRY1("rm", xin_remove_entry, program_parameters)
    XANIN_ADD_APP_ENTRY1("rd", remove_directory, program_parameters)
    XANIN_ADD_APP_ENTRY1("cd", xin_change_directory, program_parameters)

    else if(strcmp(program_name,"pwd"))
    {
        xprintf("%s\n", xin_current_directory);
        while(KeyInfo.scan_code != ENTER);
    }

    XANIN_ADD_APP_ENTRY1("help", help, program_parameters)
    XANIN_ADD_APP_ENTRY1("h", help, program_parameters)

    XANIN_ADD_APP_ENTRY1("xin_info", xin_entry_info, program_parameters)
    XANIN_ADD_APP_ENTRY0("cls", screen_clear)
    XANIN_ADD_APP_ENTRY0("clear", screen_clear)
    XANIN_ADD_APP_ENTRY1("ls", list_files, program_parameters)
    XANIN_ADD_APP_ENTRY1("dir", list_files, program_parameters)
    XANIN_ADD_APP_ENTRY1("note", xin_note, program_parameters)
    XANIN_ADD_APP_ENTRY1("logo", logo_color_change, program_parameters)

    else if(strcmp(program_name, "\0"))
    {
        logo_front_color++;
        if(logo_front_color == 16)
            logo_front_color = 1;

        app_exited = true;
    }

    else
    {
        xprintf("%zUnknown command",set_output_color(red,white));
        logo_front_color = red;
    }


    keyboard_handle = nullptr;
    app_exited = true;
    KeyInfo.character = 0x0;
    KeyInfo.scan_code = 0x0;

}
