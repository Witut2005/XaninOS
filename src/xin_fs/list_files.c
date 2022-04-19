#pragma once

#include <xaninApps/loadch.c>
#include <lib/stdiox.h>

void list_files(void)
{
    for(char* i = (char*)XIN_ENTRY_TABLE; (uint32_t)i < (XIN_ENTRY_TABLE + 0x1000); 
                                                                                i+=64)
    {
       xprintf("%s\n", i); 
    }

    while(KeyInfo.scan_code != ENTER);

}
