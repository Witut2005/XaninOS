
#include <terminal/vty.h>
#include <fs/ustar.h>
#include <lib/stdiox.h>
#include <handlers/handlers.h>
#include <lib/signal.h>



void ls()
{


    for(int i=0; i < FILE_ENTRIES; i++)
    {
        sprint(red,white, fileTable[i]);
    }


    xprintf("press 'q' key to continue...");
    
    
    while(1)
    {
        if(keyboard_input == 'q')
        {
            {app_exited = true;break;}
        }
    }


}