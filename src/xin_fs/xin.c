

#include <libc/syslog.h>
#include <libc/hal.h>
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

XinFileDescriptor* FileDescriptorTable;


uint8_t xin_base_state[100];
char xin_current_path[38] = {'\0'};
char xin_current_directory[38] = {'\0'};

char* xin_set_current_directory(char *directory)
{
    for (int i = 0; i < sizeof(xin_current_directory); i++)
        xin_current_directory[i] = '\0';

    for (int i = 0; directory[i] != '\0'; i++)
        xin_current_directory[i] = directory[i];

    return xin_current_directory;
}

void xin_get_current_directory(char* buf)
{
    memcpy((uint8_t*)buf, (uint8_t*)xin_current_directory, MAX_PATH);
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


uint8_t *xin_find_free_pointer(void)
{
    for (char *i = (char *)XIN_ENTRY_POINTERS; (uint32_t)i < XIN_ENTRY_POINTERS+ (SECTOR_SIZE * 8); i++)
    {
        if (*i == XIN_UNALLOCATED)
            return (uint8_t *)i;
    }

    return NULL;
}

uint8_t *xin_find_free_pointer_with_given_size(uint32_t size)
{

    if(!size)
        return NULL;

    for (uint8_t*i = (uint8_t*)XIN_ENTRY_POINTERS + SECTOR_SIZE; (uint32_t)i < XIN_ENTRY_POINTERS + (SECTOR_SIZE * 8); i++)
    {
        if (*i == XIN_UNALLOCATED)
        {
            bool ok = true;
            for(int j = 0; j < size; j++)
            {
                if(i[j] != XIN_UNALLOCATED) 
                {
                    ok = false;
                    break;
                }
            }

            if(ok)
                return i;
        }
    }
    return NULL;
}

                                            
void xin_entry_resize(XinEntry* entry, uint32_t new_size)
{
    uint8_t* xin_pointer_table_entry = (uint8_t*)(XIN_ENTRY_POINTERS + entry->first_sector);
    uint32_t sectors = entry->size / SECTOR_SIZE + (entry->size % SECTOR_SIZE != 0 ? 1 : 0);

    for(int i = 0; i < sectors; i++)
        xin_pointer_table_entry[i] = XIN_UNALLOCATED;

    sectors = new_size / SECTOR_SIZE + (new_size % SECTOR_SIZE != 0 ? 1 : 0);
    xin_pointer_table_entry = xin_find_free_pointer_with_given_size(sectors);

    for(int i = 0; i < sectors; i++)
        xin_pointer_table_entry[i] = XIN_ALLOCATED;

}

/* DIRECTORY AND FILES */
XinEntry *xin_find_entry(char *entry_name)
{
    if(!strlen(entry_name))
        return NULL;

    for (char *i = (char*)XIN_ENTRY_TABLE; (uint32_t)i < XIN_ENTRY_TABLE + (SECTOR_SIZE * 50); i += 64)
    {
        if (strcmp(entry_name, i))
            return (XinEntry *)i;
    }

    entry_name = xin_get_current_path(entry_name);

    if(strlen(entry_name) > 40)
        return NULL;

    for (char *i = (char*)XIN_ENTRY_TABLE; (uint32_t)i < XIN_ENTRY_TABLE + (SECTOR_SIZE * 50); i += 64)
    {
        if (strcmp(entry_name, i))
            return (XinEntry *)i;
    }

    return NULL;
}

XinEntry* xin_get_file_pf(char* path) // pf = parent folder/cr
{
    if(path[0] == '\0')
        return NULL;
        
    bool if_folder = false;

    if(path[strlen(path) - 1] == '/')
        if_folder = true;

    char parent_folder[40] = {0}; 

    int i;
    for(i = strlen(path) - 1 - if_folder; path[i] != '/'; i--);

    for(int j = 0; j <= i; j++)
        parent_folder[j] = path[j];

    if(xin_find_entry(parent_folder) != NULL)
        return xin_find_entry(parent_folder);   

    // if(xin_find_entry(xin_get_current_path(parent_folder)) != NULL)    
    //     return xin_find_entry(xin_get_current_path(parent_folder)); 

    return NULL;

}

void xin_get_file_pf_test(char* path) // pf = parent folder
{
    xprintf("parent_folder: %s\n", xin_get_file_pf(path)->path);
    // while(getscan() != ENTER);
}


XinEntry *xin_find_free_entry(void)
{
    for (char *i = (char *)XIN_ENTRY_TABLE + 64; (uint32_t)i < XIN_ENTRY_TABLE + (SECTOR_SIZE * 50); i += 64)
    {
        if (*(char *)i == '\0')
            return (XinEntry *)i;
    }

    return NULL;
}

XinEntry *xin_folder_change(char *new_directory)
{

    char* tmp = (char*)calloc(MAX_PATH);
    strcpy(tmp, new_directory);
    new_directory = tmp;

    if(strcmp(new_directory, ".."))
    {
        if(strcmp(xin_current_directory, "/"))
            return NULL;
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

    XinEntry *xin_new_directory = xin_find_entry(new_directory);

    if (xin_new_directory == NULL)
    {
        // xprintf("%zNO SUCH DIRECTORY\n", set_output_color(red, white));
        // while (KeyInfo.scan_code != ENTER);
        free(new_directory);
        return NULL;
    }

    else if(xin_new_directory->type != XIN_DIRECTORY)
    {
        free(new_directory);
        return NULL;
    }

    else if (new_directory[strlen(new_directory) - 1] != '/')
    {
        // xprintf("%zMISSING / ENDING CHARACTER IN DIRECTORY NAME\n", set_output_color(red, white));
        // while (KeyInfo.scan_code != ENTER);
        free(new_directory);
        return NULL;
    }

    for (int i = 0; i < sizeof(xin_current_directory); i++)
        xin_current_directory[i] = '\0';

    strcpy(xin_current_directory, xin_new_directory->path);

    // xprintf("your file: %s", xin_current_directory);

    free(new_directory);
    return xin_new_directory;
}

int xin_folder_create(char* entry_name)
{

    bool only_entry_name = true;
    
    XinEntry* entry = xin_find_free_entry();
    
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

        if(xin_find_entry(entry_name) != NULL)
        {
            // xprintf("%zFILE WITH THIS NAME EXISTS\n", stderr);
            // while(getscan() != ENTER);
            return XIN_FILE_EXISTS;
        }

        strcpy(entry->path, path);

    }

    else if(!only_entry_name && entry_name[0] != '/')
    {

        // return XANIN_ERROR;
        char full_path[40];
        memcpy(full_path, xin_get_current_path(entry_name), 40);

        XinEntry* path = xin_find_entry(xin_get_file_pf(full_path)->path);

        if(path == NULL)
            return XANIN_ERROR;

        strcpy(entry->path, full_path);
    }  
    
    else if(xin_get_file_pf(entry_name) != NULL)
    {
        if(xin_find_entry(entry_name) != NULL)
        {
            // xprintf("%zFILE WITH THIS NAME EXISTS\n", stderr);
            // while(getscan() != ENTER);
            return XIN_FILE_EXISTS;
        }

        strcpy(entry->path, entry_name);
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
    entry->FileInfo = NULL;
    entry->permissions = PERMISSION_MAX;
    entry->size = 0x0;
    entry->type = XIN_DIRECTORY;

    disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x12, 5, (uint16_t*)(0x800));
    disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x1a, 10, (uint16_t*)(0x1800));

    return XANIN_OK;

}


