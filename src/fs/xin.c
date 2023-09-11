
#include <sys/call/xanin_sys/handler/xanin_sys.h>
#include <sys/log/syslog.h>
#include <lib/libc/hal.h>
#include <fs/xin.h>
#include <lib/libc/memory.h>
#include <stddef.h>
#include <sys/devices/hda/disk.h>
#include <fs/xin_entry.h>
#include <lib/libc/string.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/colors.h>

//#define IF_FILE_NOT_EXIST
#define XIN_OPENED_FILES_COUNTER 100

XinFileDescriptor* FileDescriptorTable;

static XinEntry* XinFilesOpened[XIN_OPENED_FILES_COUNTER];

int8_t xin_base_state[100];
char xin_current_path[38] = {'\0'};
char xin_current_directory[38] = {'\0'};

char* xin_set_current_directory(char *directory)
{
    memset((uint8_t*)xin_current_directory, 0, XIN_MAX_PATH_LENGTH);

    for (int i = 0; directory[i] != '\0'; i++)
        xin_current_directory[i] = directory[i];

    return xin_current_directory;
}

void xin_get_current_directory(char* buf)
{
    memcpy((uint8_t*)buf, (uint8_t*)xin_current_directory, XIN_MAX_PATH_LENGTH);
}

char *xin_get_current_path(char *file_name)
{

    memset((uint8_t*)xin_current_path, 0, XIN_MAX_PATH_LENGTH);

    int i = 0x0;

    for (i = 0; xin_current_directory[i] != '\0'; i++)
        xin_current_path[i] = xin_current_directory[i];

    for (int pos = 0; file_name[pos] != '\0'; i++, pos++)
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
XinEntry *xin_find_entry(char* entry_name)
{
    if(!strlen(entry_name))
        return NULL;

    for (char *i = (char*)XIN_ENTRY_TABLE; (uint32_t)i < XIN_ENTRY_TABLE + (SECTOR_SIZE * 50); i += XIN_ENTRY_SIZE)
    {
        if (bstrcmp(entry_name, i))
            return (XinEntry *)i;
    }

    entry_name = xin_get_current_path(entry_name);

    // if(strlen(entry_name) > 40)
    //     return NULL;

    for (char *i = (char*)XIN_ENTRY_TABLE; (uint32_t)i < XIN_ENTRY_TABLE + (SECTOR_SIZE * 50); i += XIN_ENTRY_SIZE)
    {
        if (bstrcmp(entry_name, i))
            return (XinEntry *)i;
    }

    return NULL;
}

XinEntry* xin_get_file_pf(char* path) // pf = parent folder
{

    if(bstrcmp(path, "/"))
        return NULL;

    if(!strlen(path))
        return NULL;
        
    bool if_folder = false;

    if(path[strlen(path) - 1] == '/')
        if_folder = true;

    char parent_folder[XIN_MAX_PATH_LENGTH] = {0}; 

    int i;
    for(i = strlen(path) - 1 - if_folder; path[i] != '/'; i--);

    for(int j = 0; j <= i; j++)
        parent_folder[j] = path[j];

    XinEntry* Entry = xin_find_entry(parent_folder);

    return Entry != NULL ? Entry : NULL;

}

XinEntry *xin_find_free_entry(void)
{
    for (char *i = (char *)XIN_ENTRY_TABLE; (uint32_t)i < XIN_ENTRY_TABLE + (SECTOR_SIZE * 50); i += 64)
    {
        if (*(char *)i == '\0')
            return (XinEntry *)i;
    }

    return NULL;
}

void xin_free_temporary_data(XinEntry* File)
{
    free(File->FileInfo->buffer);
    free(File->FileInfo);
}

// DO POPRAWY
XinEntry *xin_folder_change(char *new_directory)
{

    if(strlen(new_directory) > XIN_MAX_PATH_LENGTH) 
        return NULL;

    char* tmp = (char*)calloc(XIN_MAX_PATH_LENGTH);

    strcpy(tmp, new_directory);
    new_directory = tmp;

    if(bstrcmp(new_directory, ".."))
    {
        if(bstrcmp(xin_current_directory, "/"))
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
        free(new_directory);
        return NULL;
    }

    for (int i = 0; i < SIZE_OF(xin_current_directory); i++)
        xin_current_directory[i] = '\0';

    strcpy(xin_current_directory, xin_new_directory->path);

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

        if(xin_find_entry(entry_name) != NULL)
            return XIN_FILE_EXISTS;

        strcpy(entry->path, path);

    }

    else if(!only_entry_name && entry_name[0] != '/')
    {
        char full_path[XIN_MAX_PATH_LENGTH];
        memcpy((uint8_t*)full_path, (uint8_t*)xin_get_current_path(entry_name), XIN_MAX_PATH_LENGTH);

        XinEntry* path = xin_find_entry(xin_get_file_pf(full_path)->path);

        if(path == NULL)
            return XANIN_ERROR;

        strcpy(entry->path, full_path);
    }  
    
    else if(xin_get_file_pf(entry_name) != NULL)
    {
        if(xin_find_entry(entry_name) != NULL)
            return XIN_FILE_EXISTS;

        strcpy(entry->path, entry_name);
    }
    


    else
        return XANIN_ERROR;

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

    disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x12, 5, (uint16_t*)(XIN_ENTRY_POINTERS));
    disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x1a, 10, (uint16_t*)(XIN_ENTRY_TABLE));

    return XANIN_OK;

}


