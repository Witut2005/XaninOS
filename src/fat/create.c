
#include <fat/fat_driver.h>
#include <fat/find_file.c>
#include <lib/stdiox.h>
#include <lib/algorithm.h>

void create(char* file_name)
{

    uint8_t file_name_length_counter = 0x0;
    keyboard_scan_code = 0x0;
    
    if(!fat_get_name_length(file_name))
    {
        xprintf("%zINVALID FILE NAME\n", set_output_color(red,white));
        while(keyboard_scan_code != ENTER);
        exit_process();
    }

    root_directory_entry* free_root_entry = fat_find_free_root_entry();


    for(int i = 0; i < 512 / 2; i++)
    {
        
        if(fat.current_folder[i] == FAT_UNALLOCATED)
        {
            fat.current_folder[i] = (char)(((uint32_t)free_root_entry - ROOT_DIRECTORY_START) / 32);
            break;
        }

    }

    root_directory_entry* folder_info = find_file(fat_current_folder);
    uint16_t* folder_data = folder_info->starting_cluster * CLUSTER_SIZE;

    for(int i = 0; i < 512; i++)
    {
        if(folder_data[i] == FAT_UNALLOCATED)
            folder_data[i] = (uint32_t)(free_root_entry - ROOT_DIRECTORY_START) / 32;
    }

    fat_save_entry_name(file_name, free_root_entry);

    if(find_character('.', file_name) == nullptr)
    {
        xprintf("%zINCORRECT FILE EXTENSION\n",set_output_color(red,white));
        while(keyboard_scan_code != ENTER);
        exit_process();
    }

    int extension_counter = 0x0;

    for(char* i = find_character('.', file_name) + 1; i < find_character('.', file_name) + 4; i++, extension_counter++)
    {

        if(*i < 0x20)
            free_root_entry->file_extension[extension_counter] = 0x20;

        else 
            free_root_entry->file_extension[extension_counter] = *i;
    }



    fat_folder* new_entry = fat_find_free_folder_entry(used_folder);

    for(int i = 0; i < FILENAME_MAX_LENGTH; i++)
        new_entry->entry_name[i] = 'a';

    new_entry->entry_attr = FAT_FOLDER;
    //new_entry->file_extension[0] = '\0';
    new_entry->entry_case = 0x0;
    new_entry->creation_time_miliseconds = 0x0;
    new_entry->creation_time = 0x0;
    new_entry->creation_date = 0x0;
    new_entry->last_access_date = 0x0;
    new_entry->reserved = 0x4142;
    new_entry->last_modification_time = 0x0;
    new_entry->last_modification_date = 0x0;
    new_entry->starting_cluster = fat_find_unallocated_cluster();  


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

