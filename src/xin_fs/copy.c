

#include <xin_fs/xin.h>

void xin_copy(char* file_name, char* new_file_name)
{

    xin_entry* entry = xin_find_entry(file_name);

    xin_create_file(new_file_name);

    xin_entry* new_entry = xin_find_entry(new_file_name);

    int counter = 0; 
    char* entry_data = entry->starting_sector * SECTOR_SIZE;
    char* new_entry_data = new_entry->starting_sector * SECTOR_SIZE;
    
    for(; counter <= SECTOR_SIZE * 0x10; counter++)
    {
        new_entry_data[counter] = entry_data[counter];
    }

}
