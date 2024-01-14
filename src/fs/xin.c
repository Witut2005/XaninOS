
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

static XinFileSystemData XinFsData; // XinFS DATA SINGLETONE

#define XIN_FS_BEGIN (XinFsData.tables)

#define XIN_FS_PTRS_TABLE_BEGIN (XinFsData.tables)
#define XIN_FS_ENTRIES_TABLE_BEGIN (XinFsData.tables + (SECTOR_SIZE * XinFsData.ptrs_size))
#define XIN_FS_ENTRIES_TABLE_END (XinFsData.tables + (SECTOR_SIZE * (XinFsData.ptrs_size + XinFsData.entries_size)))

#define XIN_FS_ENTRIES_SIZE (XinFsData.entries_size)
#define XIN_FS_PTRS_SIZE (XinFsData.ptrs_size)

bool __xin_entry_alignment_check(XinEntry *Entry)
{
    return (uint32_t)Entry % SIZE_OF(XinEntry) == 0;
}

bool __xin_entry_address_check(XinEntry *Entry)
{
    return Entry >= (XinEntry *)XIN_FS_ENTRIES_TABLE_BEGIN && Entry < (XinEntry *)XIN_FS_ENTRIES_TABLE_END;
}

bool __xin_entry_validation_check(XinEntry *Entry)
{
    return __xin_entry_alignment_check(Entry) && __xin_entry_address_check(Entry);
}

char *__xin_absolute_path_get(char *rpath, char *buf, XIN_FS_ENTRY_TYPES type)
{
    strcpy(buf, rpath);

    if (__xin_is_relative_path_used(rpath))
        strconcat(XinFsData.current_folder, buf);

    uint32_t buf_len = strlen(buf);

    if (type == XIN_DIRECTORY && buf[buf_len - 1] != '/')
        buf[buf_len] = '/'; // append at the end, so buf must be a little bit bigger

    return buf;
}

bool __xin_is_relative_path_used(char *path)
{
    return path[0] != XIN_SYSTEM_FOLDER;
}

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

