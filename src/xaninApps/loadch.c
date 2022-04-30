

#pragma once

#include <lib/stdiox.h>

void loadch(char* address_string)
{

    screen_clear();
    
    if(strcmp(address_string, "help"))
    {
        xprintf("xaninOS developer tools\n");
        xprintf("Load program loads given address to the screen in ASCII format\n");
    	while(KeyInfo.scan_code != ENTER);
        return;
    }
    

    uint32_t address = strtoi(address_string, 16);

    char* data_pointer = (char*)(address);

    for(int i = 0; i < 512; i++)
    {    
    	if(Screen.x == 32)
    	{
    		Screen.x = 0x0;
            Screen.y++;
    	}
        putchar(data_pointer[i]);
    }

	while(KeyInfo.scan_code != ENTER);
}
