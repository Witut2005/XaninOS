

#include <xin_fs/xin.h>
#include <libc/memory.h>
#include <stddef.h>
#include <devices/HARD_DISK/disk.h>
#include <xin_fs/xin_entry.h>
#include <libc/string.h>
#include <libc/stdlibx.h>
#include <libc/colors.h>
#include <terminal/vty.h>
#include <keyboard/scan_codes.h>

//#define IF_FILE_NOT_EXIST

uint8_t enter_real_mode_buffer[512];
uint8_t shutdown_program_buffer[512];
uint8_t* bootloader_program_buffer;
uint8_t* kernel_load_backup;


uint8_t xin_base_state[100];
char xin_current_path[38] = {'\0'};
char xin_current_directory[38] = {'\0'};

char *xin_set_current_directory(char *directory)
{
    for (int i = 0; i < sizeof(xin_current_directory); i++)
        xin_current_directory[i] = '\0';

    for (int i = 0; directory[i] != '\0'; i++)
        xin_current_directory[i] = directory[i];
}

char *xin_get_current_directory(void)
{
    return xin_current_directory;
}

char *xin_get_current_path(char *file_name)
{

    for (int j = 0; j < 38; j++)
        xin_current_path[j] = '\0';

    int i = 0x0;

    for (i = 0; xin_current_directory[i] != '\0'; i++)
        xin_current_path[i] = xin_current_directory[i];

    for (int pos = 0x0; file_name[pos] != '\0'; i++, pos++)
        xin_current_path[i] = file_name[pos];

    return xin_current_path;
}
                                            

/* DIRECTORY AND FILES */
xin_entry *xin_find_entry(char *entry_name)

{

    for (char *i = (char*)XIN_ENTRY_TABLE; (uint32_t)i < XIN_ENTRY_TABLE + (SECTOR_SIZE * 8); i += 64)
    {
        if (strcmp(entry_name, i))
            return (xin_entry *)i;
    }

    entry_name = xin_get_current_path(entry_name);

    if(strlen(entry_name) > 40)
        return nullptr;

    for (char *i = (char*)XIN_ENTRY_TABLE; (uint32_t)i < XIN_ENTRY_TABLE + (SECTOR_SIZE * 8); i += 64)
    {
        if (strcmp(entry_name, i))
            return (xin_entry *)i;
    }

    return nullptr;
}

xin_entry* xin_get_file_pf(char* entry_path) // pf = parent folder/cr
{
    if(entry_path[0] == '\0')
        return nullptr;
        
    bool if_folder = false;

    if(entry_path[strlen(entry_path) - 1] == '/')
        if_folder = true;

    char parent_folder[40] = {0}; 

    int i;
    for(i = strlen(entry_path) - 1 - if_folder; entry_path[i] != '/'; i--);

    for(int j = 0; j <= i; j++)
        parent_folder[j] = entry_path[j];

    if(xin_find_entry(parent_folder) != nullptr)
        return xin_find_entry(parent_folder);   

    if(xin_find_entry(xin_get_current_path(parent_folder)) != nullptr)    
        xin_find_entry(xin_get_current_path(parent_folder)); 

    return nullptr;



}

void xin_get_file_pf_test(char* entry_path) // pf = parent folder
{
    xprintf("parent_folder: %s\n", xin_get_file_pf(entry_path)->entry_path);
    // while(getscan() != ENTER);
}

uint8_t *xin_find_free_pointer(void)
{
    for (char *i = (char *)0x800; (uint32_t)i < 0x800 + (SECTOR_SIZE * 4); i++)
    {
        if (*(char *)i == '\0')
            return (uint8_t *)i;
    }

    return nullptr;
}

xin_entry *xin_find_free_entry(void)
{
    for (char *i = (char *)XIN_ENTRY_TABLE + 64; (uint32_t)i < XIN_ENTRY_TABLE + (SECTOR_SIZE * 8); i += 64)
    {
        if (*(char *)i == '\0')
            return (xin_entry *)i;
    }

    return nullptr;
}

