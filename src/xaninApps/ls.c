
#include <terminal/vty.h>
#include <fs/ustar.h>
#include <lib/stdiox.h>
#include <handlers/handlers.h>
#include <lib/signal.h>
#include <headers/colors.h>
#include <lib/stdiox.h>


void ls()
{


    for(int i=0; i < FILE_ENTRIES; i++)
        xprintf("%z%s",set_output_color(black,white),fileTable[i]);
    


    xprintf("press 'q' key to continue...");
    
    
    while(1)
    {
        if(keyboard_input == 'q')
        {
            {app_exited = true;break;}
        }
    }


}