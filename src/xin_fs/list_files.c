#pragma once

#include <xaninApps/loadch.c>
#include <lib/stdiox.h>

void list_files(void)
{

    uint32_t current_path_string_len = strlen(xin_current_directory);

    for(char* i = (char*)XIN_ENTRY_TABLE; (uint32_t)i < (XIN_ENTRY_TABLE + 0x1000); i+=64)                                                                  
    {
        //if(strncmp(xin_current_directory, i, current_path_string_len))
            xprintf("%s\n", i); 
    }

    while(KeyInfo.scan_code != ENTER);

}
