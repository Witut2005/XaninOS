

#pragma once

#include <lib/stdiox.h>

void load(uint32_t address)
{

    screen_clear();

    char* data_pointer = (char*)(address);


    for(int i = 0; i < 28; i++)
    {
        for(int j = 0; j < 26; j++)
        {
            xprintf(" %mx", data_pointer[ (16 * i) + j]);
        }
        xprintf("\n");
    }

	while(keyboard_scan_code != ENTER);

	

}