uint8_t *__xin_fs_entries_get(void)
{
    return (uint8_t *)(XinFsData.tables + (SECTOR_SIZE * XinFsData.ptrs_size));
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

char *__xin_current_directory_get(char *buf)
{
    memcpy(buf, XinFsData.current_folder, XIN_MAX_PATH_LENGTH);
    return buf;
}

uint8_t *__xin_find_free_pointer(void)
{
    for (xin_ptr_t *i = (xin_ptr_t *)XIN_FS_PTRS_TABLE_BEGIN; (uint32_t)i < (uint32_t)(XIN_FS_PTRS_TABLE_BEGIN + (XIN_FS_PTRS_SIZE * SECTOR_SIZE)); i++)
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

void __xin_entry_resize(XinEntry *Entry, uint32_t new_size)
{

    if (__xin_entry_validation_check(Entry) == false)
        return;

    uint8_t *xin_pointer_table_entry = (uint8_t *)(XIN_FS_PTRS_TABLE_BEGIN + Entry->first_sector);
    uint32_t sectors = Entry->size / SECTOR_SIZE + (Entry->size % SECTOR_SIZE != 0 ? 1 : 0);

    for (int i = 0; i < sectors; i++)
        xin_pointer_table_entry[i] = XIN_UNALLOCATED;

    sectors = new_size / SECTOR_SIZE + (new_size % SECTOR_SIZE != 0 ? 1 : 0);
    xin_pointer_table_entry = __xin_find_free_pointer_with_given_size(sectors);

    for (int i = 0; i < sectors; i++)
        xin_pointer_table_entry[i] = XIN_ALLOCATED;
}

/* DIRECTORY AND FILES */
XinEntry *__xin_find_entry(char *entryname)
{

    char entrypath[XIN_MAX_PATH_LENGTH + 1] = {0};

    if (!strlen(entryname)) // if path is empty
        return NULL;

    if (entryname[0] == XIN_SYSTEM_FOLDER && entryname[1] == XIN_SYSTEM_FOLDER)
        return __xin_find_entry(XIN_SYSTEM_FOLDER_STR);

    __xin_absolute_path_get(entryname, entrypath, XIN_DIRECTORY); // treat all Entries as directories
    uint32_t entrypath_len = strlen(entrypath);

    for (XinEntry *i = (XinEntry *)XIN_FS_ENTRIES_TABLE_BEGIN; (uint32_t)i < (uint32_t)(XIN_FS_ENTRIES_TABLE_BEGIN + (SECTOR_SIZE * XIN_FS_ENTRIES_SIZE)); i++)
    {
        // check if given folder exists
        if (bstrcmp(entrypath, i->path))
            return i;

        // check if given file exists
        entrypath[entrypath_len - 1] = '\0';

        if (bstrcmp(entrypath, i->path))
            return i;

        entrypath[entrypath_len - 1] = '/';
    }

    return NULL;
}

XinEntry *__xin_entry_pf_get(char *name) // pf = parent folder
{
    const XinEntry *Entry = __xin_find_entry(name);

    if (__xin_entry_validation_check(Entry) == false)
        return NULL;

    if (bstrcmp(Entry->path, XIN_SYSTEM_FOLDER_STR))
        return Entry;

    char parent_folder[XIN_MAX_PATH_LENGTH + 1] = {0};

    int i;
    for (i = strlen(Entry->path) - 1 - (Entry->type == XIN_DIRECTORY ? 1 : 0); Entry->path[i] != '/'; i--)
        ;

    for (int j = 0; j <= i; j++)
        parent_folder[j] = Entry->path[j];

    XinEntry *ParentEntry = __xin_find_entry(parent_folder);

    return ParentEntry != NULL ? ParentEntry : NULL;
}

XinEntry *__xin_find_free_entry(void)
{
    for (XinEntry *i = (XinEntry *)XIN_FS_ENTRIES_TABLE_BEGIN; i < (XinEntry *)XIN_FS_ENTRIES_TABLE_END; i++)
        if (i->path[0] == '\0')
            return i;

    return NULL;
}

void __xin_free_temporary_data(XinEntry *File)
{
    if (__xin_entry_validation_check(File) == false)
        return;

    free(File->FileInfo->buffer);
    free(File->FileInfo);
}

__STATUS __xin_folder_change(char *foldername)
{

    if (strlen(foldername) > XIN_MAX_PATH_LENGTH)
        return XANIN_ERROR;

    else if (bstrcmp(foldername, "."))
        return XANIN_OK;

    else if (bstrcmp(foldername, ".."))
    {
        XinEntry *CurrentFolderParent = __xin_entry_pf_get(XinFsData.current_folder);
        __xin_folder_change(CurrentFolderParent != NULL ? CurrentFolderParent->path : "/");
        return XANIN_OK;
    }

    while (memcmp(foldername, "../", 3))
    {
        if (bstrcmp(XinFsData.current_folder, XIN_SYSTEM_FOLDER_STR))
            return XANIN_ERROR;
        else
        {
            XinEntry *CurrentFolderParent = __xin_entry_pf_get(XinFsData.current_folder);
            if (CurrentFolderParent != NULL)
                strncpy(XinFsData.current_folder, CurrentFolderParent->path, XIN_MAX_PATH_LENGTH);
            else
                __xin_folder_change(XIN_SYSTEM_FOLDER_STR);
        }
        foldername = foldername + 3;
    }

    while (memcmp(foldername, "./", 2))
        foldername += 2;

    XinEntry *NewFolder = __xin_find_entry(foldername);

    if (NewFolder != NULL)
        strcpy(XinFsData.current_folder, NewFolder->path);

    else
        return XANIN_ERROR;

    return XANIN_OK;
}
void __xin_init()
{
    __xin_detect_file_system();
    __xin_fs_tables_set(kcalloc(SECTOR_SIZE * (XinFsData.ptrs_size + XinFsData.entries_size)));
    __xin_fs_load_tables_from_disk();

    __xin_folder_change(XIN_SYSTEM_FOLDER_STR);

    for (xin_ptr_t *i = XIN_FS_PTRS_TABLE_BEGIN; i < XIN_FS_PTRS_TABLE_BEGIN + 0x280; i++)
    {
        if (*i == XIN_UNALLOCATED)
            *i = XIN_EOF;
    }
}

void __xin_tables_update(void)
{
    __disk_sectors_write(ATA_FIRST_BUS, ATA_MASTER, XinFsData.first_sector, XIN_FS_PTRS_SIZE, (uint16_t *)XIN_FS_ENTRIES_TABLE_BEGIN);
    __disk_sectors_write(ATA_FIRST_BUS, ATA_MASTER, XinFsData.first_sector + XIN_FS_PTRS_SIZE, XIN_FS_ENTRIES_SIZE, (uint16_t *)(XIN_FS_ENTRIES_TABLE_BEGIN));
}

__STATUS __xin_entry_create(XinEntryCreateArgs *Args, XIN_FS_ENTRY_TYPES type)
{
    char entrypath[XIN_MAX_PATH_LENGTH + 1] = {'\0'};

    XinEntry *Entry = __xin_find_free_entry();

    __xin_absolute_path_get(Args->entryname, entrypath, type);

    if (__xin_find_entry(entrypath) != NULL)
        return XIN_FILE_EXISTS;

    if (type == XIN_HARD_LINK)
    {
        XinEntry *File = __xin_find_entry(Args->linksource);

        if (File == NULL)
            return XIN_ENTRY_NOT_FOUND;

        memcpy((uint8_t *)Entry, (uint8_t *)File, SIZE_OF(XinEntry));

        strncpy(Entry->path, entrypath, XIN_MAX_PATH_LENGTH);
        Entry->type = XIN_HARD_LINK;
    }

    else
    {

        CmosTime Time;
        time_get(&Time);

        memcpy(Entry->path, entrypath, XIN_MAX_PATH_LENGTH);
        Entry->creation_date = Entry->modification_date = time_extern_date(&Time);
        Entry->creation_time = Entry->modification_time = time_extern_time(&Time);
        Entry->FileInfo = NULL;
        Entry->permissions = PERMISSION_MAX;
        Entry->size = 0;
        Entry->type = type;
        Entry->first_sector = XIN_FIRST_SECTOR_NOT_DEFINED;
    }

    __xin_tables_update();

    return XANIN_OK;
}

__STATUS __xin_folder_create(char *foldername)
{
    XinEntryCreateArgs Args = {foldername, NULL};
    __xin_entry_create(&Args, XIN_DIRECTORY);
    return XANIN_OK;
}

__STATUS __xin_file_create(char *filename)
{
    XinEntryCreateArgs Args = {filename, NULL};
    return __xin_entry_create(&Args, XIN_FILE);
}

void __xin_entry_modification_fields_update(XinEntry *Entry)
{
    if (__xin_entry_validation_check(Entry) == false)
        return;

    CmosTime Time;
    time_get(&Time);

    Entry->modification_date = time_extern_date(&Time);
    Entry->modification_time = time_extern_time(&Time);
}

int __xin_file_reallocate_with_given_size(XinEntry *File, uint32_t size)
{
    if (__xin_entry_validation_check(File) == false)
        return XANIN_ERROR;

    uint8_t *buf = (uint8_t *)calloc(int_to_sectors(size) * SECTOR_SIZE);
    memcpy(buf, File->FileInfo->buffer, size);
    memset(&buf[size], 0, (int_to_sectors(size) * SECTOR_SIZE) - size);

    uint32_t number_of_sectors_to_deallocate = int_to_sectors(File->size);
    if (!number_of_sectors_to_deallocate)
        number_of_sectors_to_deallocate++;

    uint32_t number_of_sectors_to_allocate = int_to_sectors(size);
    if (!number_of_sectors_to_allocate)
        number_of_sectors_to_allocate++;

    // uint8_t *entry_deallocate = (uint8_t *)(File->first_sector + XIN_ENTRY_POINTERS);
    uint8_t *entry_deallocate = (uint8_t *)(File->first_sector + XIN_FS_ENTRIES_TABLE_BEGIN);
    for (int i = 0; i < number_of_sectors_to_deallocate; i++)
        entry_deallocate[i] = XIN_UNALLOCATED;

    /* write Entry to xin Entry pointers table */
    uint8_t *entry_ptrs = __xin_find_free_pointer_with_given_size(number_of_sectors_to_allocate);

    for (int i = 0; i < number_of_sectors_to_allocate - 1; i++)
        entry_ptrs[i] = XIN_ALLOCATED;

    entry_ptrs[number_of_sectors_to_allocate - 1] = XIN_EOF;

    /* write Entry to xin Entry data table */

    __xin_entry_modification_fields_update(File);
    File->size = size;

    // get all hard links
    XinEntriesPack *HardLinks = __xin_hard_links_get(File);

    for (int i = 0; i < HardLinks->length; i++)
    {
        __xin_entry_modification_fields_update(HardLinks->Entries[i]);
        HardLinks->Entries[i]->first_sector = entry_ptrs - XIN_FS_PTRS_TABLE_BEGIN;
        HardLinks->Entries[i]->size = size;
    }

    // File->first_sector = write_entry - XIN_ENTRY_POINTERS;
    File->first_sector = entry_ptrs - XIN_FS_PTRS_TABLE_BEGIN;

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
        for (xin_ptr_t *i = entry_data->first_sector + XIN_FS_PTRS_TABLE_BEGIN;
             i < entry_data->first_sector + XIN_FS_PTRS_TABLE_BEGIN + sectors_used; i++)
            *i = XIN_UNALLOCATED;

        char *tmp = (char *)entry_data;

        for (int i = 0; i < 64; i++)
            tmp[i] = '\0';
    }

    return XANIN_OK;
}

size_t __xin_fread(XinEntry *Entry, void *buf, size_t count)
{

    //////////////////VALIDATION///////////////////

    if (__xin_entry_validation_check(Entry) == false)
        return 0;

    if (Entry->FileInfo == NULL)
        return 0;

    if (!count)
        return 0;

    if ((Entry->type != XIN_FILE) && (Entry->type != XIN_HARD_LINK))
        return 0;

    if (!Entry->size)
        return 0;

    if (Entry->first_sector == XIN_FIRST_SECTOR_NOT_DEFINED)
        return 0;

    uint32_t initial_position = ftell(Entry);

    if (initial_position + count > Entry->size)
        count = Entry->size - initial_position;

    //////////////////////////////////////////////////

    if (initial_position + count > Entry->FileInfo->tmp_size)
        Entry->FileInfo->tmp_size = initial_position + count;

    if (Entry->FileInfo->tmp_size > Entry->size)
    {
        Entry->FileInfo->buffer = (uint8_t *)realloc(Entry->FileInfo->buffer, (int_to_sectors(Entry->FileInfo->tmp_size) + 1) * SECTOR_SIZE);
        Entry->FileInfo->sector_in_use = (bool *)realloc(Entry->FileInfo->sector_in_use, (int_to_sectors(Entry->FileInfo->tmp_size) + 1));
    }

    uint32_t sectors_to_load = int_to_sectors(count + initial_position);

    if (sectors_to_load > int_to_sectors(Entry->size))
        sectors_to_load = int_to_sectors(Entry->size);

    for (int i = 0; i < sectors_to_load; i++)
    {
        if (!Entry->FileInfo->sector_in_use[i])
        {
            __disk_read(Entry->first_sector + i, 1, (uint16_t *)(Entry->FileInfo->buffer + (i * SECTOR_SIZE)));
            Entry->FileInfo->sector_in_use[i] = true;
        }
    }

    if (buf == NULL)
        return 0;

    for (char *i = (char *)(Entry->FileInfo->buffer + initial_position); i < (char *)(Entry->FileInfo->buffer + initial_position + count); i++, buf++)
        *(char *)buf = *i;

    fseek(Entry, initial_position + count);

    return count;
}

size_t __xin_read(int fd, void *buf, size_t count)
{

    //////////////////VALIDATION///////////////////

    if (fd < 0)
        return 0;

    if (FileDescriptorTable[fd].is_used == false)
        return 0;

    // XinEntry *Entry = (XinEntry *)(XIN_ENTRY_TABLE + (fd * XIN_ENTRY_SIZE));
    XinEntry *Entry = (XinEntry *)(XIN_FS_ENTRIES_TABLE_BEGIN + (fd * XIN_ENTRY_SIZE));

    // if ((Entry->type != XIN_FILE) && (Entry->type != XIN_HARD_LINK))
    //     return 0;

    // uint32_t initial_position = ftell(Entry);

    // if (initial_position >= (Entry->size - 1)) // last valid byte
    //     return 0;

    // if (initial_position + count > Entry->size)
    //     count = initial_position + count - (Entry->size + 1); // 0 is start index

    //////////////////////////////////////////////////

    return fread(Entry, buf, count);
}

size_t __xin_fwrite(XinEntry *Entry, void *buf, size_t count)
{
    //////////////////VALIDATION///////////////////

    if (__xin_entry_validation_check(Entry) == false)
        return 0;

    if (Entry->FileInfo == NULL)
        return 0;

    if (!count)
        return 0;

    if (Entry == NULL)
        return 0;

    if ((Entry->type != XIN_FILE) && (Entry->type != XIN_HARD_LINK))
        return 0;

    if (Entry->first_sector == XIN_FIRST_SECTOR_NOT_DEFINED)
        return 0;

    //////////////////////////////////////////////////

    uint32_t initial_position = ftell(Entry);

    if ((initial_position + count) > Entry->FileInfo->tmp_size)
        Entry->FileInfo->tmp_size = initial_position + count;

    Entry->FileInfo->buffer = (uint8_t *)realloc(Entry->FileInfo->buffer, (int_to_sectors(Entry->FileInfo->tmp_size) + 1) * SECTOR_SIZE);
    Entry->FileInfo->sector_in_use = (bool *)realloc(Entry->FileInfo->sector_in_use, (int_to_sectors(Entry->FileInfo->tmp_size) + 1));

    uint32_t sectors_to_load = int_to_sectors(count + initial_position);

    if (sectors_to_load > int_to_sectors(Entry->size))
        sectors_to_load = int_to_sectors(Entry->size);

    for (int i = 0; i < sectors_to_load; i++)
    {
        if (!Entry->FileInfo->sector_in_use[i])
        {
            __disk_read(Entry->first_sector + i, 1, (uint16_t *)(Entry->FileInfo->buffer + (i * SECTOR_SIZE)));
            Entry->FileInfo->sector_in_use[i] = true;
        }
    }

    fseek(Entry, initial_position);

    for (char *i = (char *)(Entry->FileInfo->buffer) + initial_position; i < (char *)(Entry->FileInfo->buffer + initial_position + count); i++, buf++)
    {
        *i = *(char *)buf;
        fseek(Entry, ftell(Entry) + 1);
    }

    __xin_entry_modification_fields_update(Entry);
    return count;
}

size_t __xin_write(int fd, void *buf, size_t count)
{

    ////////////////////VALIDATION/////////////////////

    if (fd < 0)
        return 0;

    if (FileDescriptorTable[fd].is_used == false)
        return 0;

    XinEntry *Entry = (XinEntry *)(XIN_FS_ENTRIES_TABLE_BEGIN + (fd * XIN_ENTRY_SIZE));

    // if ((Entry->type != XIN_FILE) && (Entry->type != XIN_HARD_LINK))
    //     return 0;

    //////////////////////////////////////////////////

    return fwrite(Entry, buf, count);
}

void fseek(XinEntry *file, uint32_t new_position)
{
    if (__xin_entry_validation_check(file) == true)
        file->FileInfo->position = new_position;
}

void lseek(int fd, uint32_t new_position)
{
    if (FileDescriptorTable[fd].is_used == false || __xin_entry_validation_check(FileDescriptorTable[fd].Entry) == false)
        return;
    FileDescriptorTable[fd].Entry->FileInfo->position = new_position;
}

const uint32_t ftell(XinEntry *file)
{
    return file->FileInfo->position;
}

const uint32_t lteel(int fd)
{
    if (FileDescriptorTable[fd].is_used == false || __xin_entry_validation_check(FileDescriptorTable[fd].Entry) == false)
        return -1;
    return ftell(FileDescriptorTable[fd].Entry);
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
    if (__xin_entry_validation_check(*file) == false)
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

    XinEntry *File = FileDescriptorTable[fd].Entry;

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
        int fd = (int)((uint32_t)file - (uint32_t)XIN_FS_ENTRIES_TABLE_BEGIN) / SIZE_OF(XinEntry);
        FileDescriptorTable[fd].is_used = true;
        FileDescriptorTable[fd].Entry = file;

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
    for (XinEntry *i = (XinEntry *)1; i < (XinEntry *)(XIN_FS_ENTRIES_TABLE_BEGIN + SECTOR_SIZE * XIN_FS_ENTRIES_SIZE); i++)
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

char *__xin_entry_name_extern(char *path)
{
    char *tmp = (char *)calloc(XIN_MAX_PATH_LENGTH);

    if (path[0] == '/' && strlen(path) == 1)
    {
        free(tmp);
        return XIN_SYSTEM_FOLDER_STR;
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

    return tmp;
}

XinChildrenEntries *xin_children_entries_get(char *folder, bool get_hidden)
{

    if (__xin_find_entry(folder) == NULL || strlen(folder) == 0)
        return (XinChildrenEntries *)NULL;

    XinChildrenEntries *Children = (XinChildrenEntries *)calloc(SIZE_OF(XinChildrenEntries));
    Children->Children = (XinEntry **)calloc(SIZE_OF(XinEntry *));

    // XinEntry *i = (XinEntry *)XIN_ENTRY_TABLE;
    XinEntry *i = (XinEntry *)XIN_FS_ENTRIES_TABLE_BEGIN;

    uint32_t finded_entries = 0;

    while (i < (XinEntry *)(XIN_FS_ENTRIES_TABLE_BEGIN + SECTOR_SIZE * XIN_FS_ENTRIES_SIZE))
    {
        if (bstrcmp(__xin_entry_pf_get(i->path)->path, folder) && i->path[0])
        {
            if (!bstrcmp(i->path, folder))
            {
                if (__xin_entry_name_extern(i->path)[0] != '.' | get_hidden)
                {
                    Children->Children[finded_entries] = i;
                    finded_entries++;
                    Children->Children = (XinEntry **)realloc(Children->Children, SIZE_OF(XinEntry *) * (finded_entries));
                }
            }
        }
        i++;
    }
    Children->how_many = finded_entries;
    return Children;
}

XinChildrenEntries *xin_children_entries_type_get(char *folder, uint8_t type)
{

    if (__xin_find_entry(folder) == NULL || strlen(folder) == 0)
        return (XinChildrenEntries *)NULL;

    XinChildrenEntries *Children = (XinChildrenEntries *)calloc(SIZE_OF(XinChildrenEntries));
    Children->Children = (XinEntry **)calloc(SIZE_OF(XinEntry *));
    // XinEntry *i = (XinEntry *)XIN_ENTRY_TABLE;
    XinEntry *i = (XinEntry *)XIN_FS_ENTRIES_TABLE_BEGIN;

    uint32_t finded_entries = 0;
    while (i < (XinEntry *)(XIN_FS_ENTRIES_TABLE_BEGIN + SECTOR_SIZE * XIN_FS_ENTRIES_SIZE))
    {

        if (bstrcmp(__xin_entry_pf_get(i->path)->path, folder) && i->path[0])
        {
            if ((!bstrcmp(i->path, folder)) && (i->type == type))
            {
                Children->Children[finded_entries] = i;
                finded_entries++;
                Children->Children = (XinEntry **)realloc(Children->Children, SIZE_OF(XinEntry *) * (finded_entries));
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
    Pack->Entries = (XinEntry **)calloc(SIZE_OF(XinEntry *));

    if ((File->type != XIN_FILE) || (File == NULL))
    {
        free(Pack->Entries);
        free(Pack);
        return NULL;
    }

    for (XinEntry *i = (XinEntry *)XIN_FS_ENTRIES_TABLE_BEGIN; i < (XinEntry *)(XIN_FS_ENTRIES_TABLE_BEGIN + SECTOR_SIZE * XIN_FS_ENTRIES_SIZE); i++)
    {
        if (i->first_sector == File->first_sector)
        {
            Pack->Entries = realloc(Pack->Entries, Pack->length + 1);
            Pack->Entries[Pack->length++] = i;
        }
    }

    return Pack;
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
__STATUS __xin_link_create(char *filename, char *linkname)
{
    XinEntryCreateArgs Args = {linkname, filename};
    __xin_entry_create(&Args, XIN_HARD_LINK);
    return XANIN_OK;
}

__STATUS __xin_copy(char *file_name, char *new_file_name)
{

    XinEntry *Entry = __xin_find_entry(file_name);

    if (Entry == NULL)
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

__STATUS __xin_entry_move(char *entryname, char *destname)
{

    if ((__xin_entry_pf_get(destname) == NULL) || (__xin_find_entry(entryname) == NULL))
        return XIN_ENTRY_NOT_FOUND;

    XinEntry *Entry = __xin_find_entry(entryname);

    char destpath[XIN_MAX_PATH_LENGTH + 1] = {0};

    __xin_absolute_path_get(destname, destpath, Entry->type);
    memcpy(Entry->path, destpath, XIN_MAX_PATH_LENGTH);

    return XANIN_OK;
}