void xin_file_create_at_given_sector(char *path, uint32_t first_sector, uint8_t size)
{
    XinEntry* entry_created = xin_find_free_entry();

    strcpy(entry_created->path, path);

    entry_created->creation_date = 0;
    entry_created->creation_time = 0;
    entry_created->FileInfo = NULL;
    entry_created->type = XIN_FILE;
    entry_created->modification_date = 0;
    entry_created->modification_time = 0;
    entry_created->permissions = PERMISSION_MAX;
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

void xin_load_tables(void)
{
    __disk_read(0x12, XIN_ENTRY_POINTERS_SECTORS, (uint16_t*)(XIN_ENTRY_POINTERS));
    __disk_read(0x1a, XIN_ENTRY_TABLE_SECTORS, (uint16_t*)(XIN_ENTRY_TABLE));
}

void xin_init_fs(void)
{
    xin_load_tables();
    xin_folder_change("/");
    
    for(uint8_t* i = (uint8_t*)XIN_ENTRY_POINTERS; (uint32_t)i < XIN_ENTRY_POINTERS + 0x280; i++)
    {
        if(*i == XIN_UNALLOCATED)
            *i = XIN_EOF;
    }

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


        int i;
        for(i = strlen(entry_name) - 1; i >= 0; i--)
        {
            if(entry_name[i] == '/')
                break;
        }

        char tmp[XIN_MAX_PATH_LENGTH] = {0};

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

    disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x12, 8, (uint16_t*)XIN_ENTRY_POINTERS);
    disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x1a, 40, (uint16_t*)(XIN_ENTRY_TABLE));

    return XANIN_OK;

}

int xin_file_reallocate_with_given_size(XinEntry* File, uint32_t size)
{


    uint8_t* buf = (uint8_t*)calloc(int_to_sectors(size) * SECTOR_SIZE); 
    memcpy(buf, File->FileInfo->buffer, size);
    memset(&buf[size], 0, (int_to_sectors(size) * SECTOR_SIZE) - size);

    if(File == NULL)
        return XANIN_ERROR;

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
    // File->FileInfo = NULL;
    File->permissions = PERMISSION_MAX;
    File->size = size;
    // File->type = XIN_FILE;
    // get all hard links
    XinEntriesPack* HardLinks = xin_get_hard_links(File);

    for(int i = 0; i < HardLinks->length; i++)
    {
        HardLinks->entries[i]->first_sector = (uint32_t)write_entry - XIN_ENTRY_POINTERS;
        HardLinks->entries[i]->size = size;
    }

    File->first_sector = (uint32_t)write_entry - XIN_ENTRY_POINTERS;

    disk_write(ATA_FIRST_BUS, ATA_MASTER, File->first_sector, number_of_sectors_to_allocate, (uint16_t*)buf);
    
    disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x12, 8, (uint16_t*)XIN_ENTRY_POINTERS);
    disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x1a, 40, (uint16_t*)(XIN_ENTRY_TABLE));

    free(buf);

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

