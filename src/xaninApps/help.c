
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
    xprintf("type file_name\n"); 
    xprintf("execute file_name\n");
    xprintf("note file_name\n");
    xprintf("touch file_name\n");
    xprintf("md directory_name\n");
    xprintf("cd directory_name\n");
    xprintf("pwd\n");

    xscanf("%d",app_exited);

    app_exited = true;
      

}
