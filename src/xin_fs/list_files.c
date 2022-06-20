#pragma once

#include <libc/stdiox.h>

void list_files(char* path)
{

    char* i = (char*)XIN_ENTRY_TABLE; 
    
    while(*i != '\0')
    {

        if(strncmp(i, xin_current_directory, strlen(xin_current_directory)) && strlen(path) == 0)
            xprintf("%s\n", i); 

        if(strlen(path) > 0)
        {
            if(strncmp(i, path, strlen(path)))
                xprintf("%s\n", i);
        }

        i += 64;
    }

    while(KeyInfo.scan_code != ENTER);

}
