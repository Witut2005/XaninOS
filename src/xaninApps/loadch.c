

#pragma once

#include <lib/stdiox.h>

void loadch(uint32_t address)
{

    clearScr();

    char* data_pointer = (char*)(address);

    for(int i = 0; i < 512; i++)
    {    
    	if(x == 32)
    	{
    		x = 0x0;
    		y++;
    	}
        xprintf("%c", data_pointer[i]);
    }

	while(keyboard_scan_code != ENTER);
}