void xin_file_create_at_address(char *path, uint8_t creation_date, uint8_t creation_time,
                                uint16_t os_specific, uint8_t modification_date,
                                uint8_t modification_time, uint8_t permissions,
                                uint32_t first_sector, uint8_t size, uint8_t type,uint32_t entry_number)
{
    XinEntry* entry_created = (XinEntry *)((entry_number * 64) + XIN_ENTRY_TABLE);
    strcpy(entry_created->path, path);

    entry_created->creation_date = creation_date;
    entry_created->creation_time = creation_time;
    entry_created->FileInfo = NULL;
    entry_created->type = type;
    entry_created->modification_date = modification_date;
    entry_created->modification_time = modification_time;
    entry_created->permissions = permissions;
    entry_created->size = size * SECTOR_SIZE;
    entry_created->first_sector = first_sector;

    uint8_t *write_entry = (uint8_t *)(XIN_ENTRY_POINTERS + first_sector);

    uint8_t tmp = 0;

    for (uint8_t *i = write_entry; i < (write_entry + size); i++)
    {
        *i = XIN_ALLOCATED;
        tmp++;
    }

    *(write_entry + size - 1) = XIN_EOF;

    
}


void xin_init_fs(void)
{

    if(xin_find_entry("/") == NULL)
        xin_file_create_at_address("/",                         0x0, 0x0, 0x0, 0x0, 0x0, PERMISSION_MAX, 0x0, 0x0,    XIN_DIRECTORY, 0);

    if(xin_find_entry("/ivt") == NULL)
        xin_file_create_at_address("/ivt",                      0x0, 0x0, 0x0, 0x0, 0x0, PERMISSION_MAX, 0x0, 0x3,    XIN_FILE, 1);

    if(xin_find_entry("/file_system.bin") == NULL)
        xin_file_create_at_address("/file_system.bin",          0x0, 0x0, 0x0, 0x0, 0x0, PERMISSION_MAX, 0x4, 20,     XIN_FILE, 2);

    if(xin_find_entry("/enter_real_mode.bin") == NULL)
        xin_file_create_at_address("/enter_real_mode.bin",      0x0, 0x0, 0x0, 0x0, 0x0, PERMISSION_MAX, 0x1, 0x1,    XIN_FILE, 3);

    if(xin_find_entry("/boot.bin") == NULL)
        xin_file_create_at_address("/boot.bin",                 0x0, 0x0, 0x0, 0x0, 0x0, PERMISSION_MAX, 0x3E,0x1,    XIN_FILE, 4);

    if(xin_find_entry("/tmp.bin") == NULL)
        xin_file_create_at_address("/tmp.bin",                  0x0, 0x0, 0x0, 0x0, 0x0, PERMISSION_MAX, 0x80,0x1,    XIN_FILE, 5);

    if(xin_find_entry("/shutdown.bin") == NULL)
        xin_file_create_at_address("/shutdown.bin",             0x0, 0x0, 0x0, 0x0, 0x0, PERMISSION_MAX, 0x2,0x1,    XIN_FILE, 6);

    if(xin_find_entry("/syscall_test.bin") == NULL)
        xin_file_create_at_address("/syscall_test.bin",         0x0, 0x0, 0x0, 0x0, 0x0, PERMISSION_MAX, 0x500,0x1,    XIN_FILE, 7);

    if(xin_find_entry("/fast_real_mode_enter.bin") == NULL)
        xin_file_create_at_address("/fast_real_mode_enter.bin", 0x0, 0x0, 0x0, 0x0, 0x0, PERMISSION_MAX, 0x5,0x1,    XIN_FILE, 8);

    if(xin_find_entry("/fast_real_mode_return.bin") == NULL)
        xin_file_create_at_address("/fast_real_mode_return.bin", 0x0, 0x0, 0x0, 0x0, 0x0, PERMISSION_MAX, 0x6,0x1,    XIN_FILE, 9);
    
    if(xin_find_entry("/screenshot/") == NULL)
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


    if(xin_find_entry(entry_name) != NULL)
        return;

    /* write entry to xin entry pointers table */
    uint8_t *write_entry = xin_find_free_pointer();

    for (int i = 0; i < 15; i++)
        write_entry[i] = XIN_ALLOCATED;

    write_entry[15] = XIN_EOF;

    /* write entry to xin entry date table */
    XinEntry *entry = xin_find_free_entry();

    time_get(&SystemTime);

    strcpy(entry->path, entry_name);

    entry->creation_date = (uint32_t)((SystemTime.day_of_month << 24) | (SystemTime.month << 16) | (SystemTime.century << 8) | (SystemTime.year)); 
    entry->creation_time = (uint16_t)(SystemTime.hour << 8) | (SystemTime.minutes);
    entry->modification_date = (uint32_t)((SystemTime.day_of_month << 24) | (SystemTime.month << 16) | (SystemTime.century << 8) | (SystemTime.year)); 
    entry->modification_time = (uint16_t)(SystemTime.hour << 8) | (SystemTime.minutes);
    entry->FileInfo = NULL;
    entry->permissions = PERMISSION_MAX;
    entry->size = 0x0;
    entry->type = XIN_FILE;

    entry->first_sector = (uint32_t)write_entry - XIN_ENTRY_POINTERS;

    // disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x12, 8, (uint16_t*)0x800);

    // for(int i = 0; i < 40; i++)
    //     disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x1a + i, 1, (uint16_t*)(0x1800 + (i * SECTOR_SIZE)));

}


