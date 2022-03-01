
#pragma once


#include <fat/fat_driver.h>
#include <lib/exit_process.h>
#include <fat/find_file.c>
#include <lib/string.h>
#include <lib/stdiox.h>
#include <lib/memory.h>

void modify(char* file_name)
{

    //keyboard_trun_on();
    no_enter = true;
    clearScr();

    union fat_file
    {
    
        char fat_file_id[12];

        struct
        {
            char file_name[8];
            char file_extension[4];
   
        };
    }file;

    arrows_navigate = true;

    uint8_t file_name_length_counter = 0x0;

    while(file_name[file_name_length_counter] != '.')
    {
        if(file_name_length_counter > 8)
            xprintf("%zFILE NAME CANT BE MORE THAN 8 CHARACTERS LONG", 
                                        set_output_color(red,white));
        file_name_length_counter++;
    }

    for(int i = 0; i < FILENAME_MAX_LENGTH; i++)
    {

        if(i < file_name_length_counter)
        {
            if(file_name[i] < 0x20)
                file.file_name[i] = 0x20;
            
            else 
                file.file_name[i] = file_name[i];
        }
    
        else
        {
            file.file_name[i] = 0x20;
        }
    
    }

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
            file.file_extension[extension_counter] = 0x20;

        else 
            file.file_extension[extension_counter] = *i;
    }

    
    root_directory_entry* finded_entry;

    file.file_extension[3] = '\0';

    xprintf("your file: %s\n", file.fat_file_id);


    if(find_file(file.fat_file_id) == nullptr)
    {
        xprintf("%zNO SUCH FILE\n", set_output_color(red,white));
        while(keyboard_scan_code != ENTER);
        exit_process();
    }

    clearScr();
    keyboard_scan_code = 0x0;

    char* file_data = finded_entry->starting_cluster * CLUSTER_SIZE;
    
    for(int i = 0; i < finded_entry->file_size; i++)
    	xprintf("%c", file_data[i]);

	cursor = (uint16_t*)VGA_TEXT_MEMORY;

    while(keyboard_scan_code != F4_KEY);


    
    //xprintf("starting cluster: %d\n", finded_entry->starting_cluster);
    //xprintf("name finded: %s\n", finded_entry->file_name);
    //xprintf("file pointer: %d\n", file_data);



   
    uint32_t file_data_counter = 0x0;
    for(char* i = (char*)VGA_TEXT_MEMORY; 
            (uint32_t)i < VGA_TEXT_MEMORY + (512 * 2); i+=2, file_data_counter++)
            file_data[file_data_counter] = *i;

    finded_entry->file_size = file_data_counter;
    
    for(int i = 0; i < sizeof(comBuf);i++)
        keyboard_command[i] = '\0';
 
    no_enter = false;
    index = 0x0; 
}
