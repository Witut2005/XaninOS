
#include "./ustar.h"


#define FS_START_OFFSET 0x7c06

#define FILE_ENTRIES sizeof(fileTable) / sizeof(fileTable[0])
#define FILE_DATA_ENTRIES sizeof(fileData) / sizeof(fileData[0])


void ustarInit()
{    
    char* addr = *(char**)FS_START_OFFSET;

    for(int i = 0; i < FILE_ENTRIES; i++)
    {
        fileTable[i] = addr;
        addr += 0x400;
    }

    addr = *(char**)FS_START_OFFSET;

    addr += 0x200;

    for(int i = 0; i < FILE_DATA_ENTRIES; i++)
    {

        fileData[i] = addr;
        addr += 0x400;
    }
        
}