
#include <fat/fat_driver.h>
#include <lib/stdiox.h>


void create(char* file_name)
{

    uint8_t file_name_length_counter = 0x0;

    char* unallocated_entry;

    for(unallocated_entry = (char*)0x800; *unallocated_entry != 0 && (uint32_t)unallocated_entry < (0x800 + (CLUSTER_SIZE * 4)); unallocated_entry++);

    *unallocated_entry = FAT_ALLOCATED;
    *(unallocated_entry + 1) = FAT_END_FILE;
    
    while(file_name[file_name_length_counter] != '.')
    {
        if(file_name_length_counter > 8)
            xprintf("%zFILE NAME CANT BE MORE THAN 8 CHARACTERS LONG", 
                                        set_output_color(red,white));
        file_name_length_counter++;
    }

    for(int i = 0; i < FILENAME_MAX_LENGTH; i++)
    {
        if(file_name[i] < 0x20)
            root_directory_table[fat_entries].file_name[i] = 0x20;
        
        else 
            root_directory_table[fat_entries].file_name[i] = file_name[i];
    }

    if(file_name[8] != '.')
    {
        xprintf("%zINCORRECT FILE EXTENSION\n",set_output_color(red,white));
    }

    for(int i = 9; i < 12; i++)
    {
        if(file_name[i] < 0x20)
            root_directory_table[fat_entries].file_extension[i-9] = 0x20;
        
        else 
            root_directory_table[fat_entries].file_extension[i-9] = file_name[i];
    }


    root_directory_table[fat_entries].file_attr = 0x0;
    root_directory_table[fat_entries].entry_case = 0x0;
    root_directory_table[fat_entries].creation_time_miliseconds = 0x0;
    root_directory_table[fat_entries].creation_time = 0x0;
    root_directory_table[fat_entries].creation_date = 0x0;
    root_directory_table[fat_entries].last_access_date = 0x0;
    root_directory_table[fat_entries].reserved = 0x0;
    root_directory_table[fat_entries].last_modification_time = 0x0;
    root_directory_table[fat_entries].last_modification_date = 0x0;
    root_directory_table[fat_entries].starting_cluster = fat_find_unallocated_cluster();  
    root_directory_table[fat_entries].file_size = 512;



    fat_entries += 2;

    app_exited = true;
}

