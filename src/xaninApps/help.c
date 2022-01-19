
#pragma once

#include <lib/stdiox.h>

void help(void)
{

    no_enter = true;
    
    xprintf("AVAILABLE COMMANDS:\n");
    xprintf("petris\n");
    xprintf("pong\n");
    xprintf("cls\n");
    xprintf("ls\n");
    xprintf("type\n");
    xprintf("cd\n");
    xprintf("pwd\n");
    xprintf("execute\n");
    xprintf("note\n");


    xprintf("press 'q' to continue...\n");
    
    while(1)
    {
        
        if(keyboard_input == 'q')
        {
            app_exited = true;
            break;
        }
    
    }


}