xin_entry *xin_folder_change(char *new_directory)
{

    if(strcmp(new_directory, ".."))
    {
        if(strcmp(xin_current_directory, "/"))
            return nullptr;
        else 
        {
            xin_current_directory[strlen(xin_current_directory) - 1] = '\0';
            int counter = strlen(xin_current_directory) - 1;
            
            while(xin_current_directory[counter] != '/')
            {
                xin_current_directory[counter] = '\0';
                counter--;
            }
        
        }
        return xin_find_entry(xin_current_directory);
    }    

    if(new_directory[strlen(new_directory) - 1] != '/')
    {
        int name_length = strlen(new_directory); // '\0' before any modification
        new_directory[name_length + 1] = '\0';
        new_directory[name_length] = '/';
    }

    xin_entry *xin_new_directory = xin_find_entry(new_directory);

    if (xin_new_directory == nullptr)
    {
        // xprintf("%zNO SUCH DIRECTORY\n", set_output_color(red, white));
        // while (KeyInfo.scan_code != ENTER);
        return nullptr;
    }

    if (new_directory[strlen(new_directory) - 1] != '/')
    {
        // xprintf("%zMISSING / ENDING CHARACTER IN DIRECTORY NAME\n", set_output_color(red, white));
        // while (KeyInfo.scan_code != ENTER);
        return nullptr;
    }

    for (int i = 0; i < sizeof(xin_current_directory); i++)
        xin_current_directory[i] = '\0';

    strcpy(xin_current_directory, xin_new_directory->entry_path);

    // xprintf("your file: %s", xin_current_directory);

    return xin_new_directory;
}

int xin_folder_create(char* entry_name)
{

    bool only_entry_name = true;
    
    xin_entry* entry = xin_find_free_entry();
    
    if(entry_name[0] == '/' && entry_name[1] == '/')
        return XIN_FILE_EXISTS;

    
    if(entry_name[strlen(entry_name) - 1] != '/')
        return XIN_BAD_FOLDER_NAME;

    entry_name[strlen(entry_name) - 1] = ' ';

    for(int i = strlen(entry_name) - 1; i >= 0; i--)
    {
        if(entry_name[i] == '/')
        {
            only_entry_name = false;
            break;
        }
    }

    entry_name[strlen(entry_name) - 1] = '/';



    if(only_entry_name)
    {

        char* path = xin_get_current_path(entry_name); 

        xprintf("%s\n", path);

        if(xin_find_entry(entry_name) != nullptr)
        {
            // xprintf("%zFILE WITH THIS NAME EXISTS\n", stderr);
            // while(getscan() != ENTER);
            return XIN_FILE_EXISTS;
        }

        strcpy(entry->entry_path, path);

    }

    else if(!only_entry_name && entry_name[0] != '/')
    {

        // return XANIN_ERROR;
        char full_path[40];
        memcpy(full_path, xin_get_current_path(entry_name), 40);

        xin_entry* entry_path = xin_find_entry(xin_get_file_pf(full_path)->entry_path);

        if(entry_path == nullptr)
            return XANIN_ERROR;

        strcpy(entry->entry_path, full_path);
    }  
    
    else if(xin_get_file_pf(entry_name) != nullptr)
    {
        if(xin_find_entry(entry_name) != nullptr)
        {
            // xprintf("%zFILE WITH THIS NAME EXISTS\n", stderr);
            // while(getscan() != ENTER);
            return XIN_FILE_EXISTS;
        }

        strcpy(entry->entry_path, entry_name);
    }
    


    else
    {
        
        // xprintf("%zFILE CREATE FAILURE\n", stderr);
        // while(getscan() != ENTER);
        return XANIN_ERROR;
    }

    /* write entry to xin entry date table */

    time_get(&SystemTime);

    entry->creation_date = (uint32_t)((SystemTime.day_of_month << 24) | (SystemTime.month << 16) | (SystemTime.century << 8) | (SystemTime.year)); 
    entry->creation_time = (uint16_t)(SystemTime.hour << 8) | (SystemTime.minutes);
    entry->modification_date = (uint32_t)((SystemTime.day_of_month << 24) | (SystemTime.month << 16) | (SystemTime.century << 8) | (SystemTime.year)); 
    entry->modification_time = (uint16_t)(SystemTime.hour << 8) | (SystemTime.minutes);
    entry->file_info = nullptr;
    entry->entry_permissions = PERMISSION_MAX;
    entry->entry_size = 0x0;
    entry->entry_type = XIN_DIRECTORY;

    return XANIN_OK;

}


