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

//    xprintf("%z%s\n",set_output_color(blue,white),program_name);



    KEYBOARD_SIG_ON(no_keyboard_input);

    
    if(cmpstr(program_name,"petris"))
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
        rd();
    }


    /*
    else if(cmpstr(keyboard_command,"hexview"))
    {
        hexview();
        KEYBOARD_SIG_ON(no_keyboard_input);
    }
    */



    else if(cmpstr(program_name,"cd"))
    {
        cd();
    }

    else if(cmpstr(program_name,"pwd"))
    {
        pwd();
        app_exited = true;
    }


    else if(cmpstr(program_name,"help") || cmpstr(program_name,"h"))
    {
        clearScr();
        y = 2;
        x = 0;

        help();   
    }


    else if(cmpstr(program_name,"pong"))
    {

        pongStart:

        initPong();

        while(!app_exited)
        {
            updatePong();
        }   
        
        

       
    }

    else if(cmpstr(program_name,"execute"))
    {
        execute();
        KEYBOARD_SIG_ON(no_keyboard_input);
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
        nano();
        KEYBOARD_SIG_ON(no_keyboard_input);
    }

    else if(cmpstr(program_name,"type"))
    {
        type();
        KEYBOARD_SIG_ON(no_keyboard_input);
    }

    else
    {
        xprintf("%zunknown command",set_output_color(red,white));
        while(1)
        {
            if(keyboard_input == 'q')
            {
                for(int i = 0; i < 50; i++)
                    keyboard_command[i] = '\0';

                index = 0x0; /* some problems with keyboard keyboard_input when index is no reseted */
                app_exited = true; no_enter = false;break;
            }
        }
    }
    
    KEYBOARD_SIG_OFF();

}
