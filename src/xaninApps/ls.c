
#include <terminal/vty.h>
#include <fs/ustar.h>
#include <lib/stdiox.h>
#include <handlers/handlers.h>
#include <lib/signal.h>
#include <headers/colors.h>
#include <lib/stdiox.h>
#include <lib/string.h>


void ls()
{

    for(int i=0; i < 2; i++)
        xprintf("\r%s %dbytes\n",fs_entry[i].entry_name, fs_entry[i].entry_size);


    xprintf("press 'q' key to continue...");
    
    
    while(1)
    {
        if(keyboard_input == 'q')
        {
            {app_exited = true;break;}
        }
    }


}