void xin_file_create_at_address(char *path, uint8_t creation_date, uint8_t creation_time,
                                uint16_t os_specific, uint8_t modification_date,
                                uint8_t modification_time, uint8_t permissions,
                                uint32_t starting_sector, uint8_t entry_size, uint8_t entry_type,uint32_t entry_number)
{
    xin_entry* entry_created = (xin_entry *)((entry_number * 64) + XIN_ENTRY_TABLE);
    strcpy(entry_created->entry_path, path);

    entry_created->creation_date = creation_date;
    entry_created->creation_time = creation_time;
    entry_created->file_info = nullptr;
    entry_created->entry_type = entry_type;
    entry_created->modification_date = modification_date;
    entry_created->modification_time = modification_time;
    entry_created->entry_permissions = permissions;
    entry_created->entry_size = entry_size;
    entry_created->starting_sector = starting_sector;

    uint8_t *write_entry = (uint8_t *)(XIN_ENTRY_POINTERS + starting_sector);

    uint8_t tmp = 0;

    for (uint8_t *i = write_entry; i < (write_entry + entry_size); i++)
    {
        *i = XIN_ALLOCATED;
        tmp++;
    }

    *(write_entry + entry_size - 1) = XIN_EOF;

    
}


xin_entry *xin_init_fs(void)
{

    if(xin_find_entry("/") == nullptr)
        xin_file_create_at_address("/",                         0x0, 0x0, 0x0, 0x0, 0x0, PERMISSION_MAX, 0x0, 0x0,    XIN_DIRECTORY, 0);

    if(xin_find_entry("/ivt") == nullptr)
        xin_file_create_at_address("/ivt",                      0x0, 0x0, 0x0, 0x0, 0x0, PERMISSION_MAX, 0x0, 0x3,    XIN_FILE, 1);

    if(xin_find_entry("/file_system.bin") == nullptr)
        xin_file_create_at_address("/file_system.bin",          0x0, 0x0, 0x0, 0x0, 0x0, PERMISSION_MAX, 0x4, 20,     XIN_FILE, 2);

    if(xin_find_entry("/enter_real_mode.bin") == nullptr)
        xin_file_create_at_address("/enter_real_mode.bin",      0x0, 0x0, 0x0, 0x0, 0x0, PERMISSION_MAX, 0x1, 0x1,    XIN_FILE, 3);

    if(xin_find_entry("/boot.bin") == nullptr)
        xin_file_create_at_address("/boot.bin",                 0x0, 0x0, 0x0, 0x0, 0x0, PERMISSION_MAX, 0x3E,0x1,    XIN_FILE, 4);

    if(xin_find_entry("/tmp.bin") == nullptr)
        xin_file_create_at_address("/tmp.bin",                  0x0, 0x0, 0x0, 0x0, 0x0, PERMISSION_MAX, 0x80,0x1,    XIN_FILE, 5);

    if(xin_find_entry("/shutdown.bin") == nullptr)
        xin_file_create_at_address("/shutdown.bin",             0x0, 0x0, 0x0, 0x0, 0x0, PERMISSION_MAX, 0x2,0x1,    XIN_FILE, 6);

    if(xin_find_entry("/syscall_test.bin") == nullptr)
        xin_file_create_at_address("/syscall_test.bin",         0x0, 0x0, 0x0, 0x0, 0x0, PERMISSION_MAX, 0x500,0x1,    XIN_FILE, 7);

    if(xin_find_entry("/fast_real_mode_enter.bin") == nullptr)
        xin_file_create_at_address("/fast_real_mode_enter.bin", 0x0, 0x0, 0x0, 0x0, 0x0, PERMISSION_MAX, 0x5,0x1,    XIN_FILE, 8);

    if(xin_find_entry("/fast_real_mode_return.bin") == nullptr)
        xin_file_create_at_address("/fast_real_mode_return.bin", 0x0, 0x0, 0x0, 0x0, 0x0, PERMISSION_MAX, 0x6,0x1,    XIN_FILE, 9);
    
    if(xin_find_entry("/screenshot/") == nullptr)
        xin_folder_create("/screenshot/");

    uint32_t k = 0;

    for(char* i = (char*)0x600; i < (char*)0x600 + 0x200; i++, k++)
        enter_real_mode_buffer[k] = *i;
    
    k = 0;

    for(char* i = (char*)0x400; i < (char*)0x400 + SECTOR_SIZE; i++, k++)
        shutdown_program_buffer[k] = *i;

    xin_folder_change("/");

}

