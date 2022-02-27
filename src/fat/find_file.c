

#include <fat/fat_driver.h>
#include <lib/string.h>
#include <lib/stdiox.h>

root_directory_entry* find_file(char* file_name)
{

    for(char* i = (char*)0x1800; (uint32_t)i < 0x1800 + (CLUSTER_SIZE * 2); i += 32)
    {
    
        if(cmpstr(file_name, i) == 1)
            return (root_directory_entry*)i;
        
    }

    return nullptr;
}
