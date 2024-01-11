
#include <sys/call/xanin_sys/handler/xanin_sys.h>
#include <sys/log/syslog.h>
#include <lib/libc/hal.h>
#include <fs/xin.h>
#include <lib/libc/memory.h>
#include <stddef.h>
#include <sys/devices/hda/disk.h>
#include <lib/libc/string.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/colors.h>
#include <lib/libc/file.h>

// #define IF_FILE_NOT_EXIST
#define XIN_OPENED_FILES_COUNTER 100

XinFileDescriptor *FileDescriptorTable;

static XinEntry *XinFilesOpened[XIN_OPENED_FILES_COUNTER];

int8_t xin_base_state[100];
static char xin_current_path[XIN_MAX_PATH_LENGTH];
static char xin_current_directory[XIN_MAX_PATH_LENGTH];

static XinFileSystemData XinFsData; // XinFS DATA SINGLETONE

#define XIN_FS_BEGIN (XinFsData.tables)

#define XIN_FS_PTRS_BEGIN (XinFsData.tables)
#define XIN_FS_ENTRIES_BEGIN (XinFsData.tables + (SECTOR_SIZE * XinFsData.ptrs_size))
#define XIN_FS_ENTRIES_END (XinFsData.tables + (SECTOR_SIZE * (XinFsData.ptrs_size + XinFsData.entries_size)))

#define XIN_FS_ENTRIES_SIZE (XinFsData.entries_size)
#define XIN_FS_PTRS_SIZE (XinFsData.ptrs_size)

XinFileSystemData __xin_fs_data_get(void)
{
    return XinFsData;
}

uint8_t *__xin_fs_tables_get(void)
{
    return XinFsData.tables;
}

uint8_t *__xin_fs_ptrs_get(void)
{
    return XinFsData.tables;
}

uint32_t __xin_fs_entries_size_get(void)
{
    return XinFsData.entries_size;
}

uint32_t __xin_fs_ptrs_size_get(void)
{
    return XinFsData.ptrs_size;
}

XinEntry *__xin_fs_entries_get(void)
{
    return (XinEntry *)(XinFsData.tables + (SECTOR_SIZE * XinFsData.ptrs_size));
}

XinEntry *__xin_fs_entries_end_get(void)
{
    return (XinEntry *)(XinFsData.tables + (SECTOR_SIZE * (XinFsData.ptrs_size + XinFsData.entries_size)));
}

void __xin_fs_tables_set(uint8_t *tables)
{
    XinFsData.tables = tables;
}

void __xin_fs_load_tables_from_disk(void) // oj calkiem risky
{
    __disk_read(XinFsData.first_sector, XinFsData.ptrs_size + XinFsData.entries_size, (uint16_t *)XinFsData.tables);
}

void __xin_detect_file_system(void)
{
    __disk_read_bytes(ATA_FIRST_BUS, ATA_MASTER, 0, 2, 12, (uint8_t *)&XinFsData);
}

bool __xin_check_if_valid_directory(char *directory)
{
    XinEntry *Entry = __xin_find_entry(directory);

    if (Entry == NULL)
        return false;
    if (Entry->type != XIN_DIRECTORY)
        return false;
    if (Entry->path[0] != '/')
        return false;
    return true;
}

bool __xin_current_directory_set(char *directory)
{
    if (!__xin_check_if_valid_directory(directory))
        return false;
    strncpy(xin_current_directory, directory, XIN_MAX_PATH_LENGTH);
    return true;
}

char *__xin_current_directory_get(char *buf)
{
    memcpy((uint8_t *)buf, (uint8_t *)xin_current_directory, XIN_MAX_PATH_LENGTH);
    return buf;
}

uint8_t *__xin_find_free_pointer(void)
{
    // for (char *i = (char *)XIN_ENTRY_POINTERS; (uint32_t)i < XIN_ENTRY_POINTERS + (SECTOR_SIZE * 8); i++)
    // {
    //     if (*i == XIN_UNALLOCATED)
    //         return (uint8_t *)i;
    // }

    for (xin_ptr_t *i = (xin_ptr_t *)XIN_FS_PTRS_BEGIN; (uint32_t)i < (uint32_t)(XIN_FS_PTRS_BEGIN + (XIN_FS_PTRS_SIZE * SECTOR_SIZE)); i++)
    {
        if (*i == XIN_UNALLOCATED)
            return (uint8_t *)i;
    }

    return NULL;
}

uint8_t *__xin_find_free_pointer_with_given_size(uint32_t size)
{

    if (!size)
        return NULL;

    // for (uint8_t *i = (uint8_t *)XIN_ENTRY_POINTERS + SECTOR_SIZE; (uint32_t)i < XIN_ENTRY_POINTERS + (SECTOR_SIZE * 8); i++)
    // {
    //     if (*i == XIN_UNALLOCATED)
    //     {
    //         bool ok = true;
    //         for (int j = 0; j < size; j++)
    //         {
    //             if (i[j] != XIN_UNALLOCATED)
    //             {
    //                 ok = false;
    //                 break;
    //             }
    //         }

    //         if (ok)
    //             return i;
    //     }
    // }

    // leave alone first 512sectors
    for (xin_ptr_t *i = (uint8_t *)XIN_FS_BEGIN + SECTOR_SIZE; (uint32_t)i < (uint32_t)(XIN_FS_BEGIN + (SECTOR_SIZE * XIN_FS_PTRS_SIZE)); i++)
    {
        if (*i == XIN_UNALLOCATED)
        {
            bool ok = true;
            for (int j = 0; j < size; j++)
            {
                if (i[j] != XIN_UNALLOCATED)
                {
                    ok = false;
                    break;
                }
            }

            if (ok)
                return i;
        }
    }

    return NULL;
}

