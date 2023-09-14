

#pragma once

#include <lib/libc/string.h>
#include <lib/libc/stdiox.h>

//CANVAS_APP

int loadch(char* address_string)
{

    stdio_mode_set(STDIO_MODE_CANVAS);
    canvas_screen_clear();
    
    uint32_t address = strtoi(address_string, HEXADECIMAL);

    char* data_pointer = (char*)(address);

    for(int i = 0; i < 512; i++)
    {    
    	if(Screen.x == 32)
    	{
    		Screen.x = 0x0;
            Screen.y++;
    	}
        canvas_putchar(data_pointer[i]);
    }

	while(inputg().scan_code != ENTER);
    return XANIN_OK;
}
