#pragma once

#include <xaninApps/loadch.c>
#include <libc/stdiox.h>

void list_files(void)
{

    uint32_t current_path_string_len = strlen(xin_current_directory);

    char* i = (char*)XIN_ENTRY_TABLE; 
    
    while(*i != '\0')
    {
        xprintf("%s\n", i); 
        i += 64;
    }

    while(KeyInfo.scan_code != ENTER);

}