int xin_file_create(char* entry_name)
{

    bool only_entry_name = true;
    
    XinEntry* entry = xin_find_free_entry();

    if(entry == NULL)
        return XANIN_ERROR;

    if(xin_find_entry(entry_name) != NULL)
        return XIN_FILE_EXISTS;

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

        // xprintf("%s\n", xin_get_current_path(entry_name));
        strcpy(tmp, xin_get_current_path(tmp));
        XinEntry* path = xin_find_entry(tmp);

        if(path == NULL)
        {
            memset((uint8_t*)entry, 0, 64); 
            return XANIN_ERROR;
        }

        strcpy(entry->path, xin_get_current_path(entry_name));
    }

    else if(only_entry_name)
    {
        char* path = xin_get_current_path(entry_name); 

        // xprintf("%s\n", path);

        if(xin_find_entry(entry_name) != NULL)
        {
            // xprintf("%zFILE WITH THIS NAME EXISTS\n", stderr);
            // while(getscan() != ENTER);
            return XIN_FILE_EXISTS;
        }

        strcpy(entry->path, path);

    }
    
    else if(entry_name[0] == '/')
    {
        if(xin_find_entry(entry_name) != NULL)
        {
            // xprintf("%zFILE WITH THIS NAME EXISTS\n", stderr);
            // while(getscan() != ENTER);
            return XIN_FILE_EXISTS;
        }

        strcpy(entry->path, entry_name);
    }

    else if(xin_get_file_pf(entry_name) != NULL)
    {
        if(xin_find_entry(entry_name) != NULL)
        {
            // xprintf("%zFILE WITH THIS NAME EXISTS\n", stderr);
            // while(getscan() != ENTER);
            return XIN_FILE_EXISTS;
        }

        strcpy(entry->path, entry_name);
    }

    else
    {
        
        // xprintf("%zFILE CREATE FAILURE\n", stderr);
        // while(getscan() != ENTER);
        return XANIN_ERROR;
    }
    /* write entry to xin entry pointers table */
    uint8_t *write_entry = xin_find_free_pointer();//_with_given_size(2);

    // for (int i = 0; i < 15; i++)
    //     write_entry[i] = XIN_ALLOCATED;

    write_entry[0] = XIN_EOF;
    // write_entry[15] = XIN_EOF;

    /* write entry to xin entry date table */

    time_get(&SystemTime);

    entry->creation_date = (uint32_t)((SystemTime.day_of_month << 24) | (SystemTime.month << 16) | (SystemTime.century << 8) | (SystemTime.year)); 
    entry->creation_time = (uint16_t)(SystemTime.hour << 8) | (SystemTime.minutes);
    entry->modification_date = (uint32_t)((SystemTime.day_of_month << 24) | (SystemTime.month << 16) | (SystemTime.century << 8) | (SystemTime.year)); 
    entry->modification_time = (uint16_t)(SystemTime.hour << 8) | (SystemTime.minutes);
    entry->FileInfo = NULL;
    entry->permissions = PERMISSION_MAX;
    entry->size = 0;
    entry->type = XIN_FILE;

    entry->first_sector = (uint32_t)write_entry - XIN_ENTRY_POINTERS;

    // for(char* i = (char*)(entry->first_sector * SECTOR_SIZE); (uint32_t)i < entry->first_sector * SECTOR_SIZE + 0x10 * SECTOR_SIZE; i++)
    //     *i = 0x0;

    uint8_t* zeros = (uint8_t*)calloc(SECTOR_SIZE);

    disk_write(ATA_FIRST_BUS, ATA_MASTER, entry->first_sector, 2, (uint16_t*)zeros);
    
    free(zeros);

    disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x12, 8, (uint16_t*)0x800);
    disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x1a, 40, (uint16_t*)(0x1800));

    return XANIN_OK;

}

