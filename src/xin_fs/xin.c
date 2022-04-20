
#pragma once

#include <xin_fs/xin.h>
#include <stddef.h>


xin_entry* current_program = XIN_ENTRY_TABLE + (4 * sizeof(xin_entry));

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
    for(char* i = XIN_ENTRY_TABLE; (uint32_t)i < XIN_ENTRY_TABLE + (SECTOR_SIZE * 4); i += 32)
    {
        if(cmpstr(entry_name, i))     
        {
            return (xin_entry*)i;
        }
    }


    entry_name = xin_get_current_path(entry_name);

    for(char* i = XIN_ENTRY_TABLE; (uint32_t)i < XIN_ENTRY_TABLE + (SECTOR_SIZE * 4); i += 32)
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

void xin_file_create_at_address(char *path, uint8_t creation_date, uint8_t creation_time, 
                            uint16_t os_specific, uint8_t modification_date, 
                                uint8_t modification_time, uint8_t permissions, 
                                    uint8_t starting_sector, uint8_t entry_size)
{
    xin_entry* entry_created = xin_find_free_entry();
    set_string(entry_created -> entry_path, path);
    
    entry_created->creation_date = creation_date;
    entry_created->creation_time = creation_time;
    entry_created->os_specific = os_specific;
    entry_created->modification_date = modification_date;
    entry_created->modification_time = modification_time;
    entry_created->entry_permissions = permissions;
    entry_created->entry_size = entry_size;
    entry_created->starting_sector = starting_sector;

    uint8_t* write_entry = xin_find_free_pointer();
    entry_size--;

    for(uint8_t* i = write_entry;  i < ( write_entry + entry_size); i++)
        *i = XIN_ALLOCATED;
    
    *(write_entry + entry_size) = XIN_EOF;


}

void xin_folder_create(char *path, uint8_t creation_date, uint8_t creation_time, uint16_t os_specific, 
                    uint8_t modification_date, uint8_t modification_time, uint8_t permissions)
{
    xin_entry* entry_created = xin_find_free_entry();
    set_string(entry_created -> entry_path, path);
    
    entry_created->creation_date = creation_date;
    entry_created->creation_time = creation_time;
    entry_created->os_specific = os_specific;
    entry_created->modification_date = modification_date;
    entry_created->modification_time = modification_time;
    entry_created->entry_permissions = permissions;
    entry_created->entry_size = 0x0;
    entry_created->starting_sector = 0x0;

}



xin_entry* xin_init_fs(void)
{
    xin_folder_create("/", 0x0, 0x0, 0xFFFF, 0x0, 0x0, PERMISSION_MAX);
    xin_file_create_at_address("ivt.bin", 0x0, 0x0, XIN_READ_ONLY, 0x0, 0x0, PERMISSION_MAX, 0x0, 0x2);
    xin_file_create_at_address("file_system.bin", 0x0, 0x0, XIN_READ_ONLY, 0x0, 0x0, PERMISSION_MAX, 0x2, 0xE + 0x10);
    xin_file_create_at_address("boot.bin", 0x0, 0x0, XIN_READ_ONLY, 0x0, 0x0, PERMISSION_MAX, 0x3E, 0x1);
    xin_file_create_at_address("tmp.bin", 0x0, 0x0, 0xFFFF, 0x0, 0x0, PERMISSION_MAX, 0x80, 1);
    xin_file_create_at_address("shutdown.bin", 0x0, 0x0, XIN_READ_ONLY, 0x0, 0x0, PERMISSION_MAX, 0x81, 0x1);
    xin_file_create_at_address("syscall_test.bin", 0x0, 0x0, 0xFFFF, 0x0, 0x0, PERMISSION_MAX, 0x82, 1);


    xin_change_directory("/");

}



