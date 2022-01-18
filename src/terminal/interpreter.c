#pragma once

#include <terminal/interpreter.h>

void no_keyboard_input()
{
    asm("nop");
}

void scan(void)
{

    erase_spaces(program_name);
    erase_spaces(program_parameters);

    clearScr();
    xprintf("%z%s\n",set_output_color(blue,white),program_name);



    KEYBOARD_SIG_ON(no_keyboard_input);

    
    if(cmpstr(COMMAND,"petris"))
    {
        clearScr();
        tetris();
    }

    /*
    else if(cmpstr(COMMAND,"hexview"))
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
        xprintf("%z%s: unknown command\n",set_output_color(red,white),program_name);
    }
    
    KEYBOARD_SIG_OFF();

}