int xin_file_reallocate_with_given_size(XinEntry* File, uint32_t size, uint8_t* buf)
{


    if(File == NULL)
        return XANIN_ERROR;

    if(File->size >= size)
        return XANIN_OK;

    uint32_t number_of_sectors_to_deallocate = int_to_sectors(File->size);
    if(!number_of_sectors_to_deallocate)
        number_of_sectors_to_deallocate++;

    uint32_t number_of_sectors_to_allocate = int_to_sectors(size);
    if(!number_of_sectors_to_allocate)
        number_of_sectors_to_allocate++;


    uint8_t* entry_deallocate = (uint8_t*)(File->first_sector + XIN_ENTRY_POINTERS);
    for(int i = 0; i < number_of_sectors_to_deallocate; i++)
        entry_deallocate[i] = XIN_UNALLOCATED;
        

    /* write entry to xin entry pointers table */
    uint8_t* write_entry = xin_find_free_pointer_with_given_size(number_of_sectors_to_allocate);

    for (int i = 0; i < number_of_sectors_to_allocate-1; i++)
        write_entry[i] = XIN_ALLOCATED;

    write_entry[number_of_sectors_to_allocate-1] = XIN_EOF;

    /* write entry to xin entry data table */

    time_get(&SystemTime);

    File->creation_date = (uint32_t)((SystemTime.day_of_month << 24) | (SystemTime.month << 16) | (SystemTime.century << 8) | (SystemTime.year)); 
    File->creation_time = (uint16_t)(SystemTime.hour << 8) | (SystemTime.minutes);
    File->modification_date = (uint32_t)((SystemTime.day_of_month << 24) | (SystemTime.month << 16) | (SystemTime.century << 8) | (SystemTime.year)); 
    File->modification_time = (uint16_t)(SystemTime.hour << 8) | (SystemTime.minutes);
    File->FileInfo = NULL;
    File->permissions = PERMISSION_MAX;
    File->size = size;
    File->type = XIN_FILE;
    File->first_sector = (uint32_t)write_entry - XIN_ENTRY_POINTERS;

    disk_write(ATA_FIRST_BUS, ATA_MASTER, File->first_sector, number_of_sectors_to_allocate, (uint16_t*)buf);
    
    disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x12, 8, (uint16_t*)0x800);
    disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x1a, 40, (uint16_t*)(0x1800));

    return XANIN_OK;
}


