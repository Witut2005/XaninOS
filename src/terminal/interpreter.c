#pragma once

#include <terminal/interpreter.h>

void no_keyboard_input()
{
    asm("nop");
}

void scan(void)
{


    xscanf("%s %s",program_name, program_parameters);
    //xscanf("%s",program_name)
    erase_spaces(program_name);
    erase_spaces(program_parameters);

    //xprintf("%z%s\n",set_output_color(blue,white),program_name);

    //KEYBOARD_SIG_ON(no_keyboard_input);

    no_enter = true;
    keyboard_scan_code = 0x0;

    if(cmpstr(program_name,"touch"))
    {
        touch();
    }

    else if(cmpstr(program_name,"epilepsy"))
    {
        epilepsy();
    }    
    
    else if(cmpstr(program_name,"paint"))
    {
        xin_paint(program_parameters);
    }

    else if(cmpstr(program_name,"load"))
    {
    	keyboard_scan_code = 0x0;
    	load(strtoi(program_parameters, 16));
    	app_exited = true;
    }
    
    else if(cmpstr(program_name,"key-test"))
    {
        keyboard_test();
    }
     
    else if(cmpstr(program_name,"modify"))
    {
        keyboard_trun_on();
    	//while(1);
    	xprintf("%s\n", program_parameters);
    	keyboard_scan_code = 0x0;
    	//modify(program_parameters);
    	app_exited = true;
    }
	
	else if(cmpstr(program_name,"loadch"))
	{
		keyboard_scan_code = 0x0;
		loadch(strtoi(program_parameters, 16));
		app_exited = true;
	}	
	
	else if(cmpstr(program_name, "cdf"))
	{
		//cdf(program_parameters);
	}

    else if(cmpstr(program_name,"dev-info"))
    {
        get_device_info();
    }

    else if(cmpstr(program_name,"reboot")) 
    {
        reboot();
    }

    else if(cmpstr(program_name, "create"))
    {
       	//create(program_parameters); 
        xin_create_file(program_parameters);
    }
    
    else if(cmpstr(program_name, "mkdir"))
    {
		//mkdir(program_parameters);
        xin_create_directory(program_parameters);
    }

    else if(cmpstr(program_name,"shutdown"))
    {
        shutdown();
    }


    else if(cmpstr(program_name,"petris"))
    {
        clearScr();
        tetris();
    }

    else if(cmpstr(program_name,"md"))
    {
        md();
    }

    else if(cmpstr(program_name,"rd"))
    {
        //rd();
        xin_remove_entry(program_parameters);
    }

    else if(cmpstr(program_name,"cd"))
    {
        //cd();
        xin_change_directory(program_parameters);
    }

    else if(cmpstr(program_name,"pwd"))
    {
        xprintf("%s\n", xin_current_directory);
        while(keyboard_scan_code != ENTER);
        exit_process();
    }


    else if(cmpstr(program_name,"help") || cmpstr(program_name,"h"))
    {
        clearScr();
        y = 2;
        x = 0;

        help();   
    }

    /*

    else if(cmpstr(program_name,"pong"))
    {

        pongStart:

        pong_app_init();

        while(!app_exited)
        {
            pong_update_screen();
        }   
    }

    */

    else if(cmpstr(program_name,"execute"))
    {
        execute();
        KEYBOARD_SIG_ON(no_keyboard_input);
    }

    else if(cmpstr(program_name,"dir"))
    {
        //dir();
    }

    else if(cmpstr(program_name,"xin_info"))
    {
        xin_entry_info(program_parameters);
    }

    else if(cmpstr(program_name,"cls") || cmpstr(program_name,"clear"))
    {
        clearScr();
        app_exited = true;
    }


    else if(cmpstr(program_name,"ls") || cmpstr(program_name,"dir"))
        ls();
    
    else if(cmpstr(program_name,"note"))
    {
        //nano();
        keyboard_trun_on();
        xin_note(program_parameters);
        //KEYBOARD_SIG_ON(no_keyboard_input);
    }

    else if(cmpstr(program_name,"type"))
    {
        type();
        KEYBOARD_SIG_ON(no_keyboard_input);
    }

    else
    {

        keyboard_scan_code = NULL;
        xprintf("%zunknown command",set_output_color(red,white));
        
        while(keyboard_scan_code != ENTER);

        exit_process();
        
    }


}