void xin_create_file(char* entry_name)
{
    xin_entry* entry = xin_find_free_entry();

    xprintf("addr: 0x%x\n", (uint32_t)entry->entry_path);
    //while(1);
   
    entry_name = xin_get_current_path(entry_name);

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


void xin_create_directory(char* entry_path)
{

    if(entry_path[strlen(entry_path) - 1] != '/')
    {
        xprintf("%zMISSING / ENDING CHARACTER IN DIRECTORY NAME\n", set_output_color(red, white));
        while(KeyInfo.scan_code != ENTER);
        exit_process();
    }

    if(xin_find_entry(entry_path) == nullptr)
    {
        xprintf("NO SUCH DIRECTORY\n", set_output_color(red, white));
        while(KeyInfo.scan_code != ENTER);
        return;
    }

    
    xin_entry* entry = xin_find_free_entry();

    char entry_name[40];
    xprintf("your parent folder: %s\n", entry_path);
    xprintf("type your new directory name: ");
    xscanf("%s", entry_name);

    int i = 0;

    for(; entry_path[i] != '\0'; i++)
        entry->entry_path[i] = entry_path[i];
    

    for(int j = 0; entry_name[j] != '\0'; i++, j++)
        entry->entry_path[i] = entry_name[j];



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

/*
void xin_create_directory(char* entry_name)
{

    if(entry_name[strlen(entry_name) - 1] != '/')
    {
        xprintf("%zMISSING / ENDING CHARACTER IN DIRECTORY NAME\n", set_output_color(red, white));
        while(KeyInfo.scan_code != ENTER);
        exit_process();
    }

    xin_entry* entry = xin_find_free_entry();

    entry_name = xin_get_current_path(entry_name);

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
*/

xin_entry* xin_change_directory(char* new_directory)
{


    xin_entry* xin_new_directory = xin_find_entry(new_directory);

    if(xin_new_directory == nullptr)
    {
        xprintf("%zNO SUCH DIRECTORY\n", set_output_color(red,white));
        while(KeyInfo.scan_code != ENTER);
        return nullptr;
    }

    if(new_directory[strlen(new_directory) - 1] != '/')
    {
        xprintf("%zMISSING / ENDING CHARACTER IN DIRECTORY NAME\n", set_output_color(red, white));
        while(KeyInfo.scan_code != ENTER);
        return nullptr;
    }

    /*
    if(new_directory[0] != '/')
    {
        new_directory = xin_get_current_path(new_directory);
    }
    */

    for(int i = 0; i < sizeof(xin_current_directory); i++)
        xin_current_directory[i] = '\0';
    
    
    set_string(xin_current_directory, xin_new_directory->entry_path);
   

    xprintf("your file: %s", xin_current_directory);

    return xin_new_directory;

}

bool xin_remove_entry(char* entry_name)
{   
    
    char* entry_to_delete = (char*)xin_find_entry(entry_name);

    xin_entry* entry_data = (xin_entry*)entry_to_delete;

    if(entry_to_delete == nullptr)
    {
        xprintf("%zNO SUCH DIRECTORY\n", set_output_color(red,white));
        while(KeyInfo.scan_code != ENTER);
        exit_process(false);
    }   

    xprintf("starting addr: 0x%x\n", entry_data->starting_sector + XIN_ENTRY_POINTERS);


    if(entry_data->entry_type == XIN_FILE) 
    {
        for(char* i = (char*)entry_data->starting_sector + XIN_ENTRY_POINTERS; 
            (uint32_t)i < entry_data->starting_sector + XIN_ENTRY_POINTERS + 16; i++)
        {
            *i = XIN_UNALLOCATED;
            //xprintf("%x",*i);
        }
    }

    for(int i = 0; i < sizeof(xin_entry); i++)
        entry_to_delete[i] = '\0';

    return true;

    exit_process(true);


}


void xin_entry_info(char* entry_name)
{
    
    xin_entry* xin_file_descriptor = xin_find_entry(entry_name);

    if(xin_file_descriptor != nullptr)
    {
        xprintf("\n\rentry path: %s\n", xin_file_descriptor->entry_path);
        xprintf("creation date: 0x%x\n", xin_file_descriptor->creation_time);
        xprintf("starting_sector: 0x%x\n", xin_file_descriptor->starting_sector);
    }

    while(KeyInfo.scan_code != ENTER);
    exit_process();

}

uint32_t xin_get_start_sector(char* entry_name)
{
    xin_entry* xin_file_descriptor = xin_find_entry(entry_name);

    return xin_file_descriptor->starting_sector;

}