__STATUS __sys_xin_entry_remove(char *entry_name)
{

    XinEntry* entry_data = xin_find_entry(entry_name);
    uint32_t sectors_used = int_to_sectors(entry_data->size);

    if(!sectors_used)
        sectors_used++;

    if (entry_data == NULL)
        return XIN_ENTRY_NOT_FOUND;

    else if (entry_data->type == XIN_FILE)
    {
        for (char *i = (char *)entry_data->first_sector + XIN_ENTRY_POINTERS;
                (uint32_t)i < entry_data->first_sector + XIN_ENTRY_POINTERS + sectors_used; i++)
            *i = XIN_UNALLOCATED;
        
        char* tmp = (char*)entry_data;

        for (int i = 0; i < 64; i++)
            tmp[i] = '\0';
    }

    return XANIN_OK;

}

uint32_t xin_get_start_sector(char *entry_name)
{
    XinEntry *xin_file_descriptor = xin_find_entry(entry_name);

    return xin_file_descriptor->first_sector;
}

size_t fread(XinEntry *entry, void *buf, size_t count)
{

    //////////////////VALIDATION///////////////////

    if(entry == NULL)
        return 0;
    
    if((entry->type != XIN_FILE) && (entry->type != XIN_HARD_LINK))
        return 0;

    uint32_t initial_position = ftell(entry);


    if(initial_position >= (entry->size - 1)) //last valid byte
        return 0;

    if(initial_position + count > entry->size)
        count = initial_position + count - (entry->size + 1); //0 is start index    
    
    //////////////////////////////////////////////////

    char* begin = (char *)(entry->FileInfo->buffer + ftell(entry));
    char* end = (char *)(entry->FileInfo->buffer + ftell(entry) + count);

    uint32_t sectors_to_load = (count + ftell(entry)) / SECTOR_SIZE;

    if((count + entry->FileInfo->position) % SECTOR_SIZE != 0)
        sectors_to_load++;

    for(int i = 0; i < sectors_to_load; i++)
    {
        if(!entry->FileInfo->sector_in_use[i])
        {
            disk_read(ATA_FIRST_BUS, ATA_MASTER, entry->first_sector + i, 1, (uint16_t*)(entry->FileInfo->buffer + (i * SECTOR_SIZE)));
            entry->FileInfo->sector_in_use[i] = true;
        }
    }

    for (char *i = begin; i < end; i++, buf++)
        *(char *)buf = *i;

    fseek(entry, initial_position + count);

    return count;

}

