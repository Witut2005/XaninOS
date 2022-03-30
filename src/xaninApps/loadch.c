

#pragma once

#include <lib/stdiox.h>

void loadch(uint32_t address)
{

    screen_clear();

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
