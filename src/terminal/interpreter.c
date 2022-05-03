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


    /*
    xprintf("\n%s\n", program_name);
    xprintf("%s\n", program_parameters);
    xprintf("%s\n", program_parameters1);
    xprintf("%s\n", program_parameters2);
    xprintf("%s\n", program_parameters3);
    */

    KeyInfo.character = 0x0;
    KeyInfo.scan_code = 0x0;

    character_blocked = '\0';

    keyboard_handle = nullptr;

    
    /*
    keyboard_buffer_refresh(&Screen.cursor[8][1]);

    xprintf("command:\n");

    for(int i = 0; i < 40; i++)
        putchar(keyboard_command[i]);

    while(KeyInfo.scan_code != ENTER);
    */

    if(strcmp(program_name,"touch"))
    {

        //xprintf("1");
        //while(KeyInfo.scan_code != ENTER);
        //touch();
    }

    else if(strcmp(program_name, "assembler"))
    {
        assembler(program_parameters);
    }

    else if(strcmp(program_name,"xagame_test"))
    {
        xagame_test();
    }

    else if(strcmp(program_name,"zsk"))
    {
        zsk(program_parameters);
    }

    else if(strcmp(program_name,"timer_test"))
    {
        timer_test();
    }

    else if(strcmp(program_name,"epilepsy"))
    {
        //xprintf("2");
        //while(KeyInfo.scan_code != ENTER);

        epilepsy();
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
        //xprintf("3");
        //while(KeyInfo.scan_code != ENTER);
        
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
        //xprintf("4");
        //while(KeyInfo.scan_code != ENTER);
        
    	KeyInfo.scan_code = 0x0;
    	load(program_parameters);
    	app_exited = true;
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
        //xprintf("5");
        //while(KeyInfo.scan_code != ENTER);
        
        keyboard_test();
    }
     
    /*
    else if(strcmp(program_name,"shooter"))
    {
        shooter();
    }
    */


	
    else if(strcmp(program_name, "pong"))
    {
        pong();
        
    }

	else if(strcmp(program_name,"loadch"))
	{
        //xprintf("7");
        //while(KeyInfo.scan_code != ENTER);
        
		KeyInfo.scan_code = 0x0;
		loadch(program_parameters);
		app_exited = true;
	}	
	
	else if(strcmp(program_name, "cdf"))
	{
        //xprintf("8");
        //while(KeyInfo.scan_code != ENTER);
        
		//cdf(program_parameters);
	}

    else if(strcmp(program_name,"dev-info"))
    {
        //xprintf("9");
        //while(KeyInfo.scan_code != ENTER);
        
        get_device_info();
    }

    else if(strcmp(program_name,"reboot")) 
    {
        //xprintf("a");
        //while(KeyInfo.scan_code != ENTER);
        
        reboot();
    }

    else if(strcmp(program_name, "create"))
    {
        //xprintf("b");   
        //while(KeyInfo.scan_code != ENTER);
        
       	//create(program_parameters); 
        //xprintf("%zHUJ", set_output_color(red,white));
        //while(1);
        create_file(program_parameters);
    }
    
    else if(strcmp(program_name, "mkdir"))
    {
        //xprintf("c");
        //while(KeyInfo.scan_code != ENTER);
        
		//mkdir(program_parameters);
        xin_create_directory(program_parameters);
    }

    else if(strcmp(program_name,"shutdown"))
    {
        //xprintf("d");
        //while(KeyInfo.scan_code != ENTER);
        
        shutdown();
    }


    else if(strcmp(program_name,"petris"))
    {
        /*
        xprintf("e");
        //while(KeyInfo.scan_code != ENTER);
        
        screen_clear();
        tetris();
        */
    }

    else if(strcmp(program_name,"md"))
    {
        //xprintf("f");
        //while(KeyInfo.scan_code != ENTER);
        
        //md();
    }

    else if(strcmp(program_name,"rd"))
    {
        //xprintf("g");
        //while(KeyInfo.scan_code != ENTER);
        
        //rd();
        xin_remove_entry(program_parameters);
        app_exited = true;
    }

    else if(strcmp(program_name,"cd"))
    {
        //xprintf("h");
        //while(KeyInfo.scan_code != ENTER);
        
        //cd();
        xin_change_directory(program_parameters);
    }

    else if(strcmp(program_name,"pwd"))
    {
        //xprintf("i");
        //while(KeyInfo.scan_code != ENTER);
        
        xprintf("%s\n", xin_current_directory);
        while(KeyInfo.scan_code != ENTER);
        exit_process();
    }

    else if(strcmp(program_name,"help") || strcmp(program_name,"h"))
    {
        help(program_parameters);   
    }


    else if(strcmp(program_name,"execute"))
    {
        //xprintf("o");
        //while(KeyInfo.scan_code != ENTER);
        
        //execute();
        //KEYBOARD_SIG_ON(no_keyboard_input);
    }

    else if(strcmp(program_name,"dir"))
    {
        //xprintf("p");
        //while(KeyInfo.scan_code != ENTER);
        
        //dir();
    }

    else if(strcmp(program_name,"xin_info"))
    {
        //xprintf("r");
        //while(KeyInfo.scan_code != ENTER);
        
        xin_entry_info(program_parameters);
    }

    else if(strcmp(program_name,"cls") || strcmp(program_name,"clear"))
    {
        //xprintf("s");
        //while(KeyInfo.scan_code != ENTER);
        
        screen_clear();
        app_exited = true;
    }


    else if(strcmp(program_name,"ls") || strcmp(program_name,"dir"))
    {
        list_files();
    }
    
    else if(strcmp(program_name,"note"))
    {
        //xprintf("u");
        //while(KeyInfo.scan_code != ENTER);
        
        xprintf("%zEXECUTING NOTE\n", set_output_color(green,white));
        xin_note(program_parameters);
    }

    else if(strcmp(program_name,"type"))
    {
        //xprintf("w");
        //while(KeyInfo.scan_code != ENTER);
        
        //type();
        //KEYBOARD_SIG_ON(no_keyboard_input);
    }

    else if(strcmp(program_name, "logo"))
    {
        //xprintf("k");
        //while(KeyInfo.scan_code != ENTER);
        
        logo_color_change(program_parameters);
        app_exited = true;
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
        //xprintf("z");
        //while(KeyInfo.scan_code != ENTER);
        

        xprintf("%zunknown command",set_output_color(red,white));
        logo_front_color = red;

        KeyInfo.scan_code = 0x0;        
        keyboard_input = 0x0;

        KeyInfo.scan_code = 0x0;


        exit_process();
        
    }


    keyboard_handle = nullptr;
    app_exited = true;
    KeyInfo.character = 0x0;
    KeyInfo.scan_code = 0x0;

}
