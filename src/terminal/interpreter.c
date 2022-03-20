#pragma once

#include <terminal/interpreter.h>


void scan(void)
{
    for(int i = 0; i < 40; i++)
        program_name[i] = '\0';
    
    for(int i = 0; i < 40; i++)
        program_parameters[i] = '\0';

    xscanf("%s %s",program_name, program_parameters);

    erase_spaces(program_name);
    erase_spaces(program_parameters);

    xprintf("\n%s\n", program_name);
    xprintf("%s\n", program_parameters);

    KeyInfo.character = 0x0;
    KeyInfo.scan_code = 0x0;

    character_blocked = '\0';

    keyboard_handle = nullptr;

    //keyboard_buffer_refresh(&Screen.cursor[8][1]);

    xprintf("command:\n");

    for(int i = 0; i < 40; i++)
        putchar(keyboard_command[i]);

    //while(KeyInfo.scan_code != ENTER);

    if(cmpstr(program_name,"touch"))
    {

        //xprintf("1");
        //while(KeyInfo.scan_code != ENTER);
        //touch();
    }

    else if(cmpstr(program_name,"zsk"))
    {
        zsk(program_parameters);
    }

    else if(cmpstr(program_name,"timer_test"))
    {
        timer_test();
    }

    else if(cmpstr(program_name,"epilepsy"))
    {
        //xprintf("2");
        //while(KeyInfo.scan_code != ENTER);

        epilepsy();
    }    
    
    else if(cmpstr(program_name,"cpu_info"))
    {
        cpu_info();
    }

    else if(cmpstr(program_name,"calc"))
    {
        //calc();
    }

    else if(cmpstr(program_name,"paint"))
    {
        //xprintf("3");
        //while(KeyInfo.scan_code != ENTER);
        
        xin_paint(program_parameters);
    }

    else if(cmpstr(program_name,"reg_dump"))
    {
        screen_clear();
        reg_dump();
        while(getscan() != ENTER);
    }

    else if(cmpstr(program_name,"load"))
    {
        //xprintf("4");
        //while(KeyInfo.scan_code != ENTER);
        
    	keyboard_scan_code = 0x0;
    	load(strtoi(program_parameters, 16));
    	app_exited = true;
    }
    
    else if(cmpstr(program_name,"key-test"))
    {
        //xprintf("5");
        //while(KeyInfo.scan_code != ENTER);
        
        keyboard_test();
    }
     
    else if(cmpstr(program_name,"modify"))
    {
        //xprintf("6");
        //while(KeyInfo.scan_code != ENTER);
        
    	xprintf("%s\n", program_parameters);
    	keyboard_scan_code = 0x0;
    	app_exited = true;
    }
	
    else if(cmpstr(program_name, "pong"))
    {
        pong_init();
        
    }

	else if(cmpstr(program_name,"loadch"))
	{
        //xprintf("7");
        //while(KeyInfo.scan_code != ENTER);
        
		keyboard_scan_code = 0x0;
		loadch(strtoi(program_parameters, 16));
		app_exited = true;
	}	
	
	else if(cmpstr(program_name, "cdf"))
	{
        //xprintf("8");
        //while(KeyInfo.scan_code != ENTER);
        
		//cdf(program_parameters);
	}

    else if(cmpstr(program_name,"dev-info"))
    {
        //xprintf("9");
        //while(KeyInfo.scan_code != ENTER);
        
        get_device_info();
    }

    else if(cmpstr(program_name,"reboot")) 
    {
        //xprintf("a");
        //while(KeyInfo.scan_code != ENTER);
        
        reboot();
    }

    else if(cmpstr(program_name, "create"))
    {
        //xprintf("b");   
        //while(KeyInfo.scan_code != ENTER);
        
       	//create(program_parameters); 
        //xprintf("%zHUJ", set_output_color(red,white));
        //while(1);
        xin_create_file(program_parameters);
    }
    
    else if(cmpstr(program_name, "mkdir"))
    {
        //xprintf("c");
        //while(KeyInfo.scan_code != ENTER);
        
		//mkdir(program_parameters);
        xin_create_directory(program_parameters);
    }

    else if(cmpstr(program_name,"shutdown"))
    {
        //xprintf("d");
        //while(KeyInfo.scan_code != ENTER);
        
        shutdown();
    }


    else if(cmpstr(program_name,"petris"))
    {
        /*
        xprintf("e");
        //while(KeyInfo.scan_code != ENTER);
        
        screen_clear();
        tetris();
        */
    }

    else if(cmpstr(program_name,"md"))
    {
        //xprintf("f");
        //while(KeyInfo.scan_code != ENTER);
        
        //md();
    }

    else if(cmpstr(program_name,"rd"))
    {
        //xprintf("g");
        //while(KeyInfo.scan_code != ENTER);
        
        //rd();
        xin_remove_entry(program_parameters);
        app_exited = true;
    }

    else if(cmpstr(program_name,"cd"))
    {
        //xprintf("h");
        //while(KeyInfo.scan_code != ENTER);
        
        //cd();
        xin_change_directory(program_parameters);
    }

    else if(cmpstr(program_name,"pwd"))
    {
        //xprintf("i");
        //while(KeyInfo.scan_code != ENTER);
        
        xprintf("%s\n", xin_current_directory);
        while(keyboard_scan_code != ENTER);
        exit_process();
    }

    else if(cmpstr(program_name,"help") || cmpstr(program_name,"h"))
    {
        //xprintf("j");
        //while(KeyInfo.scan_code != ENTER);
        
        screen_clear();
        y = 2;
        x = 0;

        help();   
    }


    else if(cmpstr(program_name,"execute"))
    {
        //xprintf("o");
        //while(KeyInfo.scan_code != ENTER);
        
        //execute();
        //KEYBOARD_SIG_ON(no_keyboard_input);
    }

    else if(cmpstr(program_name,"dir"))
    {
        //xprintf("p");
        //while(KeyInfo.scan_code != ENTER);
        
        //dir();
    }

    else if(cmpstr(program_name,"xin_info"))
    {
        //xprintf("r");
        //while(KeyInfo.scan_code != ENTER);
        
        xin_entry_info(program_parameters);
    }

    else if(cmpstr(program_name,"cls") || cmpstr(program_name,"clear"))
    {
        //xprintf("s");
        //while(KeyInfo.scan_code != ENTER);
        
        screen_clear();
        app_exited = true;
    }


    else if(cmpstr(program_name,"ls") || cmpstr(program_name,"dir"))
    {
        //xprintf("t");
        //while(KeyInfo.scan_code != ENTER);

        //ls();
    }
    
    else if(cmpstr(program_name,"note"))
    {
        //xprintf("u");
        //while(KeyInfo.scan_code != ENTER);
        
        xprintf("%zEXECUTING NOTE\n", set_output_color(green,white));
        xin_note(program_parameters);
    }

    else if(cmpstr(program_name,"type"))
    {
        //xprintf("w");
        //while(KeyInfo.scan_code != ENTER);
        
        //type();
        //KEYBOARD_SIG_ON(no_keyboard_input);
    }

    else if(cmpstr(program_name, "logo"))
    {
        //xprintf("k");
        //while(KeyInfo.scan_code != ENTER);
        
        logo_color_change(program_parameters);
        app_exited = true;
    }

    else if(cmpstr(program_name, "\0"))
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

        keyboard_scan_code = 0x0;        
        keyboard_input = 0x0;

        KeyInfo.scan_code = 0x0;


        exit_process();
        
    }


    keyboard_handle = nullptr;
    app_exited = true;
    KeyInfo.character = 0x0;
    KeyInfo.scan_code = 0x0;

}
