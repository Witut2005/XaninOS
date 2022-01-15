
#include <fs/ustar.h>


uint32_t file_get_size(char* size_field)
{
    uint32_t result = 0x0;

    for(int i = 0;  *size_field != '\0'; i++)
    {
        result = (result * 8) + ((*size_field) - '0');
        size_field++;
    }   

    return result;
}


void file_system_init()
{    

    FileSystem.file_entries_number = *(uint8_t*)FILE_ENTRIES_NUMBER;
    FileSystem.file_system_start = *(char**)FS_START_OFFSET;

    

    char* file_system_initalizer = *(char**)FS_START_OFFSET;

    for(int i = 0; i < 2; i++)
    {
        fs_entry[i].entry_name =  file_system_initalizer;
        fs_entry[i].entry_size_pointer = file_system_initalizer + 0x7c;
        fs_entry[i].entry_size = file_get_size(fs_entry[i].entry_size_pointer);
        fs_entry[i].sub_entries = 0x0;
        fs_entry[i].owner = 0x0;
        fs_entry[i].group = 0x0;

        file_system_initalizer += 0x200;
        fs_entry[i].entry_data_pointer = file_system_initalizer;
        
        file_system_initalizer += 0x200 * ((fs_entry[i].entry_size / USTAR_SECTOR_SIZE) + 1);

    }    



    char* addr = *(char**)FS_START_OFFSET;

    for(int i = 0; i < FileSystem.file_entries_number; i++)
    {
        fileTable[i] = addr;
        addr += 0x400;
    }

    addr = *(char**)FS_START_OFFSET;
    addr += 0x200;

    for(int i = 0; i < FileSystem.file_entries_number; i++)
    {
        fileData[i] = addr;
        addr += 0x400;
    }

}