void create_file_kernel(char* entry_name)
{


    if(xin_find_entry(entry_name) != nullptr)
        return;

    /* write entry to xin entry pointers table */
    uint8_t *write_entry = xin_find_free_pointer();

    for (int i = 0; i < 15; i++)
        write_entry[i] = XIN_ALLOCATED;

    write_entry[15] = XIN_EOF;

    /* write entry to xin entry date table */
    xin_entry *entry = xin_find_free_entry();

    time_get(&SystemTime);

    strcpy(entry->entry_path, entry_name);

    entry->creation_date = (uint32_t)((SystemTime.day_of_month << 24) | (SystemTime.month << 16) | (SystemTime.century << 8) | (SystemTime.year)); 
    entry->creation_time = (uint16_t)(SystemTime.hour << 8) | (SystemTime.minutes);
    entry->modification_date = (uint32_t)((SystemTime.day_of_month << 24) | (SystemTime.month << 16) | (SystemTime.century << 8) | (SystemTime.year)); 
    entry->modification_time = (uint16_t)(SystemTime.hour << 8) | (SystemTime.minutes);
    entry->file_info = nullptr;
    entry->entry_permissions = PERMISSION_MAX;
    entry->entry_size = 0x0;
    entry->entry_type = XIN_FILE;

    entry->starting_sector = (uint32_t)write_entry - XIN_ENTRY_POINTERS;

    // disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x12, 8, (uint16_t*)0x800);

    // for(int i = 0; i < 40; i++)
    //     disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x1a + i, 1, (uint16_t*)(0x1800 + (i * SECTOR_SIZE)));

}