size_t __xin_fread(XinEntry *entry, void *buf, size_t count)
{

    //////////////////VALIDATION///////////////////
    
    if(!count)
        return 0;

    if(entry == NULL)
        return 0;
    
    if((entry->type != XIN_FILE) && (entry->type != XIN_HARD_LINK))
        return 0;
    
    if(!entry->size)
        return 0;

    uint32_t initial_position = ftell(entry);

    if(initial_position + count > entry->size)
        count = entry->size - initial_position;

    //////////////////////////////////////////////////

    if(initial_position + count > entry->FileInfo->tmp_size)
        entry->FileInfo->tmp_size = initial_position + count;

    if(entry->FileInfo->tmp_size > entry->size)
    {
        entry->FileInfo->buffer = (uint8_t*)realloc(entry->FileInfo->buffer, (int_to_sectors(entry->FileInfo->tmp_size) + 1) * SECTOR_SIZE);
        entry->FileInfo->sector_in_use = (bool*)realloc(entry->FileInfo->sector_in_use, (int_to_sectors(entry->FileInfo->tmp_size) + 1));
    }

    uint32_t sectors_to_load = int_to_sectors(count + initial_position);

    if(sectors_to_load > int_to_sectors(entry->size))
        sectors_to_load = int_to_sectors(entry->size);

    for(int i = 0; i < sectors_to_load; i++)
    {
        if(!entry->FileInfo->sector_in_use[i])
        {
            __disk_read(entry->first_sector + i, 1, (uint16_t*)(entry->FileInfo->buffer + (i * SECTOR_SIZE)));
            entry->FileInfo->sector_in_use[i] = true;
        }
    }

    if(buf == NULL)
        return 0;

    for (char *i = (char*)(entry->FileInfo->buffer + initial_position); i < (char*)(entry->FileInfo->buffer + initial_position + count); i++, buf++)
        *(char *)buf = *i;

    fseek(entry, initial_position + count);

    return count;

}

size_t __xin_read(int fd, void *buf, size_t count)
{

    //////////////////VALIDATION///////////////////

    if(fd < 0)
        return 0;

    XinEntry* entry = (XinEntry*)(XIN_ENTRY_TABLE + (fd * XIN_ENTRY_SIZE));

    if((entry->type != XIN_FILE) && (entry->type != XIN_HARD_LINK))
        return 0;

    uint32_t initial_position = ftell(entry);

    if(initial_position >= (entry->size - 1)) //last valid byte
        return 0;

    if(initial_position + count > entry->size)
        count = initial_position + count - (entry->size + 1); //0 is start index

    //////////////////////////////////////////////////

    return fread(entry, buf, count);

}

