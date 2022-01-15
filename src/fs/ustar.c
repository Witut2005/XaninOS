
#include <fs/ustar.h>


uint32_t file_get_size(char* str)
{


    uint32_t result = 0x0;

    for(int i = 0; i < BYTES_PER_FILE_SIZE_FIELD && *str != '\0'; i++)
    {
        result = (result * 8) + ((*str) - '0');
        str++;
    }   


    
    return result;

}


void file_system_init()
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