size_t read(int fd, void *buf, size_t count)
{

    //////////////////VALIDATION///////////////////

    if(fd < 0)
        return 0;

    XinEntry* entry = (XinEntry*)(XIN_ENTRY_TABLE + (fd * XIN_size));

    if((entry->type != XIN_FILE) && (entry->type != XIN_HARD_LINK))
        return 0;

    uint32_t initial_position = ftell(entry);

    if(initial_position >= (entry->size - 1)) //last valid byte
        return 0;

    if(initial_position + count > entry->size)
        count = initial_position + count - (entry->size + 1); //0 is start index

    //////////////////////////////////////////////////

    char* begin = (char *)(entry->FileInfo->buffer + ftell(entry));
    char* end = (char *)(entry->FileInfo->buffer + count + ftell(entry));

    uint32_t sectors_to_load = (count + ftell(entry)) / SECTOR_SIZE;

    if((count + entry->FileInfo->position) % SECTOR_SIZE != 0)
        sectors_to_load++;

    for(int i = 0; i < sectors_to_load; i++)
    {
        if(!entry->FileInfo->sector_in_use[i])
        {
            disk_read(ATA_FIRST_BUS, ATA_MASTER, entry->first_sector + i, 1, (uint16_t*)(entry->FileInfo->buffer + (i * SECTOR_SIZE)));
            entry->FileInfo->sector_in_use[i] = true;
        }
    }

    for (char *i = begin; i < end; i++, buf++)
        *(char *)buf = *i;

    fseek(entry, initial_position + count);
    return count;
}

size_t fwrite(XinEntry *entry, void *buf, size_t count)
{
    //////////////////VALIDATION///////////////////

    if(entry == NULL)
        return 0;

    if((entry->type != XIN_FILE) && (entry->type != XIN_HARD_LINK))
        return 0;

    //////////////////////////////////////////////////

    char* end = (char *)(entry->FileInfo->buffer + count + ftell(entry));
    uint32_t initial_position = ftell(entry);

    if(initial_position + count > entry->FileInfo->tmp_size)
        entry->FileInfo->tmp_size = initial_position + count;


    uint32_t sectors_count = (count / SECTOR_SIZE) + (count % SECTOR_SIZE != 0 ? 1 : 0);
    
    for(int i = 0; i < sectors_count; i++)
    {
        if(!entry->FileInfo->sector_in_use[i])
        {
            uint8_t* tmp = (uint8_t*)calloc(SECTOR_SIZE);
            fread(entry, tmp, SECTOR_SIZE);
            entry->FileInfo->sector_in_use[i] = true;
            free(tmp);
        }
    }

    fseek(entry, initial_position);

    for (char *i = (char *)(entry->FileInfo->buffer) + initial_position; i < end; i++, buf++)
    {
        *i = *(char *)buf;
        fseek(entry, ftell(entry) + 1);
    }

    time_get(&SystemTime);

    entry->modification_date = (uint32_t)((SystemTime.day_of_month << 24) | (SystemTime.month << 16) | (SystemTime.century << 8) | (SystemTime.year)); 
    entry->modification_time = (uint16_t)(SystemTime.hour << 8) | (SystemTime.minutes);
    return count;
}

size_t write(int fd, void* buf, size_t count)
{

    ////////////////////VALIDATION/////////////////////
    
    if(fd < 0)
        return 0;

    if(!FileDescriptorTable[fd].is_used)
        return 0;

    XinEntry* entry = (XinEntry*)(XIN_ENTRY_TABLE + (fd * XIN_size));
    
    if((entry->type != XIN_FILE) && (entry->type != XIN_HARD_LINK))
        return 0;

    //////////////////////////////////////////////////

    // char *end = (char *)(entry->first_sector * SECTOR_SIZE) + count + entry->FileInfo->position;
    char* end = (char *)(entry->FileInfo->buffer + count + entry->FileInfo->position);

    uint32_t initial_position = entry->FileInfo->position;
    
    if(initial_position + count > entry->FileInfo->tmp_size)
        entry->FileInfo->tmp_size = initial_position + count;

    uint32_t sectors_count = (count / SECTOR_SIZE) + (count % SECTOR_SIZE != 0 ? 1 : 0);
    
    for(int i = 0; i < sectors_count; i++)
    {
        if(!entry->FileInfo->sector_in_use[i])
        {
            uint8_t* tmp = (uint8_t*)calloc(SECTOR_SIZE);
            fread(entry, tmp, SECTOR_SIZE);
            entry->FileInfo->sector_in_use[i] = true;
            free(tmp);
        }
    }

    uint32_t tmp = entry->FileInfo->position;
    for (char *i = (char *)(entry->FileInfo->buffer) + tmp; i < end; i++, buf++)
    {
        *i = *(char *)buf;
        entry->FileInfo->position++;
    }

    time_get(&SystemTime);

    entry->modification_date = (uint32_t)((SystemTime.day_of_month << 24) | (SystemTime.month << 16) | (SystemTime.century << 8) | (SystemTime.year)); 
    entry->modification_time = (uint16_t)(SystemTime.hour << 8) | (SystemTime.minutes);

    return count;

}

