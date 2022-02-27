
#include <fat/fat_driver.h>
#include <lib/stdiox.h>
#include <lib/algorithm.h>

void create(char* file_name)
{

    uint8_t file_name_length_counter = 0x0;

    
    while(file_name[file_name_length_counter] != '.')
    {
        if(file_name_length_counter > 8)
            xprintf("%zFILE NAME CANT BE MORE THAN 8 CHARACTERS LONG", 
                                        set_output_color(red,white));
        file_name_length_counter++;
    }

    root_directory_entry* free_root_entry = fat_find_free_root_entry();

    for(int i = 0; i < FILENAME_MAX_LENGTH; i++)
    {
    
        if(i < file_name_length_counter)
        {
            if(file_name[i] < 0x20)
                free_root_entry->file_name[i] = 0x20;
            
            else 
                free_root_entry->file_name[i] = file_name[i];
        }
    
        else
        {
            free_root_entry->file_name[i] = 0x20;
        }
    }

    if(find_character('.', file_name) == nullptr)
    {
        xprintf("%zINCORRECT FILE EXTENSION\n",set_output_color(red,white));
    }

    int extension_counter = 0x0;

    for(char* i = find_character('.', file_name) + 1; i < find_character('.', file_name) + 4; i++, extension_counter++)
    {

        if(*i < 0x20)
            free_root_entry->file_extension[extension_counter] = 0x20;

        else 
            free_root_entry->file_extension[extension_counter] = *i;
    }


    /*
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

    */

    free_root_entry->file_attr = 0x0;
    free_root_entry->entry_case = 0x0;
    free_root_entry->creation_time_miliseconds = 0x0;
    free_root_entry->creation_time = 0x0;
    free_root_entry->creation_date = 0x0;
    free_root_entry->last_access_date = 0x0;
    free_root_entry->reserved = 0x0;
    free_root_entry->last_modification_time = 0x0;
    free_root_entry->last_modification_date = 0x0;
    free_root_entry->starting_cluster = fat_find_unallocated_cluster();  
    
    char* write_entry = (char*)0x800 + free_root_entry->starting_cluster;
    *write_entry = FAT_ALLOCATED;
    *(write_entry + 1) = FAT_END_FILE;
    
    free_root_entry->file_size = 512;

    fat_entries += 2;

    keyboard_scan_code = 0x0;

    while(keyboard_scan_code != ENTER);

    for(int i = 0; i < sizeof(comBuf);i++)
        keyboard_command[i] = '\0';
    app_exited = true;
}