int xin_create_file(char* entry_name)
{

    bool only_entry_name = true;
    
    xin_entry* entry = xin_find_free_entry();

    for(int i = strlen(entry_name) - 1; i >= 0; i--)
    {
        if(entry_name[i] == '/')
        {
            only_entry_name = false;
            break;
        }
    }

    if(!only_entry_name && entry_name[0] != '/')
    {

        char* full_path = xin_get_current_path(entry_name);


        int i;
        for(i = strlen(entry_name) - 1; i >= 0; i--)
        {
            if(entry_name[i] == '/')
                break;
        }

        char tmp[40] = {0};

        for(int j = 0; j <= i; j++)
            tmp[j] = entry_name[j];

        xprintf("%s\n", xin_get_current_path(entry_name));
        strcpy(tmp, xin_get_current_path(tmp));
        xin_entry* entry_path = xin_find_entry(tmp);

        if(entry_path == nullptr)
        {
            memset((uint8_t*)entry, 0, 64); 
            return XANIN_ERROR;
        }

        strcpy(entry->entry_path, xin_get_current_path(entry_name));
    }

    else if(only_entry_name)
    {
        char* path = xin_get_current_path(entry_name); 

        // xprintf("%s\n", path);

        if(xin_find_entry(entry_name) != nullptr)
        {
            // xprintf("%zFILE WITH THIS NAME EXISTS\n", stderr);
            // while(getscan() != ENTER);
            return XIN_FILE_EXISTS;
        }

        strcpy(entry->entry_path, path);

    }
    
    else if(entry_name[0] == '/')
    {
        if(xin_find_entry(entry_name) != nullptr)
        {
            // xprintf("%zFILE WITH THIS NAME EXISTS\n", stderr);
            // while(getscan() != ENTER);
            return XIN_FILE_EXISTS;
        }

        strcpy(entry->entry_path, entry_name);
    }

    else if(xin_get_file_pf(entry_name) != nullptr)
    {
        if(xin_find_entry(entry_name) != nullptr)
        {
            // xprintf("%zFILE WITH THIS NAME EXISTS\n", stderr);
            // while(getscan() != ENTER);
            return XIN_FILE_EXISTS;
        }

        strcpy(entry->entry_path, entry_name);
    }

    else
    {
        
        // xprintf("%zFILE CREATE FAILURE\n", stderr);
        // while(getscan() != ENTER);
        return XANIN_ERROR;
    }
    /* write entry to xin entry pointers table */
    uint8_t *write_entry = xin_find_free_pointer();

    for (int i = 0; i < 15; i++)
        write_entry[i] = XIN_ALLOCATED;

    write_entry[15] = XIN_EOF;

    /* write entry to xin entry date table */

    time_get(&SystemTime);

    entry->creation_date = (uint32_t)((SystemTime.day_of_month << 24) | (SystemTime.month << 16) | (SystemTime.century << 8) | (SystemTime.year)); 
    entry->creation_time = (uint16_t)(SystemTime.hour << 8) | (SystemTime.minutes);
    entry->modification_date = (uint32_t)((SystemTime.day_of_month << 24) | (SystemTime.month << 16) | (SystemTime.century << 8) | (SystemTime.year)); 
    entry->modification_time = (uint16_t)(SystemTime.hour << 8) | (SystemTime.minutes);
    entry->file_info = nullptr;
    entry->entry_permissions = PERMISSION_MAX;
    entry->entry_size = 0x0;
    entry->entry_type = XIN_FILE;

    entry->starting_sector = (uint32_t)write_entry - XIN_ENTRY_POINTERS;

    // for(char* i = (char*)(entry->starting_sector * SECTOR_SIZE); (uint32_t)i < entry->starting_sector * SECTOR_SIZE + 0x10 * SECTOR_SIZE; i++)
    //     *i = 0x0;

    uint8_t* zeros = (uint8_t*)calloc(SECTOR_SIZE);

    for(int i = 0; i < 0x10; i++)
        disk_write(ATA_FIRST_BUS, ATA_MASTER, entry->starting_sector + i, 1, (uint16_t*)zeros);
    
    free(zeros);

    disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x12, 8, (uint16_t*)0x800);
    disk_flush(ATA_FIRST_BUS, ATA_MASTER);

    for(int i = 0; i < 40; i++)
    {
        disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x1a + i, 1, (uint16_t*)(0x1800 + (i * SECTOR_SIZE)));
        disk_flush(ATA_FIRST_BUS, ATA_MASTER);
    }

    return XANIN_OK;

}


__STATUS sys_xin_remove_entry(char *entry_name)
{

    char *entry_to_delete = (char *)xin_find_entry(entry_name);

    xin_entry *entry_data = (xin_entry *)entry_to_delete;

    if (entry_to_delete == nullptr)
    {
        // xprintf("%zNO SUCH DIRECTORY\n", set_output_color(red, white));
        // while (KeyInfo.scan_code != ENTER);
        return XIN_ENTRY_NOT_FOUND;
    }

    // xprintf("starting addr: 0x%x\n", entry_data->starting_sector + XIN_ENTRY_POINTERS);

    if (entry_data->entry_type == XIN_FILE)
    {
        for (char *i = (char *)entry_data->starting_sector + XIN_ENTRY_POINTERS;
                (uint32_t)i < entry_data->starting_sector + XIN_ENTRY_POINTERS + 16; i++)
            *i = XIN_UNALLOCATED;
        
    }

    for (int i = 0; i < sizeof(xin_entry); i++)
        entry_to_delete[i] = '\0';

    return XANIN_OK;


}

uint32_t xin_get_start_sector(char *entry_name)
{
    xin_entry *xin_file_descriptor = xin_find_entry(entry_name);

    return xin_file_descriptor->starting_sector;
}

size_t read(xin_entry *entry, void *buf, size_t count)
{

    char* end = (char *)(entry->file_info->base_address_memory + count + entry->file_info->position);
    char* begin = (char *)(entry->file_info->base_address_memory + entry->file_info->position);

    uint32_t sectors_to_load = count / SECTOR_SIZE;

    if(count % SECTOR_SIZE != 0)
        sectors_to_load++;

    for(int i = 0; i < sectors_to_load; i++)
        disk_read(ATA_FIRST_BUS, ATA_MASTER, entry->starting_sector + i, 1, (uint16_t*)(entry->file_info->base_address_memory + (i * SECTOR_SIZE)));

    for (char *i = begin; i < end; i++, buf++)
    {
        *(char *)buf = *i;
        entry->file_info->position++;
    }
}

