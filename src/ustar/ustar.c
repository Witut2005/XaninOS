
#include "./ustar.h"





void ustarInit()
{    

    fileSystem.file_entries_number = *(uint8_t*)FILE_ENTRIES_NUMBER;
    fileSystem.file_system_start = *(char**)FS_START_OFFSET;


    char* addr = *(char**)FS_START_OFFSET;

    for(int i = 0; i < fileSystem.file_entries_number; i++)
    {
        fileTable[i] = addr;
        addr += 0x400;
    }

    addr = *(char**)FS_START_OFFSET;
    addr += 0x200;

    for(int i = 0; i < fileSystem.file_entries_number; i++)
    {
        fileData[i] = addr;
        addr += 0x400;
    }
        
}