void __xin_entry_resize(XinEntry *entry, uint32_t new_size)
{
    // uint8_t *xin_pointer_table_entry = (uint8_t *)(XIN_ENTRY_POINTERS + entry->first_sector);
    uint8_t *xin_pointer_table_entry = (uint8_t *)(XIN_FS_PTRS_BEGIN + entry->first_sector);
    uint32_t sectors = entry->size / SECTOR_SIZE + (entry->size % SECTOR_SIZE != 0 ? 1 : 0);

    for (int i = 0; i < sectors; i++)
        xin_pointer_table_entry[i] = XIN_UNALLOCATED;

    sectors = new_size / SECTOR_SIZE + (new_size % SECTOR_SIZE != 0 ? 1 : 0);
    xin_pointer_table_entry = __xin_find_free_pointer_with_given_size(sectors);

    for (int i = 0; i < sectors; i++)
        xin_pointer_table_entry[i] = XIN_ALLOCATED;
}

char *__xin_path_get(char *file_name)
{
    char *buf = kcalloc(XIN_MAX_PATH_LENGTH);

    strncpy(buf, xin_current_directory, XIN_MAX_PATH_LENGTH);

    int pos = strlen(buf);

    if (pos + strlen(file_name) >= XIN_MAX_PATH_LENGTH)
    {
        kfree(buf);
        return NULL;
    }

    strncpy(&buf[pos], file_name, XIN_MAX_PATH_LENGTH - pos);

    return buf;
}

/* DIRECTORY AND FILES */
XinEntry *__xin_find_entry(char *entry_name)
{
    if (!strlen(entry_name)) // if path is empty
        return NULL;

    // for (char *i = (char *)XIN_ENTRY_TABLE; (uint32_t)i < XIN_ENTRY_TABLE + (SECTOR_SIZE * 50); i += XIN_ENTRY_SIZE)
    // {
    //     if (bstrcmp(entry_name, i))
    //         return (XinEntry *)i;
    // }

    // entry_name = __xin_path_get(entry_name);

    // // if(strlen(entry_name) > 40)
    // //     return NULL;

    // for (char *i = (char *)XIN_ENTRY_TABLE; (uint32_t)i < XIN_ENTRY_TABLE + (SECTOR_SIZE * 50); i += XIN_ENTRY_SIZE)
    // {
    //     if (bstrcmp(entry_name, i))
    //         return (XinEntry *)i;

    for (XinEntry *i = (XinEntry *)XIN_FS_ENTRIES_BEGIN; (uint32_t)i < (uint32_t)(XIN_FS_ENTRIES_BEGIN + (SECTOR_SIZE * XIN_FS_ENTRIES_SIZE)); i++)
    {
        if (bstrcmp(entry_name, i->path))
            return i;
    }

    entry_name = __xin_path_get(entry_name);

    // if(strlen(entry_name) > 40)
    //     return NULL;

    for (XinEntry *i = (XinEntry *)XIN_FS_ENTRIES_BEGIN; (uint32_t)i < (uint32_t)(XIN_FS_ENTRIES_BEGIN + (SECTOR_SIZE * XIN_FS_ENTRIES_SIZE)); i++)
    {
        if (bstrcmp(entry_name, i->path))
            return i;
    }
    return NULL;
}

XinEntry *__xin_get_file_pf(char *path) // pf = parent folder
{
    if (!strlen(path))
        return NULL;

    if (bstrcmp(path, "/"))
        return NULL;

    bool if_folder = false;

    if (path[strlen(path) - 1] == '/')
        if_folder = true;

    char parent_folder[XIN_MAX_PATH_LENGTH] = {0};

    int i;
    for (i = strlen(path) - 1 - if_folder; path[i] != '/'; i--)
        ;

    for (int j = 0; j <= i; j++)
        parent_folder[j] = path[j];

    XinEntry *Entry = __xin_find_entry(parent_folder);

    return Entry != NULL ? Entry : NULL;
}

XinEntry *__xin_find_free_entry(void)
{
    for (XinEntry *i = (XinEntry *)XIN_FS_ENTRIES_BEGIN; i < (XinEntry *)XIN_FS_ENTRIES_END; i++)
        if (i->path[0] == '\0')
            return i;

    return NULL;
}

void __xin_free_temporary_data(XinEntry *File)
{
    free(File->FileInfo->buffer);
    free(File->FileInfo);
}

