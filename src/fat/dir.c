#pragma once

#include <fat/fat_driver.h>
#include <lib/stdiox.h>
#include <lib/algorithm.h>
#include <stddef.h>

void dir(void) 
{

    keyboard_scan_code = 0x0;

    char* print;
    int i = 0;

    root_directory_entry* file = ROOT_DIRECTORY_START;

    while(fat.current_folder[i] != FAT_UNALLOCATED)
    {
        xprintf("%s\n", file[fat.current_folder[i]].file_name);
        i++;
    }

    while(keyboard_scan_code != ENTER);
    exit_process();

}