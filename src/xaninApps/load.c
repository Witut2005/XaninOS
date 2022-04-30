

#pragma once

#include <lib/stdiox.h>

void load(char* address_string)
{

    screen_clear();

    if(strcmp(address_string, "help"))
    {
        xprintf("xaninOS developer tools\n");
        xprintf("Load program loads given address to the screen in hexadecimal format\n");
    	while(KeyInfo.scan_code != ENTER);
        return;
    }
    


    uint32_t address = strtoi(address_string, 16);
    char* data_pointer = (char*)(address);


    for(int i = 0; i < 28; i++)
    {
        for(int j = 0; j < 26; j++)
        {
            xprintf(" %mx", data_pointer[ (16 * i) + j]);
        }
        xprintf("\n");
    }

	while(KeyInfo.scan_code != ENTER);

	

}
