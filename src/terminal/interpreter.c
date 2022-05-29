#pragma once

#include <terminal/interpreter.h>

char scanf_str[40] = "%s %s %s %s %s";

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

    else if(strcmp(program_name,"timer_test"))
    {
        timer_test();
    }

    else if(strcmp(program_name,"epilepsy"))
    {
        epilepsy();
    }    

    else if(strcmp(program_name,"elft"))
    {
        elfreader(program_parameters);
    }

    else if(strcmp(program_name, "mouse"))
    {
        xprintf("11111111111111");
        mouse_test(program_parameters);
    }
    
    else if(strcmp(program_name, "hexeditor"))
    {
        hexeditor(program_parameters);
    }

    else if(strcmp(program_name,"run16"))
    {
        run16(program_parameters);
    }
    
    else if(strcmp(program_name,"run"))
    {
        run(program_parameters);
    }

    else if(strcmp(program_name, "test"))
    {
        cpp_test();
    }

    else if(strcmp(program_name,"stack_test"))
    {
        stack_frame_test(0x10, 0x20);
    }

    else if(strcmp(program_name,"cpu_info"))
    {
        cpu_info(program_parameters);
    }

    else if(strcmp(program_name,"calc"))
    {
        calc(program_parameters);
    }

    else if(strcmp(program_name,"paint"))
    {
        xin_paint(program_parameters);
    }

    else if(strcmp(program_name,"reg_dump"))
    {
        screen_clear();
        reg_dump();
        while(getscan() != ENTER);
    }

    else if(strcmp(program_name,"load"))
    {
    	load(program_parameters);
    }

    else if(strcmp(program_name,"install"))
    {
        install();
    }

    else if(strcmp(program_name,"disk_load"))
    {
        disk_load(strtoi(program_parameters,16), strtoi(program_parameters1, 16), strtoi(program_parameters2, 16));
    }

    else if(strcmp(program_name,"disk_write"))
    {
        disk_write_data(strtoi(program_parameters,16), strtoi(program_parameters1, 16), strtoi(program_parameters2, 16));
    }

    else if(strcmp(program_name,"execute_addr"))
    {
        execute_addr(strtoi(program_parameters, 16));
        app_exited = true;
    }
    
    else if(strcmp(program_name,"key-test"))
    {
        keyboard_test();
    }
     
    
    else if(strcmp(program_name, "pong"))
    {
        pong();
        
    }

    else if(strcmp(program_name,"loadch"))
    {
        loadch(program_parameters);
    }	
	

    else if(strcmp(program_name,"dev-info"))
    {
        get_device_info();
    }

    else if(strcmp(program_name,"reboot")) 
    {
        reboot();
    }

    else if(strcmp(program_name, "create"))
    {
        create_file(program_parameters);
    }
    
    else if(strcmp(program_name, "mkdir"))
    {
        xin_create_directory(program_parameters);
    }

    else if(strcmp(program_name,"shutdown"))
    {
        shutdown();
    }


    else if(strcmp(program_name,"rm"))
    {
        xin_remove_entry(program_parameters);
    }

    else if(strcmp(program_name,"rd"))
    {
        remove_directory(program_parameters);
    }

    else if(strcmp(program_name,"cd"))
    {
        xin_change_directory(program_parameters);
    }

    else if(strcmp(program_name,"pwd"))
    {
        xprintf("%s\n", xin_current_directory);
        while(KeyInfo.scan_code != ENTER);
    }

    else if(strcmp(program_name,"help") || strcmp(program_name,"h"))
    {
        help(program_parameters);   
    }

    else if(strcmp(program_name,"xin_info"))
    {
        xin_entry_info(program_parameters);
    }

    else if(strcmp(program_name,"cls") || strcmp(program_name,"clear"))
    {
        screen_clear();
    }


    else if(strcmp(program_name,"ls") || strcmp(program_name,"dir"))
    {
        list_files();
    }
    
    else if(strcmp(program_name,"note"))
    {
        xin_note(program_parameters);
    }

    else if(strcmp(program_name, "logo"))
    {
        logo_color_change(program_parameters);
    }

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
