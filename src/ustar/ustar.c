
#include "./ustar.h"


#define FS_START_OFFSET 0x7c06



void ustarInit()
{    
    char* addr = *(char**)FS_START_OFFSET;

    for(int i = 0; i < sizeof(fileTable) / sizeof(uint32_t); i++)
    {
        fileTable[i] = addr;
        addr += 0x400;
    }

    addr = *(char**)FS_START_OFFSET;

    addr += 0x200;

    for(int i = 0; i < sizeof(fileData) / sizeof(uint32_t); i++)
    {

        fileData[i] = addr;
        addr += 0x400;
    }
        

}