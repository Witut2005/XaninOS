
#include <fat/fat_driver.h>
#include <lib/stdiox.h>
#include <lib/algorithm.h>
#include <lib/string.h>

void mkdir(char* file_name)
{

    uint8_t file_name_length_counter = strlen(file_name);
    keyboard_scan_code = 0x0;


    if(file_name_length_counter >= 8)
    {
        xprintf("%zFILE NAME CANT BE MORE THAN 8 CHARACTERS LONG", 
                                        set_output_color(red,white));
        
        while(keyboard_scan_code != ENTER);
        exit_process();
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


    fat_folder* new_entry = fat_find_free_folder_entry(used_folder);


    for(int i = 0; i < FILENAME_MAX_LENGTH; i++)
        new_entry->entry_name[i] = "a";

    new_entry->entry_attr = FAT_FOLDER;
    //new_entry->file_extension[0] = '\0';
    new_entry->entry_case = 0x0;
    new_entry->creation_time_miliseconds = 0x0;
    new_entry->creation_time = 0x0;
    new_entry->creation_date = 0x0;
    new_entry->last_access_date = 0x0;
    new_entry->reserved = 0x0;
    new_entry->last_modification_time = 0x0;
    new_entry->last_modification_date = 0x0;
    new_entry->starting_cluster = fat_find_unallocated_cluster();  


    free_root_entry->file_attr = FAT_FOLDER;
    free_root_entry->file_extension[0] = '\0';
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

    exit_process();

}

