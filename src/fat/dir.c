#pragma once

#include <fat/fat_driver.h>
#include <lib/stdiox.h>
#include <lib/algorithm.h>
#include <stddef.h>

void dir(void) 
{

    keyboard_scan_code = 0x0;

    char* print;


    for(print = (char*)0x1800; *print != NULL; print += 32)
        xprintf("%s\n", print);

    while(keyboard_scan_code != ENTER);

    exit_process();

}