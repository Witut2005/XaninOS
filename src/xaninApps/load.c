

#pragma once

#include <lib/stdiox.h>

void load(uint32_t address)
{

    clearScr();

    char* data_pointer = (char*)(address);

    for(int i = 0; i < 512; i++)
    {
        xprintf("%x ", data_pointer[i]);
    }

	while(keyboard_scan_code != ENTER);
	

}