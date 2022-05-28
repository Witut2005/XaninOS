

#pragma once

#include <lib/stdiox.h>

void load(char* address_string)
{

    screen_clear();


    uint32_t address = strtoi(address_string, 16);
    char* data_pointer = (char*)(address);


    for(int i = 0; i < 28; i++)
    {
        (i * 16) >= 0x100 ? xprintf("0x%X:", i * 16) : xprintf("0x%X: ", i * 16);
        
        for(int j = 0; j < 16; j++)
        {
            xprintf(" %mX", data_pointer[ (16 * i) + j]);
        }
        xprintf("\n");
    }

	while(KeyInfo.scan_code != ENTER);

	

}
