#pragma once

#include <fat/fat_driver.h>
#include <lib/string.h>
#include <lib/stdiox.h>
#include <lib/memory.h>

root_directory_entry* find_file(char* file_name)
{

    for(char* i = (char*)0x1800; (uint32_t)i < (0x1800 + (CLUSTER_SIZE * 2)); i += 32)
    {
     
        if(cmpstr(file_name, i))
        {
         	//xprintf("%zFILE EXIST", set_output_color(green,white));
            return (root_directory_entry*)i;
		}
		
		xprintf("%s\n", (char*)0x1820);
    }


    return nullptr;
}