__STATUS __xin_folder_change(char *new_directory)
{

    if (strlen(new_directory) > XIN_MAX_PATH_LENGTH)
        // return NULL;
        return XANIN_ERROR;

    char *tmp = (char *)calloc(XIN_MAX_PATH_LENGTH);

    strcpy(tmp, new_directory);
    new_directory = tmp;

    if (bstrcmp(new_directory, ".."))
    {
        if (bstrcmp(xin_current_directory, "/"))
            // return NULL;
            return XANIN_ERROR;
        else
        {
            xin_current_directory[strlen(xin_current_directory) - 1] = '\0';
            int counter = strlen(xin_current_directory) - 1;

            while (xin_current_directory[counter] != '/')
            {
                xin_current_directory[counter] = '\0';
                counter--;
            }
        }
        // return xin_find_entry(xin_current_directory);
        return XANIN_OK;
    }

    if (new_directory[strlen(new_directory) - 1] != '/')
    {
        int name_length = strlen(new_directory); // '\0' before any modification
        new_directory[name_length + 1] = '\0';
        new_directory[name_length] = '/';
    }

    if (new_directory[0] != '/')
        new_directory = strconcat(xin_current_directory, new_directory);

    XinEntry *xin_new_directory = __xin_find_entry(new_directory);

    if (xin_new_directory == NULL)
    {
        free(new_directory);
        // while (1)
        // ;
        // return NULL;
        return XANIN_ERROR;
    }

    else if (xin_new_directory->type != XIN_DIRECTORY)
    {
        free(new_directory);
        // return NULL;
        return XANIN_ERROR;
    }

    else if (new_directory[strlen(new_directory) - 1] != '/')
    {
        free(new_directory);
        // return NULL;
        return XANIN_ERROR;
    }

    for (int i = 0; i < SIZE_OF(xin_current_directory); i++)
        xin_current_directory[i] = '\0';

    strcpy(xin_current_directory, xin_new_directory->path);

    free(new_directory);
    return XANIN_OK;

    // if (strlen(new_directory) > XIN_MAX_PATH_LENGTH)
    //     return XIN_TO_LONG_PATH;

    // char *tmp = (char *)calloc(XIN_MAX_PATH_LENGTH);

    // strcpy(tmp, new_directory);
    // new_directory = tmp;

    // if (bstrcmp(new_directory, ".."))
    // {
    //     if (bstrcmp(xin_current_directory, "/"))
    //         return XIN_ENTRY_NOT_FOUND;
    //     else
    //     {
    //         xin_current_directory[strlen(xin_current_directory) - 1] = '\0';
    //         int counter = strlen(xin_current_directory) - 1;

    //         while (xin_current_directory[counter] != '/')
    //         {
    //             xin_current_directory[counter] = '\0';
    //             counter--;
    //         }
    //     }
    //     return XANIN_OK;
    // }

    // if (new_directory[strlen(new_directory) - 1] != '/')
    // {
    //     int name_length = strlen(new_directory); // '\0' before any modification
    //     new_directory[name_length + 1] = '\0';
    //     new_directory[name_length] = '/';
    // }

    // XinEntry *xin_new_directory = __xin_find_entry(new_directory);

    // if (xin_new_directory == NULL)
    // {
    //     free(new_directory);
    //     return XIN_ENTRY_NOT_FOUND;
    // }

    // else if (xin_new_directory->type != XIN_DIRECTORY)
    // {
    //     free(new_directory);
    //     return XIN_NOT_A_FOLDER;
    // }

    // else if (new_directory[strlen(new_directory) - 1] != '/')
    // {
    //     free(new_directory);
    //     return XIN_BAD_FOLDER_NAME;
    // }

    // for (int i = 0; i < SIZE_OF(xin_current_directory); i++)
    //     xin_current_directory[i] = '\0';

    // strcpy(xin_current_directory, xin_new_directory->path);

    // free(new_directory);
    // return XANIN_OK;
}

