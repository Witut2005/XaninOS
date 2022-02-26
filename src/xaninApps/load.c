

#pragma once

#include <lib/stdiox.h>

void load(uint32_t sector_number)
{

    clearScr();

    char* data_pointer = (char*)(sector_number);

    for(int i = 0; i < 512; i++)
    {
        xprintf("%x ", data_pointer[i]);
    }

    while(1);

}