size_t write(xin_entry *entry, void *buf, size_t count)
{

    // char *end = (char *)(entry->starting_sector * SECTOR_SIZE) + count + entry->file_info->position;
    char* end = (char *)(entry->file_info->base_address_memory + count + entry->file_info->position);

    uint32_t tmp = entry->file_info->position;

    // for (char *i = (char *)(entry->starting_sector * SECTOR_SIZE) + tmp; i < end; i++, buf++)
    for (char *i = (char *)(entry->file_info->base_address_memory) + tmp; i < end; i++, buf++)
    {
        *i = *(char *)buf;
        entry->file_info->position++;
    }

    time_get(&SystemTime);

    entry->modification_date = (uint32_t)((SystemTime.day_of_month << 24) | (SystemTime.month << 16) | (SystemTime.century << 8) | (SystemTime.year)); 
    entry->modification_time = (uint16_t)(SystemTime.hour << 8) | (SystemTime.minutes);

}

void fseek(xin_entry *file, uint32_t new_position)
{
    file->file_info->position = new_position;
}

uint32_t ftell(xin_entry* file)
{
    return file->file_info->position;
}

xin_entry *fopen(char *file_path, char *mode)
{


    xin_entry* file = xin_find_entry(file_path);

    if(strcmp(mode, "a"))
    {
        if(file->file_info == nullptr)
            file->file_info = (file_information_block*)calloc(sizeof(file_information_block));

        file->file_info->base_address_memory = (uint8_t*)calloc(SECTOR_SIZE * 0x10);
        strcpy(file->file_info->rights, mode);
        file->file_info->position = file->entry_size;

        uint32_t sectors_to_load = file->entry_size / SECTOR_SIZE;

        if(file->entry_size % SECTOR_SIZE != 0)
            sectors_to_load++;

        for(int i = 0; i < sectors_to_load; i++)
            disk_read(ATA_FIRST_BUS, ATA_MASTER, file->starting_sector + i, 1, (uint16_t*)(file->file_info->base_address_memory + (i * SECTOR_SIZE)));

        
        return file;
    }



    else if (file != nullptr && file->entry_type != XIN_DIRECTORY && file->entry_path[0] != '\0')
    {
        

        if(file->file_info == nullptr)
            file->file_info = (file_information_block*)calloc(sizeof(file_information_block));
        
        file->file_info->base_address_memory = (uint8_t*)calloc(SECTOR_SIZE * 0x10);
        strcpy(file->file_info->rights, mode);

        file->file_info->position = 0;
        return file;
    }

    else if(strcmp(mode, "rw") | strcmp(mode, "w"))
    {
        file = create(file_path);

        file->file_info = (file_information_block*)calloc(sizeof(file_information_block));
        strcpy(file->file_info->rights, mode);
        file->file_info->base_address_memory = (uint8_t*)calloc(SECTOR_SIZE * 0x10);
        
        file->file_info->position = 0;
        return file;
    }



    return nullptr;
}


void fclose(xin_entry** file)
{
    
    (*file)->entry_size = (*file)->file_info->position;
    (*file)->file_info->position = 0;
    memset((*file)->file_info->rights, '\0', 2);

//    for(int i = 0; i < (*file)->entry_size / 512 + ((*file)->entry_size % 512 != 0 ? 1 : 0); i++)
    for(int i = 0; i < 0x10; i++)
       disk_write(ATA_FIRST_BUS, ATA_MASTER, (*file)->starting_sector + i, 1, (uint16_t*)((*file)->file_info->base_address_memory + (i * SECTOR_SIZE)));

    // while(1);
    free((*file)->file_info->base_address_memory);
    *file = nullptr;

}