size_t __xin_fwrite(XinEntry *entry, void *buf, size_t count)
{
    //////////////////VALIDATION///////////////////
    
    if(!count)
        return 0;

    if(entry == NULL)
        return 0;

    if((entry->type != XIN_FILE) && (entry->type != XIN_HARD_LINK))
        return 0;

    //////////////////////////////////////////////////

    uint32_t initial_position = ftell(entry);

    if((initial_position + count) > entry->FileInfo->tmp_size)
        entry->FileInfo->tmp_size = initial_position + count;

    entry->FileInfo->buffer = (uint8_t*)realloc(entry->FileInfo->buffer, (int_to_sectors(entry->FileInfo->tmp_size) + 1) * SECTOR_SIZE);
    entry->FileInfo->sector_in_use = (bool*)realloc(entry->FileInfo->sector_in_use, (int_to_sectors(entry->FileInfo->tmp_size) + 1));

    uint32_t sectors_to_load = int_to_sectors(count + initial_position);

    if(sectors_to_load > int_to_sectors(entry->size))
        sectors_to_load = int_to_sectors(entry->size);
    
    for(int i = 0; i < sectors_to_load; i++)
    {
        if(!entry->FileInfo->sector_in_use[i])
        {
            __disk_read(entry->first_sector + i, 1, (uint16_t*)(entry->FileInfo->buffer + (i * SECTOR_SIZE)));
            entry->FileInfo->sector_in_use[i] = true;
        }
    }

    fseek(entry, initial_position);

    for (char *i = (char *)(entry->FileInfo->buffer) + initial_position; i < (char*)(entry->FileInfo->buffer + initial_position + count); i++, buf++)
    {
        *i = *(char *)buf;
        fseek(entry, ftell(entry) + 1);
    }

    time_get(&SystemTime);

    entry->modification_date = (uint32_t)((SystemTime.day_of_month << 24) | (SystemTime.month << 16) | (SystemTime.century << 8) | (SystemTime.year)); 
    entry->modification_time = (uint16_t)(SystemTime.hour << 8) | (SystemTime.minutes);
    // xprintf("buf: %s\n", entry->FileInfo->buffer);
    return count;
}

