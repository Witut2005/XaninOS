
#pragma once

#include <xin_fs/xin.h>
#include <stddef.h>






uint8_t* xin_find_free_pointer(void)
{
    for(char* i = (char*)0x800; (uint32_t)i < 0x800 + (SECTOR_SIZE * 4); i++)
    {
        if(*(char*)i == '\0')
            return (uint8_t*)i;
    }

    return nullptr;

}

xin_entry* xin_find_free_entry(void)
{
    for(xin_entry* i = XIN_ENTRY_TABLE; (uint32_t)i < 0x1800 + (SECTOR_SIZE * 2); i++)
    {
        if(*(char*)i == '\0')
            return i;
    }

    return nullptr;

}

xin_entry* xin_find_entry(char* entry_name)
{
    for(xin_entry* i = XIN_ENTRY_TABLE; (uint32_t)i < 0x1800 +  (SECTOR_SIZE * 2); i++)
    {
        if(cmpstr(entry_name, (char*)i))
            return i;
    }

    return nullptr;

}

xin_entry* xin_init_fs(void)
{
    xin_entry* home_entry = (xin_entry*)XIN_ENTRY_TABLE;
    
    set_string(home_entry->entry_path, "/");
    xin_current_directory[0] = '/';

    home_entry->creation_date = 0x0;
    home_entry->creation_time = 0x0;
    home_entry->os_specific = 0xFFFF;
    home_entry->modification_date = 0x0;
    home_entry->modification_time = 0x0;
    home_entry->entry_permissions = PERMISSION_MAX;
    home_entry->entry_size = 0x0;
    home_entry->starting_sector = 0x0;

}


void xin_create_file(char* entry_name)
{
    xin_entry* entry = xin_find_free_entry();

    set_string(entry->entry_path, entry_name);
    entry->creation_date = 0x0;
    entry->creation_time = 0x0;
    entry->os_specific = 0xFFFF;
    entry->modification_date = 0x0;
    entry->modification_time = 0x0;
    entry->entry_permissions = PERMISSION_MAX;
    entry->entry_size = 0x0;
    entry->starting_sector = 0x0;
    entry->entry_type = XIN_FILE;

    uint8_t* write_entry = xin_find_free_pointer();
    write_entry[0] = XIN_ALLOCATED;
    write_entry[1] = XIN_EOF;

    exit_process();

}

void xin_create_directory(char* entry_name)
{

    if(entry_name[strlen(entry_name) - 1] != '/')
    {
        xprintf("%zMISSING / ENDING CHARACTER IN DIRECTORY NAME\n", set_output_color(red, white));
        while(keyboard_scan_code != ENTER);
        exit_process();
    }

    xin_entry* entry = xin_find_free_entry();

    set_string(entry->entry_path, entry_name);
    entry->creation_date = 0x0;
    entry->creation_time = 0x0;
    entry->os_specific = 0xFFFF;
    entry->modification_date = 0x0;
    entry->modification_time = 0x0;
    entry->entry_permissions = PERMISSION_MAX;
    entry->entry_size = 0x0;
    entry->starting_sector = 0x0;
    entry->entry_type = XIN_DIRECTORY;

    exit_process();

}


void xin_change_directory(char* new_directory)
{

    if(xin_find_entry(new_directory) == nullptr)
    {
        xprintf("%zNO SUCH DIRECTORY\n", set_output_color(red,white));
        while(keyboard_scan_code != ENTER);
        exit_process();
    }

    if(new_directory[strlen(new_directory) - 1] != '/')
    {
        xprintf("%zMISSING / ENDING CHARACTER IN DIRECTORY NAME\n", set_output_color(red, white));
        while(keyboard_scan_code != ENTER);
        exit_process();
    }

    set_string(xin_current_directory,new_directory);
    exit_process();
}

bool xin_remove_entry(char* entry_name)
{

}