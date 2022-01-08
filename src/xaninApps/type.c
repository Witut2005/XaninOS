
#pragma once

#include "../ustar/ustar.h"
#include "../headers/colors.h"
#include "../handlers/handlers.c"

/* INTERRUPT */

void type()
{

    xprintf("\rplease type name file which you want to use\n");
    
    KEYBOARD_SIG_OFF();

    no_enter = true;

    scanCode = 0x0;

    while(1)
    {

        if(!index && scanCode == ENTER)
        {
        
            for(int i = 0; i < fileSystem.file_entries_number; i++)
            {
                if(cmpstr(COMMAND,fileTable[i]))
                {

                    xprintf("%s\n",fileData[i]);
                    goto end;
                }
            }
            
            goto error;

        }

    }

    error:
    xprintf("\r%z%s\n",getColors(red,white),COMMAND);
    xprintf("\r%z%s\n",getColors(red,white),"NO SUCH FILE OR DIRECTORY");


    end:

    xprintf("\n");
    xprintf("\npress 'q' key to continue...");
    
    
    

    while(1)
    {
        if(input == 'q')
        {
            for(int i = 0; i < sizeof(comBuf);i++)
                COMMAND[i] = '\0';

            index = 0x0; /* some problems with keyboard input when index is no reseted */
            exitApp = true; no_enter = false;break;
        }
    }

}