void fseek(XinEntry *file, uint32_t new_position)
{
    file->FileInfo->position = new_position;
}

void lseek(int fd, uint32_t new_position)
{
    if(!FileDescriptorTable[fd].is_used)
        return;
    FileDescriptorTable[fd].entry->FileInfo->position = new_position;
}

const uint32_t ftell(XinEntry* file)
{
    return file->FileInfo->position;
}

const uint32_t lteel(int fd)
{
    if(!FileDescriptorTable[fd].is_used)
        return -1;

    else
        return ftell(FileDescriptorTable[fd].entry);
}

XinEntry *fopen(char *file_path, char *mode)
{

    XinEntry* file = xin_find_entry(file_path);

    if(file != NULL)
    {
        file->FileInfo = (FileInformationBlock*)calloc(sizeof(FileInformationBlock));
        file->FileInfo->buffer = (uint8_t*)calloc(SECTOR_SIZE * 0x10);
        file->FileInfo->sector_in_use = (bool*)calloc(0x10 * sizeof(bool));
        strcpy(file->FileInfo->rights, mode);
        file->FileInfo->position = file->size;
    }

    if(strcmp(mode, "a"))
    {

        uint32_t sectors_to_load = file->size / SECTOR_SIZE;

        if(file->size % SECTOR_SIZE != 0)
            sectors_to_load++;

        for(int i = 0; i < sectors_to_load; i++)
            disk_read(ATA_FIRST_BUS, ATA_MASTER, file->first_sector + i, 1, (uint16_t*)(file->FileInfo->buffer + (i * SECTOR_SIZE)));

        return file;
    }

    else if (file != NULL && file->type != XIN_DIRECTORY && file->path[0] != '\0')
    {

        strcpy(file->FileInfo->rights, mode);
        file->FileInfo->position = 0;
        return file;
    }

    else if(strcmp(mode, "rw") || strcmp(mode, "w"))
    {
        int status = xin_file_create(file_path);

        if(status == XANIN_OK)
        {
            file = xin_find_entry(file_path);
            strcpy(file->FileInfo->rights, mode);
            file->FileInfo->sector_in_use = (bool*)calloc(0x10 * sizeof(bool));
            file->FileInfo->position = 0;
            return file;
        }

        else
            return NULL;
    }



    return NULL;
}


void fclose(XinEntry** file)
{
    
    interrupt_disable();

    uint32_t new_size;

    if((*file)->FileInfo->tmp_size > (*file)->size)
        new_size = (*file)->FileInfo->tmp_size;
    else
        new_size = (*file)->size;

    xin_file_reallocate_with_given_size((*file), new_size, (*file)->FileInfo->buffer);
    *file = NULL;

    interrupt_enable();

}

void close(int fd)
{

    if(!FileDescriptorTable[fd].is_used)
        return;

    XinEntry* file = FileDescriptorTable[fd].entry;
    file->size = file->FileInfo->position;
    file->FileInfo->position = 0;
    memset(file->FileInfo->rights, '\0', 2);

//    for(int i = 0; i < (*file)->size / 512 + ((*file)->size % 512 != 0 ? 1 : 0); i++)
    disk_write(ATA_FIRST_BUS, ATA_MASTER, file->first_sector, 0x10, (uint16_t*)(file->FileInfo->buffer));

    // while(1);
    free(file->FileInfo->buffer);
    FileDescriptorTable[fd].is_used = false;

}

