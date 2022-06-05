

#pragma once

#include <libc/stdiox.h>

void loadch(char* address_string)
{

    screen_clear();
    
    

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
