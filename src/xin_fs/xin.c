
#pragma once

#include <xin_fs/xin.h>
#include <stddef.h>


xin_entry* xin_init_fs(void)
{
    xin_entry* home_entry = (xin_entry*)XIN_ENTRY_TABLE;
    
    set_string(home_entry->entry_path, "/");
    home_entry->creation_date = 0x0;
    home_entry->creation_time = 0x0;
    home_entry->os_specific = 0xFFFF;
    home_entry->modification_date = 0x0;
    home_entry->modification_time = 0x0;
    home_entry->entry_permissions = PERMISSION_MAX;
    home_entry->entry_size = 0x0;
    home_entry->starting_sector = 0x0;

}

xin_entry* xin_find_free_entry(void)
{
    for(xin_entry* i = XIN_ENTRY_TABLE; (uint32_t)i < 0x1800 * SECTOR_SIZE; i++)
    {
        if(*(char*)i == '\0')
            return i;
    }

    return nullptr;

}

uint8_t* xin_find_free_pointer(void)
{
    for(char* i = (char*)0x800; (uint32_t)i < 0x800 + (SECTOR_SIZE * 4); i++)
    {
        if(*(char*)i == '\0')
            return (uint8_t*)i;
    }

    return nullptr;

}



xin_entry* xin_create_file(char* entry_name)
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

    uint8_t* write_entry = xin_find_free_pointer();
    write_entry[0] = XIN_ALLOCATED;
    write_entry[1] = XIN_EOF;

}


bool xin_remove_entry(char* entry_name)
{

}