int open(char* file_path, uint32_t options)
{

    XinEntry* file = xin_find_entry(file_path);


    if (file != NULL && file->type != XIN_DIRECTORY && file->path[0] != '\0')
    {

        if(file->FileInfo == NULL)
            file->FileInfo = (FileInformationBlock*)calloc(sizeof(FileInformationBlock));

        file->FileInfo->position = 0;
        int fd = (int)((uint32_t)file - XIN_ENTRY_TABLE) / 64;
        FileDescriptorTable[fd].is_used = true;
        FileDescriptorTable[fd].entry = file;

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
                    return NULL;
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


char* getline(XinEntry* file, int line_id)
{

    char* file_data = (char*)(file->FileInfo->buffer);
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
                    return NULL;
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


    XinEntry* folder = xin_find_entry(folder_name);

    if(folder == NULL)
        return XIN_ENTRY_NOT_FOUND;

    char name[40];
    uint32_t name_length;

    memcpy(name, folder->path, 40); 
    name_length = strlen(name) - 1;

    for(XinEntry* i = (XinEntry*)XIN_ENTRY_TABLE; i < (XinEntry*)(XIN_ENTRY_TABLE + SECTOR_SIZE * 4); i++)
    {

        if(strncmp(name, i -> path, name_length))
        {
            char* tmp = (char*)i;
            for(char* j = (char*)i; j < tmp + 64; j++)
            {
                *j = '\0';
            }
        }
    }

    memset(folder->path, 0, 40);
    return XANIN_OK;

}

char* xin_get_entry_name(char* path)
{
    char* tmp = (char*)calloc(MAX_PATH);

    if(path[0] == '/' && strlen(path) == 1)
    {
        free(tmp);
        return "/";
    }

    if(!strlen(path))
    {
        free(tmp);
        return NULL;
    }

    int index = strlen(path)-1;

    if(path[strlen(path)-1] == '/')
        index--;
    
    for(; path[index-1] != '/'; index--);

    for(int i = 0; i < MAX_PATH; i++)
        tmp[i] = path[index+i];

    // if(path[strlen(path)-1] == '/')
        // tmp[strlen(tmp)] = '/';

    return tmp;
}

XinChildrenEntries* xin_get_children_entries(char* folder, bool show_hidden)
{

    if(xin_find_entry(folder) == NULL || strlen(folder) == 0)
        return (XinChildrenEntries*)NULL;

    XinChildrenEntries* Children = (XinChildrenEntries*)calloc(sizeof(XinChildrenEntries));
    Children->children = (XinEntry**)calloc(sizeof(XinEntry*));
    XinEntry* i = (XinEntry*)XIN_ENTRY_TABLE; 

    uint32_t finded_entries = 0;

    while((uint32_t)i < XIN_ENTRY_TABLE + SECTOR_SIZE * 50)
    {
        if(strcmp(xin_get_file_pf(i->path)->path, folder) && i->path[0])
        {
            if(!strcmp(i->path, folder))
            {
                if(xin_get_entry_name(i->path)[0] != '.' || show_hidden)
                {
                    Children->children[finded_entries] = i;
                    finded_entries++;
                    Children->children = (XinEntry**)realloc(Children->children, sizeof(XinEntry*) * (finded_entries));
                }
            }
        }
        i++;
    }
    Children->how_many = finded_entries;
    return Children;

}

XinChildrenEntries* xin_get_children_entries_type(char* folder, uint8_t type)
{

    if(xin_find_entry(folder) == NULL || strlen(folder) == 0)
        return (XinChildrenEntries*)NULL;

    XinChildrenEntries* Children = (XinChildrenEntries*)calloc(sizeof(XinChildrenEntries));
    Children->children = (XinEntry**)calloc(sizeof(XinEntry*));
    XinEntry* i = (XinEntry*)XIN_ENTRY_TABLE; 

    uint32_t finded_entries = 0;
    while((uint32_t)i < XIN_ENTRY_TABLE + SECTOR_SIZE * 50)
    {

        if(strcmp(xin_get_file_pf(i->path)->path, folder) && i->path[0])
        {
            if((!strcmp(i->path, folder)) && (i->type == type))
            {
                Children->children[finded_entries] = i;
                finded_entries++;
                Children->children = (XinEntry**)realloc(Children->children, sizeof(XinEntry*) * (finded_entries));
            }
        }
        i++;
    }
    Children->how_many = finded_entries;
    return Children;
}


int xin_get_file_size_in_sectors(XinEntry* File)
{
    int size = File->size / SECTOR_SIZE;
    if(File->size % SECTOR_SIZE)
        size++;
    return size;
}