
#pragma once

#include <xin_fs/xin.h>
#include <stddef.h>

char* xin_set_current_directory(char* directory)
{
    for(int i = 0; i < sizeof(xin_current_directory); i++)
        xin_current_directory[i] = '\0';
    
    for(int i = 0; directory[i] != '\0'; i++)
        xin_current_directory[i] = directory[i];
}

char* xin_get_current_directory(void)
{
    return xin_current_directory;
}

char* xin_get_current_path(char* file_name)
{

    for(int j = 0; j < 50; j++)
        xin_current_path[j] = '\0';

    int i = 0x0;

    for(i = 0; xin_current_directory[i] != '\0'; i++)
        xin_current_path[i] = xin_current_directory[i];

    for(int pos = 0x0; file_name[pos] != '\0'; i++, pos++)
        xin_current_path[i] = file_name[pos];

    return xin_current_path;
}

/* DIRECTORY AND FILES */
xin_entry* xin_find_entry(char* entry_name)
{
    for(char* i = XIN_ENTRY_TABLE; (uint32_t)i < XIN_ENTRY_TABLE + (SECTOR_SIZE * 2); i += 32)
    {
        if(cmpstr(entry_name, i))     
        {
            return (xin_entry*)i;
        }
    }


    entry_name = xin_get_current_path(entry_name);

    for(char* i = XIN_ENTRY_TABLE; (uint32_t)i < XIN_ENTRY_TABLE + (SECTOR_SIZE * 2); i += 32)
    {
        if(cmpstr(entry_name, i))  
        {   
            return (xin_entry*)i;    
        }
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

xin_entry* xin_find_free_entry(void)
{
    for(char* i = (char*)XIN_ENTRY_TABLE + 64; (uint32_t)i < 0x1800 + (SECTOR_SIZE * 2); i += 64)
    {
        if(*(char*)i == '\0')
            return (xin_entry*)i;
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

    xprintf("addr: 0x%x\n", (uint32_t)entry->entry_path);
    //while(1);
    
    set_string(entry->entry_path, entry_name);
    
    entry->creation_date = 0x0;
    entry->creation_time = 0x0;
    entry->os_specific = 0xFFFF;
    entry->modification_date = 0x0;
    entry->modification_time = 0x0;
    entry->entry_permissions = PERMISSION_MAX;
    entry->entry_size = 0x0;
    entry->entry_type = XIN_FILE;

    uint8_t* write_entry = xin_find_free_pointer();

    for(int i = 0; i < 15; i++)
        write_entry[i] = XIN_ALLOCATED;

    write_entry[15] = XIN_EOF;

    entry->starting_sector = (uint32_t)write_entry - XIN_ENTRY_POINTERS;

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


xin_entry* xin_change_directory(char* new_directory)
{


    if(xin_find_entry(new_directory) == nullptr)
    {
        xprintf("%zNO SUCH DIRECTORY\n", set_output_color(red,white));
        while(keyboard_scan_code != ENTER);
        return nullptr;
    }

    if(new_directory[strlen(new_directory) - 1] != '/')
    {
        xprintf("%zMISSING / ENDING CHARACTER IN DIRECTORY NAME\n", set_output_color(red, white));
        while(keyboard_scan_code != ENTER);
        return nullptr;
    }

    set_string(xin_current_directory,new_directory);
    
    exit_process(xin_find_entry(new_directory));

}

bool xin_remove_entry(char* entry_name)
{   
    
    xprintf("%s", entry_name);

    char* entry_to_delete = (char*)xin_find_entry(entry_name);

    xin_entry* entry_data = (xin_entry*)entry_to_delete;

    if(entry_to_delete == nullptr)
    {
        xprintf("%zNO SUCH DIRECTORY\n", set_output_color(red,white));
        while(keyboard_scan_code != ENTER);
        exit_process(false);
    }   

    if(entry_data->entry_type == XIN_FILE) 
    {

        for(char* i = (char*)entry_data->starting_sector + XIN_ENTRY_POINTERS;
                *i != XIN_EOF; i++)
        {
            *i = XIN_UNALLOCATED;
        }

    }

    for(int i = 0; i < sizeof(xin_entry); i++)
        entry_to_delete[i] = '\0';

    exit_process(true);

}


void xin_entry_info(char* entry_name)
{
    
    xin_entry* xin_file_descriptor = xin_find_entry(entry_name);

    xprintf("entry path: %s\n", xin_file_descriptor->entry_path);
    xprintf("creation date: 0x%x\n", xin_file_descriptor->creation_time);
    xprintf("starting_sector: 0x%x\n", xin_file_descriptor->starting_sector);

    while(KeyInfo.scan_code != ENTER);
    exit_process();

}