size_t __xin_write(int fd, void* buf, size_t count)
{

    ////////////////////VALIDATION/////////////////////
    
    if(fd < 0)
        return 0;

    if(!FileDescriptorTable[fd].is_used)
        return 0;

    XinEntry* entry = (XinEntry*)(XIN_ENTRY_TABLE + (fd * XIN_ENTRY_SIZE));
    
    if((entry->type != XIN_FILE) && (entry->type != XIN_HARD_LINK))
        return 0;

    //////////////////////////////////////////////////

    return fwrite(entry, buf, count);

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

XinEntry *__xin_fopen(char *file_path, char *mode)
{

    XinEntry* file = xin_find_entry(file_path);

    if(file != NULL)
    {
    	if(file->type != XIN_FILE && file->type != XIN_HARD_LINK)
            return NULL;

        file->FileInfo = (FileInformationBlock*)calloc(SIZE_OF(FileInformationBlock));
        file->FileInfo->buffer = (uint8_t*)calloc(file->size + SECTOR_SIZE); // additional space
        file->FileInfo->sector_in_use = (bool*)calloc(int_to_sectors(file->size) + 1); // additional space

        strcpy(file->FileInfo->rights, mode);

        file->FileInfo->position = 0;
        file->FileInfo->tmp_size = 0;
        xin_add_files_to_xfo(file);
    }

    if(bstrncmp(mode, "a", 2))
    {
        file->FileInfo = (FileInformationBlock*)calloc(SIZE_OF(FileInformationBlock));
        file->FileInfo->buffer = (uint8_t*)calloc(file->size + SECTOR_SIZE);
        file->FileInfo->sector_in_use = (bool*)calloc(int_to_sectors(file->size) + 5);

        strcpy(file->FileInfo->rights, mode);

        file->FileInfo->position = file->size;
        file->FileInfo->tmp_size = file->size;
        xin_add_files_to_xfo(file);

        return file;
    }


    else if(bstrncmp(mode, "r", 2))
    {
        xin_add_files_to_xfo(file);
        return file;
    }

    else if(bstrncmp(mode, "rw", 2) || bstrncmp(mode, "w", 2))
    {
        int status = xin_file_create(file_path);

        if(status == XANIN_OK)
        {
      	    file = xin_find_entry(file_path);
            file->FileInfo = (FileInformationBlock*)calloc(SIZE_OF(FileInformationBlock));
            file->FileInfo->buffer = (uint8_t*)calloc(file->size + SECTOR_SIZE);
            file->FileInfo->sector_in_use = (bool*)calloc(int_to_sectors(file->size) + 5);

            strcpy(file->FileInfo->rights, mode);

            file->FileInfo->position = 0;
            file->FileInfo->tmp_size = 0;
            xin_add_files_to_xfo(file);
            return file;
        }

        else if(status == XIN_FILE_EXISTS)
        {
            xin_add_files_to_xfo(file);
            return file;
        }

        else
            return NULL;
    }

    return NULL;
}

void fclose_with_given_size(XinEntry** file, uint32_t new_size)
{

    if(*file == NULL)
        return;

    if(!bstrncmp((*file)->FileInfo->rights, "r", 2)) //READ-ONLY OPTION
        xin_file_reallocate_with_given_size((*file), new_size);

    free((*file)->FileInfo->buffer);
    free((*file)->FileInfo);

    for(int i = 0; i < XIN_OPENED_FILES_COUNTER; i++)
    {
        if(XinFilesOpened[i] == (*file))
            XinFilesOpened[i] = NULL;
    }

    (*file)->FileInfo = NULL;
    (*file) = NULL;

}



void __xin_fclose(XinEntry** file)
{
    if(*file == NULL)
        return;

    uint32_t new_size;

    if((*file)->FileInfo->tmp_size > (*file)->size)
        new_size = (*file)->FileInfo->tmp_size;
    else
        new_size = (*file)->size;

    if(!bstrncmp((*file)->FileInfo->rights, "r", 2)) // READ-ONLY OPTION
        xin_file_reallocate_with_given_size((*file), new_size);

    free((*file)->FileInfo->buffer);
    free((*file)->FileInfo);

    for(int i = 0; i < XIN_OPENED_FILES_COUNTER; i++)
    {
        if(XinFilesOpened[i] == (*file))
            XinFilesOpened[i] = NULL;
    }

    (*file)->FileInfo = NULL;
    (*file) = NULL;

}

void __xin_close(int fd)
{

    if(!FileDescriptorTable[fd].is_used)
        return;

    XinEntry* File = FileDescriptorTable[fd].entry;
    
    fclose(&File);

    FileDescriptorTable[fd].is_used = false;

}

int __xin_open(char* file_path, uint32_t options)
{

    XinEntry* file = xin_find_entry(file_path);


    if (file != NULL && file->type != XIN_DIRECTORY && file->path[0] != '\0')
    {

        if(file->FileInfo == NULL)
            file->FileInfo = (FileInformationBlock*)calloc(SIZE_OF(FileInformationBlock));

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
        memset((uint8_t*)line, 0, 200);

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
        memset((uint8_t*)line, 0, 200);

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
                {
                    free(line);
                    return NULL;
                }
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

    free(line);
    return line;



}

__STATUS remove_directory(char* folder_name)
{


    XinEntry* folder = xin_find_entry(folder_name);

    if(folder == NULL)
        return XIN_ENTRY_NOT_FOUND;

    char name[XIN_MAX_PATH_LENGTH];
    uint32_t name_length;

    memcpy((uint8_t*)name, (uint8_t*)folder->path, XIN_MAX_PATH_LENGTH); 
    name_length = strlen(name) - 1;

    for(XinEntry* i = (XinEntry*)XIN_ENTRY_TABLE; i < (XinEntry*)(XIN_ENTRY_TABLE + SECTOR_SIZE * 4); i++)
    {

        if(bstrncmp(name, i -> path, name_length))
        {
            char* tmp = (char*)i;
            for(char* j = (char*)i; j < tmp + 64; j++)
            {
                *j = '\0';
            }
        }
    }

    memset((uint8_t*)folder->path, 0, XIN_MAX_PATH_LENGTH);
    return XANIN_OK;

}

char* xin_get_entry_name(char* path)
{
    char* tmp = (char*)calloc(XIN_MAX_PATH_LENGTH);

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

    for(int i = 0; i < XIN_MAX_PATH_LENGTH; i++)
        tmp[i] = path[index+i];

    // if(path[strlen(path)-1] == '/')
        // tmp[strlen(tmp)] = '/';

    return tmp;
}

XinChildrenEntries* xin_get_children_entries(char* folder, bool get_hidden)
{

    if(xin_find_entry(folder) == NULL || strlen(folder) == 0)
        return (XinChildrenEntries*)NULL;

    XinChildrenEntries* Children = (XinChildrenEntries*)calloc(SIZE_OF(XinChildrenEntries));
    Children->children = (XinEntry**)calloc(SIZE_OF(XinEntry*));
    XinEntry* i = (XinEntry*)XIN_ENTRY_TABLE; 

    uint32_t finded_entries = 0;

    while((uint32_t)i < XIN_ENTRY_TABLE + SECTOR_SIZE * 50)
    {
        if(bstrcmp(xin_get_file_pf(i->path)->path, folder) && i->path[0])
        {
            if(!bstrcmp(i->path, folder))
            {
                if(xin_get_entry_name(i->path)[0] != '.' || get_hidden)
                {
                    Children->children[finded_entries] = i;
                    finded_entries++;
                    Children->children = (XinEntry**)realloc(Children->children, SIZE_OF(XinEntry*) * (finded_entries));
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

    XinChildrenEntries* Children = (XinChildrenEntries*)calloc(SIZE_OF(XinChildrenEntries));
    Children->children = (XinEntry**)calloc(SIZE_OF(XinEntry*));
    XinEntry* i = (XinEntry*)XIN_ENTRY_TABLE; 

    uint32_t finded_entries = 0;
    while((uint32_t)i < XIN_ENTRY_TABLE + SECTOR_SIZE * 50)
    {

        if(bstrcmp(xin_get_file_pf(i->path)->path, folder) && i->path[0])
        {
            if((!bstrcmp(i->path, folder)) && (i->type == type))
            {
                Children->children[finded_entries] = i;
                finded_entries++;
                Children->children = (XinEntry**)realloc(Children->children, SIZE_OF(XinEntry*) * (finded_entries));
            }
        }
        i++;
    }
    Children->how_many = finded_entries;
    return Children;
}

XinEntriesPack* xin_get_hard_links(const XinEntry* const File)
{

    XinEntriesPack* Pack = (XinEntriesPack*)calloc(SIZE_OF(XinEntriesPack));
    Pack->entries = (XinEntry**)calloc(SIZE_OF(XinEntry*));

    if(File->type != XIN_FILE || !File)
        return Pack;

    for(XinEntry* i = (XinEntry*)XIN_ENTRY_TABLE; (uint32_t)i < XIN_ENTRY_TABLE + (SECTOR_SIZE * 50); i++)
    {
        if(i->first_sector == File->first_sector)
        {
            Pack->entries = realloc(Pack->entries, Pack->length + 1);
            Pack->entries[Pack->length++] = i;
        }
    }

    return Pack;
    
}

int xin_get_file_size_in_sectors(XinEntry* File)
{
    int size = File->size / SECTOR_SIZE;
    if(File->size % SECTOR_SIZE)
        size++;
    return size;
}


bool xin_add_files_to_xfo(XinEntry* File)
{
    for(int i = 0; i < XIN_OPENED_FILES_COUNTER; i++)
    {
        if(XinFilesOpened[i] == File)
            return true;
    }

    for(int i = 0; i < XIN_OPENED_FILES_COUNTER; i++)
    {
        if(!XinFilesOpened[i])
        {
            XinFilesOpened[i] = File;
            return true;
        }
    }

    return false;
}

void xin_close_all_files(void)
{
    interrupt_disable();

    for(int i = 0; i < XIN_OPENED_FILES_COUNTER; i++)
    {
        if(XinFilesOpened[i] != NULL)
            fclose(&XinFilesOpened[i]);
    }

    interrupt_enable();
}