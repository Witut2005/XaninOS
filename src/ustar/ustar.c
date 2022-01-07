
#include "./ustar.h"





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