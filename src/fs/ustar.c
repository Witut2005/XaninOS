
#pragma once

#include <fs/ustar.h>


uint32_t file_get_size(char* size_field)
{
    uint32_t result = 0x0;

    while(*size_field != '\0')
    {
        result = (result * 8) + ((*size_field) - '0');
        size_field++;
    }   

    return result;
}

char* set_current_directory(char* directory)
{
    for(int i = 0; directory[i] != '\0'; i++)
        current_directory[i] = directory[i];
}

char* get_current_directory(void)
{

    for(int i = 0; fs_entry[0].entry_name[i] != '\0'; i++)
        current_directory[i] = fs_entry[0].entry_name[i];

    return current_directory;
}

char* get_current_path(void)
{

    int i = 0x0;
    for(i = 0; current_directory[i] != '\0'; i++)
        current_path[i] = current_directory[i];

    for(int pos = 0x0; file_name[pos] != '\0'; i++, pos++)
        current_path[i] = file_name[pos];

    return current_path;
}

void file_system_init(void)
{    

    FileSystem.file_entries_number = *(uint8_t*)FILE_ENTRIES_NUMBER;
    FileSystem.file_system_start = *(char**)FS_START_OFFSET;

    current_directory = current_directory_buffer;
    current_path = current_path_buffer;
    file_name = file_name_buffer;

    char* file_system_initializer = *(char**)FS_START_OFFSET;

    for(int i = 0; i < FileSystem.file_entries_number; i++)
    {
        fs_entry[i].entry_name =  file_system_initializer;
        fs_entry[i].entry_size_pointer = file_system_initializer + 0x7c;
        fs_entry[i].entry_size = file_get_size(fs_entry[i].entry_size_pointer);
        fs_entry[i].entry_type = *(file_system_initializer + 0x9C);
        fs_entry[i].sub_entries = 0x0;
        fs_entry[i].owner = 0x0;
        fs_entry[i].group = 0x0;

        file_system_initializer += USTAR_SECTOR_SIZE;
    
        if(fs_entry[i].entry_type != DIRECTORY)
        {
            fs_entry[i].entry_data_pointer = file_system_initializer;
            file_system_initializer += USTAR_SECTOR_SIZE * ((fs_entry[i].entry_size / USTAR_SECTOR_SIZE));
            file_system_initializer += (fs_entry[i].entry_size % USTAR_SECTOR_SIZE == 0) ? 0 : USTAR_SECTOR_SIZE;
        }
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