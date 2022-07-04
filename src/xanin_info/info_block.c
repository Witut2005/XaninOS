
#pragma once

#include <xanin_info/info_block.h>

XaninInfoBlock* xanin_information_block_get(void)
{   
    char* tmp = (char*)0x7C00;
    while(!strcmp(tmp, "XANIN_INFO")){tmp++;};

    return (XaninInfoBlock*) tmp;


}