int __xin_folder_create(char *entry_name)
{

    bool only_entry_name = true;

    XinEntry *entry = __xin_find_free_entry();

    if (entry_name[0] == '/' && entry_name[1] == '/')
        return XIN_FILE_EXISTS;

    if (entry_name[strlen(entry_name) - 1] != '/')
        return XIN_BAD_FOLDER_NAME;

    entry_name[strlen(entry_name) - 1] = ' ';

    for (int i = strlen(entry_name) - 1; i >= 0; i--)
    {
        if (entry_name[i] == '/')
        {
            only_entry_name = false;
            break;
        }
    }

    entry_name[strlen(entry_name) - 1] = '/';

    if (only_entry_name)
    {
        char *path = __xin_path_get(entry_name);

        if (__xin_find_entry(entry_name) != NULL)
            return XIN_FILE_EXISTS;

        strcpy(entry->path, path);
    }

    else if (!only_entry_name && entry_name[0] != '/')
    {
        char full_path[XIN_MAX_PATH_LENGTH];
        memcpy((uint8_t *)full_path, (uint8_t *)__xin_path_get(entry_name), XIN_MAX_PATH_LENGTH);

        XinEntry *path = __xin_find_entry(__xin_get_file_pf(full_path)->path);

        if (path == NULL)
            return XANIN_ERROR;

        strcpy(entry->path, full_path);
    }

    else if (__xin_get_file_pf(entry_name) != NULL)
    {
        if (__xin_find_entry(entry_name) != NULL)
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

    // __disk_sectors_write(ATA_FIRST_BUS, ATA_MASTER, 0x12, 5, (uint16_t *)(XIN_ENTRY_POINTERS));
    // __disk_sectors_write(ATA_FIRST_BUS, ATA_MASTER, 0x1a, 10, (uint16_t *)(XIN_ENTRY_TABLE));
    __xin_tables_update();

    return XANIN_OK;
}

void __xin_file_create_at_given_sector(char *path, uint32_t first_sector, uint8_t size)
{
    XinEntry *entry_created = __xin_find_free_entry();

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

    uint8_t *write_entry = (uint8_t *)(XIN_FS_PTRS_BEGIN + first_sector);

    uint8_t tmp = 0;

    for (uint8_t *i = write_entry; i < (write_entry + size); i++)
        *i = XIN_ALLOCATED;
    tmp++;

    *(write_entry + size - 1) = XIN_EOF;
}

// ADD PARAMETER TMP TABLE
void __xin_init_fs(void)
{
    // xin_load_tables();
    __xin_folder_change("/");

    for (xin_ptr_t *i = XIN_FS_PTRS_BEGIN; i < XIN_FS_PTRS_BEGIN + 0x280; i++)
    {
        if (*i == XIN_UNALLOCATED)
            *i = XIN_EOF;
    }
}

void __xin_tables_update(void)
{
    __disk_sectors_write(ATA_FIRST_BUS, ATA_MASTER, XinFsData.first_sector, XIN_FS_PTRS_SIZE, (uint16_t *)XIN_FS_ENTRIES_BEGIN);
    __disk_sectors_write(ATA_FIRST_BUS, ATA_MASTER, XinFsData.first_sector + XIN_FS_PTRS_SIZE, XIN_FS_ENTRIES_SIZE, (uint16_t *)(XIN_FS_ENTRIES_BEGIN));
}

__STATUS __xin_file_create(char *entry_name)
{

    bool only_entry_name = true;

    XinEntry *entry = __xin_find_free_entry();

    if (entry == NULL)
        return XANIN_ERROR;

    if (__xin_find_entry(entry_name) != NULL)
        return XIN_FILE_EXISTS;

    for (int i = strlen(entry_name) - 1; i >= 0; i--)
    {
        if (entry_name[i] == '/')
        {
            only_entry_name = false;
            break;
        }
    }

    if (!only_entry_name && entry_name[0] != '/')
    {

        int i;
        for (i = strlen(entry_name) - 1; i >= 0; i--)
        {
            if (entry_name[i] == '/')
                break;
        }

        char tmp[XIN_MAX_PATH_LENGTH] = {0};

        for (int j = 0; j <= i; j++)
            tmp[j] = entry_name[j];

        strcpy(tmp, __xin_path_get(tmp));
        XinEntry *path = __xin_find_entry(tmp);

        if (path == NULL)
        {
            memset((uint8_t *)entry, 0, 64);
            return XANIN_ERROR;
        }

        strcpy(entry->path, __xin_path_get(entry_name));
    }

    else if (only_entry_name)
    {
        char *path = __xin_path_get(entry_name);

        if (__xin_find_entry(entry_name) != NULL)
            return XIN_FILE_EXISTS;

        strcpy(entry->path, path);
    }

    else if (entry_name[0] == '/')
    {
        if (__xin_find_entry(entry_name) != NULL)
            return XIN_FILE_EXISTS;

        strcpy(entry->path, entry_name);
    }

    else if (__xin_get_file_pf(entry_name) != NULL)
    {
        if (__xin_find_entry(entry_name) != NULL)
            return XIN_FILE_EXISTS;

        strcpy(entry->path, entry_name);
    }

    else
        return XANIN_ERROR;

    /* write entry to xin entry pointers table */
    xin_ptr_t *entry_xin_fs_ptr = __xin_find_free_pointer(); //_with_given_size(2);

    entry_xin_fs_ptr[0] = XIN_EOF;

    time_get(&SystemTime);

    entry->creation_date = (uint32_t)((SystemTime.day_of_month << 24) | (SystemTime.month << 16) | (SystemTime.century << 8) | (SystemTime.year));
    entry->creation_time = (uint16_t)(SystemTime.hour << 8) | (SystemTime.minutes);
    entry->modification_date = (uint32_t)((SystemTime.day_of_month << 24) | (SystemTime.month << 16) | (SystemTime.century << 8) | (SystemTime.year));
    entry->modification_time = (uint16_t)(SystemTime.hour << 8) | (SystemTime.minutes);
    entry->FileInfo = NULL;
    entry->permissions = PERMISSION_MAX;
    entry->size = 0;
    entry->type = XIN_FILE;

    entry->first_sector = entry_xin_fs_ptr - XIN_FS_PTRS_BEGIN;

    uint8_t *zeros = (uint8_t *)calloc(SECTOR_SIZE);

    __disk_sectors_write(ATA_FIRST_BUS, ATA_MASTER, entry->first_sector, 1, (uint16_t *)zeros);

    free(zeros);

    __xin_tables_update();

    // __disk_sectors_write(ATA_FIRST_BUS, ATA_MASTER, 0x12, 8, (uint16_t *)XIN_ENTRY_POINTERS);
    // __disk_sectors_write(ATA_FIRST_BUS, ATA_MASTER, 0x1a, 40, (uint16_t *)(XIN_ENTRY_TABLE));

    return XANIN_OK;
}

int __xin_file_reallocate_with_given_size(XinEntry *File, uint32_t size)
{

    uint8_t *buf = (uint8_t *)calloc(int_to_sectors(size) * SECTOR_SIZE);
    memcpy(buf, File->FileInfo->buffer, size);
    memset(&buf[size], 0, (int_to_sectors(size) * SECTOR_SIZE) - size);

    if (File == NULL)
        return XANIN_ERROR;

    uint32_t number_of_sectors_to_deallocate = int_to_sectors(File->size);
    if (!number_of_sectors_to_deallocate)
        number_of_sectors_to_deallocate++;

    uint32_t number_of_sectors_to_allocate = int_to_sectors(size);
    if (!number_of_sectors_to_allocate)
        number_of_sectors_to_allocate++;

    // uint8_t *entry_deallocate = (uint8_t *)(File->first_sector + XIN_ENTRY_POINTERS);
    uint8_t *entry_deallocate = (uint8_t *)(File->first_sector + XIN_FS_ENTRIES_BEGIN);
    for (int i = 0; i < number_of_sectors_to_deallocate; i++)
        entry_deallocate[i] = XIN_UNALLOCATED;

    /* write entry to xin entry pointers table */
    uint8_t *write_entry = __xin_find_free_pointer_with_given_size(number_of_sectors_to_allocate);

    for (int i = 0; i < number_of_sectors_to_allocate - 1; i++)
        write_entry[i] = XIN_ALLOCATED;

    write_entry[number_of_sectors_to_allocate - 1] = XIN_EOF;

    /* write entry to xin entry data table */
    time_get(&SystemTime);

    File->creation_date = (uint32_t)((SystemTime.day_of_month << 24) | (SystemTime.month << 16) | (SystemTime.century << 8) | (SystemTime.year));
    File->creation_time = (uint16_t)(SystemTime.hour << 8) | (SystemTime.minutes);
    File->modification_date = (uint32_t)((SystemTime.day_of_month << 24) | (SystemTime.month << 16) | (SystemTime.century << 8) | (SystemTime.year));
    File->modification_time = (uint16_t)(SystemTime.hour << 8) | (SystemTime.minutes);
    File->permissions = PERMISSION_MAX;
    File->size = size;

    // get all hard links
    XinEntriesPack *HardLinks = __xin_hard_links_get(File);

    for (int i = 0; i < HardLinks->length; i++)
    {
        // HardLinks->entries[i]->first_sector = (uint32_t)write_entry - XIN_ENTRY_POINTERS;
        HardLinks->entries[i]->first_sector = write_entry - XIN_FS_PTRS_BEGIN;
        HardLinks->entries[i]->size = size;
    }

    // File->first_sector = write_entry - XIN_ENTRY_POINTERS;
    File->first_sector = write_entry - XIN_FS_PTRS_BEGIN;

    __disk_sectors_write(ATA_FIRST_BUS, ATA_MASTER, File->first_sector, number_of_sectors_to_allocate, (uint16_t *)buf);
    __xin_tables_update();

    free(buf);

    return XANIN_OK;
}

__STATUS __xin_entry_remove(char *entry_name)
{
    XinEntry *entry_data = __xin_find_entry(entry_name);
    uint32_t sectors_used = int_to_sectors(entry_data->size);

    if (!sectors_used)
        sectors_used++;

    if (entry_data == NULL)
        return XIN_ENTRY_NOT_FOUND;

    else if (entry_data->type == XIN_FILE)
    {
        for (xin_ptr_t *i = entry_data->first_sector + XIN_FS_PTRS_BEGIN;
             i < entry_data->first_sector + XIN_FS_PTRS_BEGIN + sectors_used; i++)
            *i = XIN_UNALLOCATED;

        char *tmp = (char *)entry_data;

        for (int i = 0; i < 64; i++)
            tmp[i] = '\0';
    }

    return XANIN_OK;
}

uint32_t xin_get_start_sector(char *entry_name)
{
    XinEntry *xin_file_descriptor = __xin_find_entry(entry_name);

    return xin_file_descriptor->first_sector;
}

size_t __xin_fread(XinEntry *entry, void *buf, size_t count)
{

    //////////////////VALIDATION///////////////////

    if (!count)
        return 0;

    if (entry == NULL)
        return 0;

    if ((entry->type != XIN_FILE) && (entry->type != XIN_HARD_LINK))
        return 0;

    if (!entry->size)
        return 0;

    uint32_t initial_position = ftell(entry);

    if (initial_position + count > entry->size)
        count = entry->size - initial_position;

    //////////////////////////////////////////////////

    if (initial_position + count > entry->FileInfo->tmp_size)
        entry->FileInfo->tmp_size = initial_position + count;

    if (entry->FileInfo->tmp_size > entry->size)
    {
        entry->FileInfo->buffer = (uint8_t *)realloc(entry->FileInfo->buffer, (int_to_sectors(entry->FileInfo->tmp_size) + 1) * SECTOR_SIZE);
        entry->FileInfo->sector_in_use = (bool *)realloc(entry->FileInfo->sector_in_use, (int_to_sectors(entry->FileInfo->tmp_size) + 1));
    }

    uint32_t sectors_to_load = int_to_sectors(count + initial_position);

    if (sectors_to_load > int_to_sectors(entry->size))
        sectors_to_load = int_to_sectors(entry->size);

    for (int i = 0; i < sectors_to_load; i++)
    {
        if (!entry->FileInfo->sector_in_use[i])
        {
            __disk_read(entry->first_sector + i, 1, (uint16_t *)(entry->FileInfo->buffer + (i * SECTOR_SIZE)));
            entry->FileInfo->sector_in_use[i] = true;
        }
    }

    if (buf == NULL)
        return 0;

    for (char *i = (char *)(entry->FileInfo->buffer + initial_position); i < (char *)(entry->FileInfo->buffer + initial_position + count); i++, buf++)
        *(char *)buf = *i;

    fseek(entry, initial_position + count);

    return count;
}

size_t __xin_read(int fd, void *buf, size_t count)
{

    //////////////////VALIDATION///////////////////

    if (fd < 0)
        return 0;

    // XinEntry *entry = (XinEntry *)(XIN_ENTRY_TABLE + (fd * XIN_ENTRY_SIZE));
    XinEntry *entry = (XinEntry *)(XIN_FS_ENTRIES_BEGIN + (fd * XIN_ENTRY_SIZE));

    if ((entry->type != XIN_FILE) && (entry->type != XIN_HARD_LINK))
        return 0;

    uint32_t initial_position = ftell(entry);

    if (initial_position >= (entry->size - 1)) // last valid byte
        return 0;

    if (initial_position + count > entry->size)
        count = initial_position + count - (entry->size + 1); // 0 is start index

    //////////////////////////////////////////////////

    return fread(entry, buf, count);
}

size_t __xin_fwrite(XinEntry *entry, void *buf, size_t count)
{
    //////////////////VALIDATION///////////////////

    if (!count)
        return 0;

    if (entry == NULL)
        return 0;

    if ((entry->type != XIN_FILE) && (entry->type != XIN_HARD_LINK))
        return 0;

    //////////////////////////////////////////////////

    uint32_t initial_position = ftell(entry);

    if ((initial_position + count) > entry->FileInfo->tmp_size)
        entry->FileInfo->tmp_size = initial_position + count;

    entry->FileInfo->buffer = (uint8_t *)realloc(entry->FileInfo->buffer, (int_to_sectors(entry->FileInfo->tmp_size) + 1) * SECTOR_SIZE);
    entry->FileInfo->sector_in_use = (bool *)realloc(entry->FileInfo->sector_in_use, (int_to_sectors(entry->FileInfo->tmp_size) + 1));

    uint32_t sectors_to_load = int_to_sectors(count + initial_position);

    if (sectors_to_load > int_to_sectors(entry->size))
        sectors_to_load = int_to_sectors(entry->size);

    for (int i = 0; i < sectors_to_load; i++)
    {
        if (!entry->FileInfo->sector_in_use[i])
        {
            __disk_read(entry->first_sector + i, 1, (uint16_t *)(entry->FileInfo->buffer + (i * SECTOR_SIZE)));
            entry->FileInfo->sector_in_use[i] = true;
        }
    }

    fseek(entry, initial_position);

    for (char *i = (char *)(entry->FileInfo->buffer) + initial_position; i < (char *)(entry->FileInfo->buffer + initial_position + count); i++, buf++)
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

size_t __xin_write(int fd, void *buf, size_t count)
{

    ////////////////////VALIDATION/////////////////////

    if (fd < 0)
        return 0;

    if (!FileDescriptorTable[fd].is_used)
        return 0;

    XinEntry *entry = (XinEntry *)(XIN_FS_ENTRIES_BEGIN + (fd * XIN_ENTRY_SIZE));

    if ((entry->type != XIN_FILE) && (entry->type != XIN_HARD_LINK))
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
    if (!FileDescriptorTable[fd].is_used)
        return;
    FileDescriptorTable[fd].entry->FileInfo->position = new_position;
}

const uint32_t ftell(XinEntry *file)
{
    return file->FileInfo->position;
}

const uint32_t lteel(int fd)
{
    if (!FileDescriptorTable[fd].is_used)
        return -1;

    else
        return ftell(FileDescriptorTable[fd].entry);
}

XinEntry *__xin_fopen(char *file_path, char *mode)
{

    XinEntry *file = __xin_find_entry(file_path);

    if (file != NULL)
    {
        if (file->type != XIN_FILE && file->type != XIN_HARD_LINK)
            return NULL;

        file->FileInfo = (FileInformationBlock *)calloc(SIZE_OF(FileInformationBlock));
        file->FileInfo->buffer = (uint8_t *)calloc(file->size + SECTOR_SIZE);           // additional space
        file->FileInfo->sector_in_use = (bool *)calloc(int_to_sectors(file->size) + 1); // additional space

        strcpy(file->FileInfo->rights, mode);

        file->FileInfo->position = 0;
        file->FileInfo->tmp_size = 0;
        __xin_file_to_xfo_add(file);
    }

    if (bstrncmp(mode, "a", 2))
    {
        file->FileInfo = (FileInformationBlock *)calloc(SIZE_OF(FileInformationBlock));
        file->FileInfo->buffer = (uint8_t *)calloc(file->size + SECTOR_SIZE);
        file->FileInfo->sector_in_use = (bool *)calloc(int_to_sectors(file->size) + 5);

        strcpy(file->FileInfo->rights, mode);

        file->FileInfo->position = file->size;
        file->FileInfo->tmp_size = file->size;
        __xin_file_to_xfo_add(file);

        return file;
    }

    else if (bstrncmp(mode, "r", 2))
    {
        __xin_file_to_xfo_add(file);
        return file;
    }

    else if (bstrncmp(mode, "rw", 2) || bstrncmp(mode, "w", 2))
    {
        int status = __xin_file_create(file_path);

        if (status == XANIN_OK)
        {
            file = __xin_find_entry(file_path);
            file->FileInfo = (FileInformationBlock *)calloc(SIZE_OF(FileInformationBlock));
            file->FileInfo->buffer = (uint8_t *)calloc(file->size + SECTOR_SIZE);
            file->FileInfo->sector_in_use = (bool *)calloc(int_to_sectors(file->size) + 5);

            strcpy(file->FileInfo->rights, mode);

            file->FileInfo->position = 0;
            file->FileInfo->tmp_size = 0;
            __xin_file_to_xfo_add(file);
            return file;
        }

        else if (status == XIN_FILE_EXISTS)
        {
            __xin_file_to_xfo_add(file);
            return file;
        }

        else
            return NULL;
    }

    return NULL;
}

void fclose_with_given_size(XinEntry **file, uint32_t new_size)
{

    if (*file == NULL)
        return;

    if (!bstrncmp((*file)->FileInfo->rights, "r", 2)) // READ-ONLY OPTION
        __xin_file_reallocate_with_given_size((*file), new_size);

    free((*file)->FileInfo->buffer);
    free((*file)->FileInfo);

    for (int i = 0; i < XIN_OPENED_FILES_COUNTER; i++)
    {
        if (XinFilesOpened[i] == (*file))
            XinFilesOpened[i] = NULL;
    }

    (*file)->FileInfo = NULL;
    (*file) = NULL;
}

void __xin_fclose(XinEntry **file)
{
    if (*file == NULL)
        return;

    uint32_t new_size;

    if ((*file)->FileInfo->tmp_size > (*file)->size)
        new_size = (*file)->FileInfo->tmp_size;
    else
        new_size = (*file)->size;

    if (!bstrncmp((*file)->FileInfo->rights, "r", 2)) // READ-ONLY OPTION
        __xin_file_reallocate_with_given_size((*file), new_size);

    free((*file)->FileInfo->buffer);
    free((*file)->FileInfo);

    for (int i = 0; i < XIN_OPENED_FILES_COUNTER; i++)
    {
        if (XinFilesOpened[i] == (*file))
            XinFilesOpened[i] = NULL;
    }

    (*file)->FileInfo = NULL;
    (*file) = NULL;
}

void __xin_close(int fd)
{

    if (!FileDescriptorTable[fd].is_used)
        return;

    XinEntry *File = FileDescriptorTable[fd].entry;

    fclose(&File);

    FileDescriptorTable[fd].is_used = false;
}

int __xin_open(char *file_path, uint32_t options)
{

    XinEntry *file = __xin_find_entry(file_path);

    if (file != NULL && file->type != XIN_DIRECTORY && file->path[0] != '\0')
    {

        if (file->FileInfo == NULL)
            file->FileInfo = (FileInformationBlock *)calloc(SIZE_OF(FileInformationBlock));

        file->FileInfo->position = 0;
        // int fd = (int)((uint32_t)file - XIN_ENTRY_TABLE) / 64;
        int fd = (int)((uint32_t)file - (uint32_t)XIN_FS_ENTRIES_BEGIN) / SIZE_OF(XinEntry);
        FileDescriptorTable[fd].is_used = true;
        FileDescriptorTable[fd].entry = file;

        return fd;
    }
    return -1;
}

char *getline_from_ptr(char *data, int line_id)
{

    char *file_data = (char *)(data);
    char *line = (char *)calloc(200);

    int column = 0;
    int current_line = 0;
    int file_offset = 0;

    while (current_line < line_id)
    {
        memset((uint8_t *)line, 0, 200);

        while (file_data[file_offset] != '\n')
        {
            if (file_data[file_offset] == '\0')
            {
                if (current_line + 1 == line_id)
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

char *getline(XinEntry *file, int line_id)
{

    char *file_data = (char *)(file->FileInfo->buffer);
    char *line = (char *)calloc(200);

    int column = 0;
    int current_line = 0;
    int file_offset = 0;

    while (current_line < line_id)
    {
        memset((uint8_t *)line, 0, 200);

        while (file_data[file_offset] != '\n')
        {
            if (file_data[file_offset] == '\0')
            {
                if (current_line + 1 == line_id)
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

__STATUS __xin_folder_remove(char *folder_name)
{

    XinEntry *folder = __xin_find_entry(folder_name);

    if (folder == NULL)
        return XIN_ENTRY_NOT_FOUND;

    char name[XIN_MAX_PATH_LENGTH];
    uint32_t name_length;

    memcpy((uint8_t *)name, (uint8_t *)folder->path, XIN_MAX_PATH_LENGTH);
    name_length = strlen(name) - 1;

    // for (XinEntry *i = (XinEntry *)1; i < (XinEntry *)(XIN_ENTRY_TABLE + SECTOR_SIZE * 4); i++)
    for (XinEntry *i = (XinEntry *)1; i < (XinEntry *)(XIN_FS_ENTRIES_BEGIN + SECTOR_SIZE * XIN_FS_ENTRIES_SIZE); i++)
    {

        if (bstrncmp(name, i->path, name_length))
        {
            char *tmp = (char *)i;
            for (char *j = (char *)i; j < tmp + 64; j++)
                *j = '\0';
        }
    }

    memset((uint8_t *)folder->path, 0, XIN_MAX_PATH_LENGTH);
    return XANIN_OK;
}

char *xin_get_entry_name(char *path)
{
    char *tmp = (char *)calloc(XIN_MAX_PATH_LENGTH);

    if (path[0] == '/' && strlen(path) == 1)
    {
        free(tmp);
        return "/";
    }

    if (!strlen(path))
    {
        free(tmp);
        return NULL;
    }

    int index = strlen(path) - 1;

    if (path[strlen(path) - 1] == '/')
        index--;

    for (; path[index - 1] != '/'; index--)
        ;

    for (int i = 0; i < XIN_MAX_PATH_LENGTH; i++)
        tmp[i] = path[index + i];

    // if(path[strlen(path)-1] == '/')
    // tmp[strlen(tmp)] = '/';

    return tmp;
}

XinChildrenEntries *xin_get_children_entries(char *folder, bool get_hidden)
{

    if (__xin_find_entry(folder) == NULL || strlen(folder) == 0)
        return (XinChildrenEntries *)NULL;

    XinChildrenEntries *Children = (XinChildrenEntries *)calloc(SIZE_OF(XinChildrenEntries));
    Children->children = (XinEntry **)calloc(SIZE_OF(XinEntry *));

    // XinEntry *i = (XinEntry *)XIN_ENTRY_TABLE;
    XinEntry *i = (XinEntry *)XIN_FS_ENTRIES_BEGIN;

    uint32_t finded_entries = 0;

    while (i < (XinEntry *)(XIN_FS_ENTRIES_BEGIN + SECTOR_SIZE * XIN_FS_ENTRIES_SIZE))
    {
        if (bstrcmp(__xin_get_file_pf(i->path)->path, folder) && i->path[0])
        {
            if (!bstrcmp(i->path, folder))
            {
                if (xin_get_entry_name(i->path)[0] != '.' | get_hidden)
                {
                    Children->children[finded_entries] = i;
                    finded_entries++;
                    Children->children = (XinEntry **)realloc(Children->children, SIZE_OF(XinEntry *) * (finded_entries));
                }
            }
        }
        i++;
    }
    Children->how_many = finded_entries;
    return Children;
}

XinChildrenEntries *xin_get_children_entries_type(char *folder, uint8_t type)
{

    if (__xin_find_entry(folder) == NULL || strlen(folder) == 0)
        return (XinChildrenEntries *)NULL;

    XinChildrenEntries *Children = (XinChildrenEntries *)calloc(SIZE_OF(XinChildrenEntries));
    Children->children = (XinEntry **)calloc(SIZE_OF(XinEntry *));
    // XinEntry *i = (XinEntry *)XIN_ENTRY_TABLE;
    XinEntry *i = (XinEntry *)XIN_FS_ENTRIES_BEGIN;

    uint32_t finded_entries = 0;
    while (i < (XinEntry *)(XIN_FS_ENTRIES_BEGIN + SECTOR_SIZE * XIN_FS_ENTRIES_SIZE))
    {

        if (bstrcmp(__xin_get_file_pf(i->path)->path, folder) && i->path[0])
        {
            if ((!bstrcmp(i->path, folder)) && (i->type == type))
            {
                Children->children[finded_entries] = i;
                finded_entries++;
                Children->children = (XinEntry **)realloc(Children->children, SIZE_OF(XinEntry *) * (finded_entries));
            }
        }
        i++;
    }
    Children->how_many = finded_entries;
    return Children;
}

XinEntriesPack *__xin_hard_links_get(const XinEntry *const File)
{

    XinEntriesPack *Pack = (XinEntriesPack *)calloc(SIZE_OF(XinEntriesPack));
    Pack->entries = (XinEntry **)calloc(SIZE_OF(XinEntry *));

    if ((File->type != XIN_FILE) || (File == NULL))
    {
        free(Pack->entries);
        free(Pack);
        return NULL;
    }

    for (XinEntry *i = (XinEntry *)XIN_FS_ENTRIES_BEGIN; i < (XinEntry *)(XIN_FS_ENTRIES_BEGIN + SECTOR_SIZE * XIN_FS_ENTRIES_SIZE); i++)
    {
        if (i->first_sector == File->first_sector)
        {
            Pack->entries = realloc(Pack->entries, Pack->length + 1);
            Pack->entries[Pack->length++] = i;
        }
    }

    return Pack;
}

int __xin_file_size_in_sectors_get(XinEntry *File)
{
    int size = File->size / SECTOR_SIZE;
    if (File->size % SECTOR_SIZE)
        size++;
    return size;
}

bool __xin_file_to_xfo_add(XinEntry *File)
{
    for (int i = 0; i < XIN_OPENED_FILES_COUNTER; i++)
    {
        if (XinFilesOpened[i] == File) // already added
            return true;
    }

    for (int i = 0; i < XIN_OPENED_FILES_COUNTER; i++)
    {
        if (!XinFilesOpened[i])
        {
            XinFilesOpened[i] = File;
            return true;
        }
    }

    return false;
}

void __xin_all_files_close(void)
{
    EFlags Flags = eflags_get();
    interrupt_disable();

    for (int i = 0; i < XIN_OPENED_FILES_COUNTER; i++)
    {
        if (XinFilesOpened[i] != NULL)
            fclose(&XinFilesOpened[i]);
    }

    if (Flags.intf)
        interrupt_enable();
}

__STATUS __xin_link_remove(const char *linkname)
{
    XinEntry *file = __xin_find_entry(linkname);

    if (file != NULL && file->type == XIN_LINK)
    {
        memset((uint8_t *)file, 0x0, SIZE_OF(XinEntry));
        return XANIN_OK;
    }

    return XANIN_ERROR;
}
__STATUS __xin_link_create(char *file_name, char *link_name)
{
    XinEntry *file = __xin_find_entry(file_name);

    if (file == NULL)
        return XIN_ENTRY_NOT_FOUND;

    XinEntry *link = __xin_find_free_entry();
    memcpy((uint8_t *)link, (uint8_t *)file, SIZE_OF(XinEntry));

    link->type = XIN_HARD_LINK;

    for (int i = 0; i < XIN_MAX_PATH_LENGTH; i++)
        link->path[i] = file->path[i];

    if (link_name[0] != '/')
        link_name = __xin_path_get(link_name);

    for (int i = 0; i < XIN_MAX_PATH_LENGTH; i++)
        link->path[i] = link_name[i];

    return XANIN_OK;
}

__STATUS __xin_copy(char *file_name, char *new_file_name)
{

    XinEntry *entry = __xin_find_entry(file_name);

    if (entry == NULL)
        return XIN_ENTRY_NOT_FOUND;

    __STATUS status = __xin_file_create(new_file_name);

    if (status != XANIN_OK)
        return status;

    XinEntry *file = fopen(file_name, "r");
    XinEntry *file_created = fopen(new_file_name, "rw");

    char *entry_data = (char *)calloc(file->size);

    fread(file, entry_data, file->size);
    fwrite(file_created, entry_data, file->size);

    free(entry_data);
    fclose(&file);
    fclose(&file_created);

    return XANIN_OK;
}

__STATUS __xin_entry_move(char *entry_name, char *new_name)
{

    if ((__xin_get_file_pf(new_name) == NULL) || (__xin_find_entry(entry_name) == NULL))
        return XIN_ENTRY_NOT_FOUND;

    XinEntry *entry = __xin_find_entry(entry_name);

    int i;
    if (new_name[0] == '/')
    {
        for (i = 0; new_name[i] != '\0'; i++)
            entry->path[i] = new_name[i];
    }
    else
    {
        for (i = 0; __xin_path_get(new_name)[i] != '\0'; i++)
            entry->path[i] = __xin_path_get(new_name)[i];
    }

    entry->path[i] = '\0';

    return XANIN_OK;
}