xin_entry* create(char* file_name)
{

    char* entry_full_name;

    entry_full_name = xin_get_current_path(file_name);

    /* write entry to xin entry pointers table */
    uint8_t *write_entry = xin_find_free_pointer();

    for (int i = 0; i < 15; i++)
        write_entry[i] = XIN_ALLOCATED;

    write_entry[15] = XIN_EOF;

    /* write entry to xin entry date table */
    xin_entry *entry = xin_find_free_entry();

    time_get(&SystemTime);

    strcpy(entry->entry_path, entry_full_name);

    entry->creation_date = (uint32_t)((SystemTime.day_of_month << 24) | (SystemTime.month << 16) | (SystemTime.century << 8) | (SystemTime.year)); 
    entry->creation_time = (uint16_t)(SystemTime.hour << 8) | (SystemTime.minutes);
    entry->modification_date = (uint32_t)((SystemTime.day_of_month << 24) | (SystemTime.month << 16) | (SystemTime.century << 8) | (SystemTime.year)); 
    entry->modification_time = (uint16_t)(SystemTime.hour << 8) | (SystemTime.minutes);

    entry->entry_permissions = PERMISSION_MAX;
    entry->entry_size = 0x0;
    entry->entry_type = XIN_FILE;
    entry->file_info = nullptr;
    entry->starting_sector = (uint32_t)write_entry - XIN_ENTRY_POINTERS;

    uint8_t* zero_mregion = (uint8_t*)malloc(512);
    memset(zero_mregion, 0x0, 512);

    for(int i = 0; i < 16; i++)
        disk_write(ATA_FIRST_BUS, ATA_MASTER, entry->starting_sector, i, (uint16_t*)zero_mregion);

}

int open(char* file_path, uint32_t options)
{

    xin_entry* file = xin_find_entry(file_path);


    if (file != nullptr && file->entry_type != XIN_DIRECTORY && file->entry_path[0] != '\0')
    {

        if(file->file_info == nullptr)
            file->file_info = (file_information_block*)calloc(sizeof(file_information_block));

        file->file_info->position = 0;
        int fd = (int)((uint32_t)file - XIN_ENTRY_TABLE) / 64;
        return fd;
    }
    return -1;
}

char* getline_from_ptr(char* data, int line_id)
{

    char* file_data = (char*)(data);
    char* line = (char*)calloc(200);

    int column = 0;
    int current_line = 0;
    int file_offset = 0;

    while(current_line < line_id)
    {
        memset(line, 0, 200);

        while(file_data[file_offset] != '\n')
        {
            if(file_data[file_offset] == '\0')
            {                
                if(current_line + 1 == line_id)
                {
                    line[column] = file_data[file_offset];
                    column++;
                    file_offset++;               
                    break;
                }

                else
                    return nullptr;
                break;
            }

            line[column] = file_data[file_offset];
            column++;
            file_offset++;
        }
        
        line[column] = '\0';
        file_offset++;

        column = 0;
        current_line++;
    }

    return line;

}


char* getline(xin_entry* file, int line_id)
{

    char* file_data = (char*)(file->file_info->base_address_memory);
    char* line = (char*)calloc(200);

    int column = 0;
    int current_line = 0;
    int file_offset = 0;    

    while(current_line < line_id)
    {
        memset(line, 0, 200);

        while(file_data[file_offset] != '\n')
        {
            if(file_data[file_offset] == '\0')
            {                
                if(current_line + 1 == line_id)
                {
                    line[column] = file_data[file_offset];
                    column++;
                    file_offset++;               
                    break;
                }

                else
                    return nullptr;
                break;
            }

            line[column] = file_data[file_offset];
            column++;
            file_offset++;
        }
        
        line[column] = '\0';
        file_offset++;

        column = 0;
        current_line++;
    }

    return line;



}

__STATUS remove_directory(char* folder_name)
{


    xin_entry* folder = xin_find_entry(folder_name);

    if(folder == nullptr)
    {
        xprintf("%zNO SUCH DIRECTORY\n", set_output_color(red, white));
        return XIN_ENTRY_NOT_FOUND;
    }

    char name[40];
    uint32_t name_length;

    memcpy(name, folder->entry_path, 40); 
    name_length = strlen(name) - 1;

    for(xin_entry* i = (xin_entry*)XIN_ENTRY_TABLE; i < (xin_entry*)(XIN_ENTRY_TABLE + SECTOR_SIZE * 4); i++)
    {

        if(strncmp(name, i -> entry_path, name_length))
        {
            char* tmp = (char*)i;
            for(char* j = (char*)i; j < tmp + 64; j++)
            {
                *j = '\0';
            }
        }


    }


}

