
#include <fs/xin.h>
#include <stddef.h>
#include <lib/libc/hal.h>
#include <lib/libc/file.h>
#include <sys/log/syslog.h>
#include <lib/libc/memory.h>
#include <lib/libc/string.h>
#include <lib/libc/colors.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/stdlibx.h>
#include <lib/libcpp/string.h>
#include <lib/libcpp/lexer.hpp>
#include <lib/libcpp/container/vector.hpp>
#include <lib/libcpp/memory.hpp>
#include <sys/devices/com/com.h>
#include <sys/devices/hda/disk.h>
#include <sys/call/xanin_sys/handler/xanin_sys.h>
#include <sys/pmmngr/alloc.h>

#define XIN_OPENED_FILES_COUNTER 100

XinFileDescriptor* FileDescriptorTable;
static XinEntry* XinFilesOpened[XIN_OPENED_FILES_COUNTER];

static XinFileSystemData XinFsData; // XinFS DATA SINGLETONE

#define XIN_FS_BEGIN (XinFsData.tables)

#define XIN_FS_PTRS_TABLE_BEGIN (XinFsData.tables)
#define XIN_FS_PTRS_TABLE_END (XinFsData.tables + (SECTOR_SIZE * XinFsData.ptrs_size))

#define XIN_FS_ENTRIES_TABLE_BEGIN (XinFsData.tables + (SECTOR_SIZE * XinFsData.ptrs_size))
#define XIN_FS_ENTRIES_TABLE_END (XinFsData.tables + (SECTOR_SIZE * (XinFsData.ptrs_size + XinFsData.entries_size)))

#define XIN_FS_ENTRIES_SIZE (XinFsData.entries_size)
#define XIN_FS_PTRS_SIZE (XinFsData.ptrs_size)

#define XIN_FS_ITERATE_OVER_ENTRY_TABLE(iterator) for (XinEntry *iterator = (XinEntry *)XIN_FS_ENTRIES_TABLE_BEGIN; iterator < (XinEntry *)XIN_FS_ENTRIES_TABLE_END; iterator++)
#define XIN_FS_ITERATE_OVER_PTRS_TABLE(iterator) for (xin_ptr_t *iterator = (xin_ptr_t *)XIN_FS_PTRS_TABLE_BEGIN; iterator < (xin_ptr_t *)XIN_FS_PTRS_TABLE_END; iterator++)

/* -------------------------------------------------------------------------------------- */

static constexpr XIN_FS_ENTRY_TYPES xin_entry_type(uint8_t type) { return (XIN_FS_ENTRY_TYPES)type; }

using std::string;

std::string __nxin_absolute_path_get(const std::string& name)
{
    if (__xin_is_relative_path_used(name.c_str())) {
        return std::string(__xin_current_directory_get(std::UniquePtr((char*)kcalloc(XIN_MAX_PATH_LENGTH)).get())) + name;
    }
    return name;
}

std::string __nxin_entry_name_extern(const std::string& path)
{
    if (auto delim_index = path.last_of("/"); delim_index != -1) {
        return std::string(path.cbegin(), path.cbegin() + delim_index);
    }
    return path;
}

std::string __nxin_path_parse(std::string path)
{

    auto conditional_goto_to_parent_folder = [](bool cond, const std::string& path, int start_index) -> std::string {
        if (cond) {
            if (auto delim_index = path.last_of("/", start_index); delim_index != std::string::npos) {
                return path.substr(0, delim_index + 1); //we dont want to delete '/' char
            }
            else {
                return "/";
            }
        }
        return path;
    };

    if (__xin_is_relative_path_used(path.c_str())) {
        path = std::string(__xin_current_directory_get(std::UniquePtr((char*)kcalloc(XIN_MAX_PATH_LENGTH)).get())) + path;
    }

    std::BaseLexer lexer(path);
    path = "";

    while (lexer.all_parsed() == false)
    {
        auto result = lexer.consume_until(std::vector<std::string>({ "../", "./" }), true);
        path = path + result.first;// + std::string("/"); //TODO char + operator

        path = conditional_goto_to_parent_folder(result.second == "../", path, -2);
    }

    if (path.substr(-2) == "/.") {
        return path.substr(0, path.length() - 2);
    }
    return conditional_goto_to_parent_folder(path.substr(-2) == "..", path, -4); // check if path ends with .. (nicho/ble/ble/..)
}

extern "C"
{

    void __xin_detect_file_system(void)
    {
        disk_read_bytes(ATA_FIRST_BUS, ATA_MASTER, 0, 2, 12, (uint8_t*)&XinFsData);
    }

    XinFileSystemData __xin_fs_data_get(void)
    {
        return XinFsData;
    }

    uint8_t* __xin_fs_tables_get(void)
    {
        return XinFsData.tables;
    }

    uint8_t* __xin_fs_ptrs_get(void)
    {
        return XinFsData.tables;
    }

    xin_ptr_t* __xin_fs_ptrs_end_get(void)
    {
        return XinFsData.tables + (SECTOR_SIZE * XinFsData.ptrs_size);
    }

    uint32_t __xin_fs_ptrs_size_get(void)
    {
        return XinFsData.ptrs_size;
    }

    uint8_t* __xin_fs_entries_get(void)
    {
        return (uint8_t*)(XinFsData.tables + (SECTOR_SIZE * XinFsData.ptrs_size));
    }

    XinEntry* __xin_fs_entries_end_get(void)
    {
        return (XinEntry*)(XinFsData.tables + (SECTOR_SIZE * (XinFsData.ptrs_size + XinFsData.entries_size)));
    }

    uint32_t __xin_fs_entries_size_get(void)
    {
        return XinFsData.entries_size;
    }

    void __xin_fs_tables_set(uint8_t* tables)
    {
        XinFsData.tables = tables;
    }

    void __xin_fs_load_tables_from_disk(void) // oj calkiem risky
    {
        disk_read(XinFsData.first_sector, XinFsData.ptrs_size + XinFsData.entries_size, (uint16_t*)XinFsData.tables);
    }

    void __xin_tables_update(void)
    {
        disk_sectors_write(ATA_FIRST_BUS, ATA_MASTER, XinFsData.first_sector, XIN_FS_PTRS_SIZE, (uint16_t*)XIN_FS_ENTRIES_TABLE_BEGIN);
        disk_sectors_write(ATA_FIRST_BUS, ATA_MASTER, XinFsData.first_sector + XIN_FS_PTRS_SIZE, XIN_FS_ENTRIES_SIZE, (uint16_t*)(XIN_FS_ENTRIES_TABLE_BEGIN));
    }

    void __xin_init(void)
    {
        __xin_detect_file_system();
        __xin_fs_tables_set((uint8_t*)kcalloc(SECTOR_SIZE * (XinFsData.ptrs_size + XinFsData.entries_size)));
        __xin_fs_load_tables_from_disk();

        __xin_folder_change(XIN_SYSTEM_FOLDER_STR);

        for (xin_ptr_t* i = XIN_FS_PTRS_TABLE_BEGIN; i < XIN_FS_PTRS_TABLE_BEGIN + 0x280; i++)
        {
            if (*i == XIN_UNALLOCATED)
                *i = XIN_EOF;
        }
        dbg_success("XinFs", "xin_fs::init success");
    }

    /* -------------------------------------------------------------------------------------- */

    bool __xin_entry_alignment_check(const XinEntry* Entry)
    {
        return (uint32_t)Entry % sizeof(XinEntry) == 0;
    }

    bool __xin_entry_address_check(const XinEntry* Entry)
    {
        return Entry >= (XinEntry*)XIN_FS_ENTRIES_TABLE_BEGIN && Entry < (XinEntry*)XIN_FS_ENTRIES_TABLE_END;
    }

    bool __xin_entry_validation_check(const XinEntry* Entry)
    {
        return __xin_entry_alignment_check(Entry) && __xin_entry_address_check(Entry);
    }

    bool __xin_is_relative_path_used(const char* path)
    {
        return path[0] != XIN_SYSTEM_FOLDER;
    }

    // in buf we have abs path
    // in ret we have parsed abs path
    char* __xin_absolute_path_get(const char* rpath, char* buf, XIN_FS_ENTRY_TYPES type)
    {
        EFlags Flags;
        INTERRUPTS_OFF(&Flags)

            if (strlen(rpath) == 0)
            {
                INTERRUPTS_ON(&Flags);
                return __xin_current_directory_get(buf);
            }

        strcpy(buf, rpath);

        if (__xin_is_relative_path_used(rpath)) {
            strcat(STRCAT_SRC_FIRST, buf, XinFsData.current_folder);
        }

        char ret[XIN_MAX_PATH_LENGTH + 1] = { '\0' };

        int ret_index = 0;
        int buf_index = 0;

        while (buf[buf_index] != '\0')
        {
            if (bmemcmp(&buf[buf_index], "./", 2)) {
                buf_index += 2;
            }

            if (bmemcmp(&buf[buf_index], "../", 3))
            {
                __xin_path_pf_extern(ret, ret);
                buf_index += 3;
                ret_index = strlen(ret);
            }
            else
            {
                ret[ret_index] = buf[buf_index];

                ret_index++;
                buf_index++;
            }
        }

        ret[ret_index] = '\0';

        memcpy(buf, ret, XIN_MAX_PATH_LENGTH);
        uint32_t buf_len = strlen(buf);

        if (type == XIN_DIRECTORY && rpath[strlen(rpath) - 1] != '/') {
            buf[buf_len] = '/'; // append at the end, so buf must be a little bit bigger
            buf[buf_len + 1] = '\0';
        }

        INTERRUPTS_ON(&Flags);
        return buf;
    }

    char* __xin_entry_name_extern(char* path, char* buf)
    {
        if (path[0] == '/' && strlen(path) == 1) {
            return XIN_SYSTEM_FOLDER_STR;
        }

        if (!strlen(path)) {
            return NULL;
        }

        char* pathptr = char_find_from_end(path, path[strlen(path) - 1] == '/' ? 1 : 0, '/');
        memcpy(buf, pathptr, XIN_MAX_PATH_LENGTH - (pointers_offset_get(pathptr, path)));

        return buf;
    }

    int __xin_entry_descriptor_get(const XinEntry* Entry)
    {
        if (__xin_entry_validation_check(Entry) == false)
            return -1;
        return (int)((uint32_t)Entry - (uint32_t)XIN_FS_ENTRIES_TABLE_BEGIN) / sizeof(XinEntry);
    }

    void __xin_entry_modification_fields_update(XinEntry* Entry)
    {
        if (__xin_entry_validation_check(Entry) == false)
            return;

        CmosTime Time;
        time_get(&Time);

        Entry->modification_date = time_extern_date(&Time);
        Entry->modification_time = time_extern_time(&Time);
    }

    bool __xin_is_entry_rwable_check(const XinEntry* Entry) // read and write (nie sprawdza czy size = 0)
    {
        if (__xin_entry_validation_check(Entry) == false)
            return false;

        if (Entry->FileInfo == NULL)
            return false;

        if ((Entry->type != XIN_FILE) && (Entry->type != XIN_HARD_LINK))
            return false;

        if (Entry->first_sector == XIN_FIRST_SECTOR_NOT_DEFINED)
            return false;
        return true;
    }

    /* -------------------------------------------------------------------------------------- */

    XinEntry* __xin_find_entry(const char* entryname)
    {
        char entrypath[XIN_MAX_PATH_LENGTH + 1] = { 0 };

        // if path is empty 
        if (!strlen(entryname)) {
            return NULL;
        }

        if (entryname[0] == XIN_SYSTEM_FOLDER && entryname[1] == XIN_SYSTEM_FOLDER) {
            return __xin_find_entry(XIN_SYSTEM_FOLDER_STR);
        }

        __xin_absolute_path_get(entryname, entrypath, XIN_DIRECTORY); // treat all Entries as directories
        uint32_t entrypath_len = strlen(entrypath);

        // dbg_info(DEBUG_LABEL_XIN_FS, entrypath);

        XIN_FS_ITERATE_OVER_ENTRY_TABLE(i)
        {
            // check if given folder exists
            if (bstrcmp(entrypath, i->path)) {
                return i;
            }

            // check if given file exists
            entrypath[entrypath_len - 1] = '\0';

            if (bstrcmp(entrypath, i->path)) {
                return i;
            }

            entrypath[entrypath_len - 1] = '/';
        }

        return NULL;
    }

    XinEntry* __xin_find_free_entry(void)
    {
        XIN_FS_ITERATE_OVER_ENTRY_TABLE(i)
        {
            if (i->path[0] == '\0')
                return i;
        }

        return NULL;
    }

    uint8_t* __xin_find_free_pointer(void)
    {
        XIN_FS_ITERATE_OVER_PTRS_TABLE(i)
        {
            if (*i == XIN_UNALLOCATED)
                return (uint8_t*)i;
        }

        return NULL;
    }

    uint8_t* __xin_find_free_pointer_with_given_size(uint32_t size)
    {
        if (!size)
            return NULL;

        XIN_FS_ITERATE_OVER_PTRS_TABLE(i)
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

    char* __xin_path_pf_extern(char* abspath, char* buf) // pf = parent folder
    {

        char abspathbuf[XIN_MAX_PATH_LENGTH + 1] = { 0 };

        memcpy(abspathbuf, abspath, XIN_MAX_PATH_LENGTH);

        if (bstrcmp(abspathbuf, XIN_SYSTEM_FOLDER_STR))
        {
            memcpy(buf, "/", 2);
            return buf;
        }

        if (__xin_is_relative_path_used(abspathbuf) == true)
            return buf;

        bool is_directory = abspathbuf[strlen(abspathbuf) - 1] == '/';

        int i;
        for (i = strlen(abspathbuf) - 1 - is_directory; abspathbuf[i] != '/'; i--)
            ;

        for (int j = 0; j <= i; j++)
            buf[j] = abspathbuf[j];

        buf[i + 1] = '\0';

        return buf;
    }

    XinEntry* __xin_entry_pf_extern(char* name) // pf = parent folder
    {
        if (bstrcmp(name, XIN_SYSTEM_FOLDER_STR))
            return __xin_find_entry("/");

        char parent_folder[XIN_MAX_PATH_LENGTH + 1] = { 0 };
        char path[XIN_MAX_PATH_LENGTH + 1] = { 0 };

        bool is_directory = name[strlen(name) - 1] == '/';

        __xin_absolute_path_get(name, path, is_directory ? XIN_DIRECTORY : XIN_FILE);

        int i;
        for (i = strlen(path) - 1 - is_directory; path[i] != '/'; i--)
            ;

        for (int j = 0; j <= i; j++)
            parent_folder[j] = path[j];

        XinEntry* ParentEntry = __xin_find_entry(parent_folder);

        return ParentEntry != NULL ? ParentEntry : NULL;
    }

    XinEntry* __xin_entry_pf_get(char* name) // pf = parent folder
    {
        XinEntry* Entry = __xin_find_entry(name);

        if (__xin_entry_validation_check(Entry) == false)
            return NULL;

        if (bstrcmp(Entry->path, XIN_SYSTEM_FOLDER_STR))
            return Entry;

        char parent_folder[XIN_MAX_PATH_LENGTH + 1] = { 0 };

        int i;
        for (i = strlen(Entry->path) - 1 - (Entry->type == XIN_DIRECTORY ? 1 : 0); Entry->path[i] != '/'; i--)
            ;

        for (int j = 0; j <= i; j++)
            parent_folder[j] = Entry->path[j];

        XinEntry* ParentEntry = __xin_find_entry(parent_folder);

        return ParentEntry != NULL ? ParentEntry : NULL;
    }

    XinChildrenEntries* xin_children_entries_get(char* folder, bool get_hidden)
    {

        if (__xin_find_entry(folder) == NULL || strlen(folder) == 0) {
            return NULL;
        }

        XinChildrenEntries* Children = (XinChildrenEntries*)calloc(sizeof(XinChildrenEntries));
        Children->Children = (XinEntry**)calloc(sizeof(XinEntry*));

        XinEntry* i = (XinEntry*)XIN_FS_ENTRIES_TABLE_BEGIN;
        char entrypf[XIN_MAX_PATH_LENGTH + 1] = { 0 };

        uint32_t finded_entries = 0;

        while (i < (XinEntry*)(XIN_FS_ENTRIES_TABLE_BEGIN + SECTOR_SIZE * XIN_FS_ENTRIES_SIZE))
        {
            if (bstrcmp(__xin_entry_pf_get(i->path)->path, folder) && i->path[0])
            {
                if (!bstrcmp(i->path, folder))
                {
                    if (get_hidden || __xin_entry_name_extern(i->path, entrypf)[0] != '.')
                    {
                        Children->Children[finded_entries] = i;
                        finded_entries++;
                        Children->Children = (XinEntry**)realloc(Children->Children, sizeof(XinEntry*) * (finded_entries));
                    }
                }
            }
            i++;
        }
        Children->how_many = finded_entries;
        return Children;
    }

    XinChildrenEntries* xin_children_entries_type_get(char* folder, uint8_t type)
    {

        if (__xin_find_entry(folder) == NULL || strlen(folder) == 0)
            return (XinChildrenEntries*)NULL;

        XinChildrenEntries* Children = (XinChildrenEntries*)calloc(sizeof(XinChildrenEntries));
        Children->Children = (XinEntry**)calloc(sizeof(XinEntry*));

        XinEntry* i = (XinEntry*)XIN_FS_ENTRIES_TABLE_BEGIN;

        uint32_t finded_entries = 0;
        while (i < (XinEntry*)(XIN_FS_ENTRIES_TABLE_BEGIN + SECTOR_SIZE * XIN_FS_ENTRIES_SIZE))
        {

            if (bstrcmp(__xin_entry_pf_get(i->path)->path, folder) && i->path[0])
            {
                if ((!bstrcmp(i->path, folder)) && (i->type == type))
                {
                    Children->Children[finded_entries] = i;
                    finded_entries++;
                    Children->Children = (XinEntry**)realloc(Children->Children, sizeof(XinEntry*) * (finded_entries));
                }
            }
            i++;
        }
        Children->how_many = finded_entries;
        return Children;
    }

    XinEntriesPack* __xin_hard_links_get(const XinEntry* const File)
    {

        XinEntriesPack* Pack = (XinEntriesPack*)calloc(sizeof(XinEntriesPack));
        Pack->Entries = (XinEntry**)calloc(sizeof(XinEntry*));

        if (File->type != XIN_FILE || File == NULL)
        {
            free(Pack->Entries);
            free(Pack);
            return NULL;
        }

        XIN_FS_ITERATE_OVER_ENTRY_TABLE(i)
        {
            if (i->first_sector == File->first_sector)
            {
                Pack->Entries = (XinEntry**)realloc(Pack->Entries, sizeof(XinEntry) * Pack->length + 1);
                Pack->Entries[Pack->length++] = i;
            }
        }

        return Pack;
    }

    char* __xin_current_directory_get(char* buf)
    {
        memcpy(buf, XinFsData.current_folder, XIN_MAX_PATH_LENGTH);
        return buf;
    }

    /* -------------------------------------------------------------------------------------- */

    bool __xin_entry_sectors_unlock(XinEntry* Entry)
    {
        if (__xin_entry_validation_check(Entry) == false)
            return false;

        if (Entry->type != XIN_FILE) // return false if entry is not a file
            return false;

        uint32_t sectors = int_to_sectors(Entry->size);
        memset(XIN_FS_PTRS_TABLE_BEGIN + Entry->first_sector, XIN_UNALLOCATED, sectors);

        Entry->first_sector = XIN_FIRST_SECTOR_NOT_DEFINED;
        Entry->size = 0;
        return true;
    }

    bool __xin_entry_data_remove(XinEntry* Entry)
    {
        if (__xin_entry_validation_check(Entry) == false)
            return false;

        __xin_entry_sectors_unlock(Entry);
        memset((uint8_t*)Entry, '\0', sizeof(XinEntry));
        return true;
    }

    XIN_FS_RETURN_STATUSES __xin_entry_create(XinEntryCreateArgs* Args, XIN_FS_ENTRY_TYPES type)
    {
        char entrypath[XIN_MAX_PATH_LENGTH + 1] = { '\0' };

        XinEntry* Entry = __xin_find_free_entry();

        __xin_absolute_path_get(Args->entryname, entrypath, type);

        if (__xin_find_entry(entrypath) != NULL)
            return XIN_ENTRY_EXISTS;

        if (type == XIN_HARD_LINK)
        {
            XinEntry* File = __xin_find_entry(Args->linksource);

            if (File == NULL)
                return XIN_ENTRY_NOT_FOUND;

            memcpy((uint8_t*)Entry, (uint8_t*)File, sizeof(XinEntry));

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

        return XIN_OK;
    }

    XIN_FS_RETURN_STATUSES __xin_file_create(char* filename)
    {
        XinEntryCreateArgs Args = { filename, NULL };
        return __xin_entry_create(&Args, XIN_FILE);
    }

    #warning "check if parent folder exists";
    XIN_FS_RETURN_STATUSES __xin_folder_create(char* foldername)
    {
        XinEntryCreateArgs Args = { foldername, NULL };
        return __xin_entry_create(&Args, XIN_DIRECTORY);
    }

    XIN_FS_RETURN_STATUSES __xin_link_create(char* filename, char* linkname)
    {
        XinEntryCreateArgs Args = { linkname, filename };
        return __xin_entry_create(&Args, XIN_HARD_LINK);
    }

    XIN_FS_RETURN_STATUSES __xin_folder_change(const char* foldername)
    {

        if (strlen(foldername) > XIN_MAX_PATH_LENGTH) {
            return XIN_ERROR;
        }

        else if (bstrcmp(foldername, ".")) {
            return XIN_OK;
        }

        else if (bstrcmp(foldername, "..")) {
            XinEntry* CurrentFolderParent = __xin_entry_pf_get(XinFsData.current_folder);
            __xin_folder_change(CurrentFolderParent != NULL ? CurrentFolderParent->path : "/");
            return XIN_OK;
        }

        while (bmemcmp(foldername, "../", 3))
        {
            if (bstrcmp(XinFsData.current_folder, XIN_SYSTEM_FOLDER_STR))
                return XIN_ERROR;
            else
            {
                XinEntry* CurrentFolderParent = __xin_entry_pf_get(XinFsData.current_folder);
                if (CurrentFolderParent != NULL)
                    strncpy(XinFsData.current_folder, CurrentFolderParent->path, XIN_MAX_PATH_LENGTH);
                else
                    __xin_folder_change(XIN_SYSTEM_FOLDER_STR);
            }
            foldername = foldername + 3;
        }

        #warning "TODO I think xin_find_entry can handle this";
        while (bmemcmp(foldername, "./", 2)) {
            foldername += 2;
        }

        XinEntry* NewFolder = __xin_find_entry(foldername);
        dbg_info(DEBUG_LABEL_XIN_FS, NewFolder->path);

        if (NewFolder != NULL && NewFolder->type == XIN_DIRECTORY) {
            strcpy(XinFsData.current_folder, NewFolder->path);
        }

        else {
            return XIN_ERROR;
        }

        return XIN_OK;
    }

    XIN_FS_RETURN_STATUSES __xin_file_remove(char* entryname)
    {
        XinEntry* Entry = __xin_find_entry(entryname);

        if (__xin_entry_validation_check(Entry) == false)
            return XIN_ENTRY_NOT_FOUND;

        if (Entry->type == XIN_FILE)
            // {
            //     memset(XIN_FS_PTRS_TABLE_BEGIN + Entry->first_sector, XIN_UNALLOCATED, int_to_sectors(Entry->size));
            __xin_entry_data_remove(Entry);
        // }

        else
            return XIN_ERROR; // xin bad entry type

        return XIN_OK;
    }

    XIN_FS_RETURN_STATUSES __xin_folder_remove(char* foldername)
    {

        XinEntry* Folder = __xin_find_entry(foldername);

        if (__xin_entry_validation_check(Folder) == false)
            return XIN_ENTRY_NOT_FOUND;

        XIN_FS_ITERATE_OVER_ENTRY_TABLE(i)
        {
            if (bstrcmp(__xin_entry_pf_get(i->path)->path, Folder->path))
                __xin_entry_data_remove(i);
        }

        __xin_entry_data_remove(Folder);
        return XIN_OK;
    }

    XIN_FS_RETURN_STATUSES __xin_link_remove(const char* linkname)
    {
        XinEntry* File = __xin_find_entry(linkname);

        if (File != NULL && File->type == XIN_LINK)
        {
            __xin_entry_data_remove(File);
            return XIN_OK;
        }

        return XIN_ERROR;
    }

    XIN_FS_RETURN_STATUSES __xin_entry_resize(XinEntry* Entry, uint32_t size)
    {

        if (__xin_entry_validation_check(Entry) == false)
            return XIN_ENTRY_VALIDATION_FAILED;

        __xin_entry_sectors_unlock(Entry);
        uint32_t sectors = int_to_sectors(size);

        xin_ptr_t* xin_pointer_table_entry = __xin_find_free_pointer_with_given_size(sectors);

        for (int i = 0; i < sectors - 1; i++)
            xin_pointer_table_entry[i] = XIN_ALLOCATED;

        xin_pointer_table_entry[sectors - 1] = XIN_EOF;
        return XIN_OK;
    }

    XIN_FS_RETURN_STATUSES __xin_entry_move(char* entryname, char* destname)
    {

        XinEntry* Entry = __xin_find_entry(entryname);
        char destpath[XIN_MAX_PATH_LENGTH + 1] = { 0 };
        memset(destpath, 0, XIN_MAX_PATH_LENGTH + 1);
        __xin_absolute_path_get(destname, destpath, xin_entry_type(Entry->type));

        xprintf("src: %s\n", Entry->path);
        xprintf("abs: %s\n", destpath);
        xprintf("pf: 0x%x\n", __xin_entry_pf_get(destpath));

        if ((__xin_entry_pf_get(destpath) == NULL) || (Entry == NULL))
            return XIN_ENTRY_NOT_FOUND;

        memcpy(Entry->path, destpath, XIN_MAX_PATH_LENGTH);

        return XIN_OK;
    }

    XIN_FS_RETURN_STATUSES __xin_copy(char* file_name, char* new_file_name)
    {

        XinEntry* Entry = __xin_find_entry(file_name);

        if (Entry == NULL)
            return XIN_ENTRY_NOT_FOUND;

        XIN_FS_RETURN_STATUSES status = __xin_file_create(new_file_name);

        if (status != XIN_OK)
            return status;

        XinEntry* File = fopen(file_name, "r");
        XinEntry* file_created = fopen(new_file_name, "rw");

        char* entry_data = (char*)calloc(File->size);

        fread(File, entry_data, File->size);
        fwrite(file_created, entry_data, File->size);

        free(entry_data);
        fclose(&File);
        fclose(&file_created);

        return XIN_OK;
    }

    /* -------------------------------------------------------------------------------------- */

    bool __xin_file_to_xfo_add(XinEntry* File)
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

    XinEntry* __xin_fopen(char* file_path, char* mode)
    {

        XinEntry* File = __xin_find_entry(file_path);

        if (File != NULL)
        {
            if (File->type != XIN_FILE && File->type != XIN_HARD_LINK)
                return NULL;

            File->FileInfo = (FileInformationBlock*)calloc(sizeof(FileInformationBlock));
            File->FileInfo->buffer = (uint8_t*)calloc(File->size + SECTOR_SIZE);           // additional space
            File->FileInfo->sector_in_use = (bool*)calloc(int_to_sectors(File->size) + 1); // additional space

            strcpy(File->FileInfo->rights, mode);

            File->FileInfo->position = 0;
            File->FileInfo->tmp_size = 0;
            __xin_file_to_xfo_add(File);
        }

        if (bstrncmp(mode, "a", 2))
        {
            File->FileInfo = (FileInformationBlock*)calloc(sizeof(FileInformationBlock));
            File->FileInfo->buffer = (uint8_t*)calloc(File->size + SECTOR_SIZE);
            File->FileInfo->sector_in_use = (bool*)calloc(int_to_sectors(File->size) + 5);

            strcpy(File->FileInfo->rights, mode);

            File->FileInfo->position = File->size;
            File->FileInfo->tmp_size = File->size;
            __xin_file_to_xfo_add(File);

            return File;
        }

        else if (bstrncmp(mode, "r", 2))
        {
            __xin_file_to_xfo_add(File);
            return File;
        }

        else if (bstrncmp(mode, "rw", 2) || bstrncmp(mode, "w", 2))
        {
            int status = __xin_file_create(file_path);

            if (status == XANIN_OK)
            {
                File = __xin_find_entry(file_path);
                File->FileInfo = (FileInformationBlock*)calloc(sizeof(FileInformationBlock));
                File->FileInfo->buffer = (uint8_t*)calloc(File->size + SECTOR_SIZE);
                File->FileInfo->sector_in_use = (bool*)calloc(int_to_sectors(File->size) + 5);

                strcpy(File->FileInfo->rights, mode);

                File->FileInfo->position = 0;
                File->FileInfo->tmp_size = 0;
                __xin_file_to_xfo_add(File);
                return File;
            }

            else if (status == XIN_ENTRY_EXISTS)
            {
                __xin_file_to_xfo_add(File);
                return File;
            }

            else
                return NULL;
        }

        return NULL;
    }

    size_t __xin_fread(XinEntry* Entry, void* buf, size_t count)
    {

        //////////////////VALIDATION///////////////////

        if (__xin_entry_validation_check(Entry) == false)
            return 0;

        if (!count)
            return 0;

        // if (Entry->FileInfo == NULL)
        //     return 0;

        // if ((Entry->type != XIN_FILE) && (Entry->type != XIN_HARD_LINK))
        //     return 0;

        // if (!Entry->size)
        //     return 0;

        // if (Entry->first_sector == XIN_FIRST_SECTOR_NOT_DEFINED)
        //     return 0;

        if (__xin_is_entry_rwable_check(Entry) == false)
            return 0;

        uint32_t initial_position = __xin_ftell(Entry);

        if (initial_position + count > Entry->size)
            count = Entry->size - initial_position;

        //////////////////////////////////////////////////

        if (initial_position + count > Entry->FileInfo->tmp_size)
            Entry->FileInfo->tmp_size = initial_position + count;

        if (Entry->FileInfo->tmp_size > Entry->size)
        {
            Entry->FileInfo->buffer = (uint8_t*)realloc(Entry->FileInfo->buffer, (int_to_sectors(Entry->FileInfo->tmp_size) + 1) * SECTOR_SIZE);
            Entry->FileInfo->sector_in_use = (bool*)realloc(Entry->FileInfo->sector_in_use, (int_to_sectors(Entry->FileInfo->tmp_size) + 1));
        }

        uint32_t sectors_to_load = int_to_sectors(count + initial_position);

        if (sectors_to_load > int_to_sectors(Entry->size))
            sectors_to_load = int_to_sectors(Entry->size);

        for (int i = 0; i < sectors_to_load; i++)
        {
            if (!Entry->FileInfo->sector_in_use[i])
            {
                disk_read(Entry->first_sector + i, 1, (uint16_t*)(Entry->FileInfo->buffer + (i * SECTOR_SIZE)));
                Entry->FileInfo->sector_in_use[i] = true;
            }
        }

        if (buf == NULL)
            return 0;

        for (char* i = (char*)(Entry->FileInfo->buffer + initial_position); i < (char*)(Entry->FileInfo->buffer + initial_position + count); i++, buf++)
            *(char*)buf = *i;

        __xin_fseek(Entry, initial_position + count);

        return count;
    }

    size_t __xin_fwrite(XinEntry* Entry, void* buf, size_t count)
    {
        //////////////////VALIDATION///////////////////

        if (!count)
            return 0;

        if (__xin_entry_validation_check(Entry) == false)
            return 0;

        if (__xin_is_entry_rwable_check(Entry) == false)
            return 0;

        //////////////////////////////////////////////////

        uint32_t initial_position = __xin_ftell(Entry);

        if ((initial_position + count) > Entry->FileInfo->tmp_size)
            Entry->FileInfo->tmp_size = initial_position + count;

        Entry->FileInfo->buffer = (uint8_t*)realloc(Entry->FileInfo->buffer, (int_to_sectors(Entry->FileInfo->tmp_size) + 1) * SECTOR_SIZE);
        Entry->FileInfo->sector_in_use = (bool*)realloc(Entry->FileInfo->sector_in_use, (int_to_sectors(Entry->FileInfo->tmp_size) + 1));

        uint32_t sectors_to_load = int_to_sectors(count + initial_position);

        if (sectors_to_load > int_to_sectors(Entry->size))
            sectors_to_load = int_to_sectors(Entry->size);

        for (int i = 0; i < sectors_to_load; i++)
        {
            if (!Entry->FileInfo->sector_in_use[i])
            {
                disk_read(Entry->first_sector + i, 1, (uint16_t*)(Entry->FileInfo->buffer + (i * SECTOR_SIZE)));
                Entry->FileInfo->sector_in_use[i] = true;
            }
        }

        __xin_fseek(Entry, initial_position);

        for (char* i = (char*)(Entry->FileInfo->buffer) + initial_position; i < (char*)(Entry->FileInfo->buffer + initial_position + count); i++, buf++)
        {
            *i = *(char*)buf;
            __xin_fseek(Entry, __xin_ftell(Entry) + 1);
        }

        __xin_entry_modification_fields_update(Entry);
        return count;
    }

    int __xin_open(char* filename, uint32_t options)
    {

        XinEntry* Entry = fopen(filename, "rw");
        return __xin_entry_descriptor_get(Entry);

        // XinEntry *File = __xin_find_entry(file_path);

        // if (File != NULL && File->type != XIN_DIRECTORY && File->path[0] != '\0')
        // {

        //     if (File->FileInfo == NULL)
        //         File->FileInfo = (FileInformationBlock *)calloc(sizeof(FileInformationBlock));

        //     File->FileInfo->position = 0;
        //     FileDescriptorTable[fd].is_used = true;
        //     FileDescriptorTable[fd].Entry = File;

        //     return fd;
        // }
    }

    size_t __xin_read(int fd, void* buf, size_t count)
    {

        //////////////////VALIDATION///////////////////

        if (fd < 0)
            return 0;

        if (FileDescriptorTable[fd].is_used == false)
            return 0;

        XinEntry* Entry = (XinEntry*)(XIN_FS_ENTRIES_TABLE_BEGIN + (fd * XIN_ENTRY_SIZE));

        //////////////////////////////////////////////////

        return fread(Entry, buf, count);
    }

    size_t __xin_write(int fd, void* buf, size_t count)
    {

        ////////////////////VALIDATION/////////////////////

        if (fd < 0)
            return 0;

        if (FileDescriptorTable[fd].is_used == false)
            return 0;

        XinEntry* Entry = (XinEntry*)(XIN_FS_ENTRIES_TABLE_BEGIN + (fd * XIN_ENTRY_SIZE));

        //////////////////////////////////////////////////

        return fwrite(Entry, buf, count);
    }

    #warning "TODO move __xin_fseek and __xin_ftell to stdlibx";
    void __xin_fseek(XinEntry* File, uint32_t new_position)
    {
        if (__xin_entry_validation_check(File) == true) {
            File->FileInfo->position = new_position;
        }
    }

    void __xin_lseek(int fd, uint32_t new_position)
    {
        if (FileDescriptorTable[fd].is_used == false || __xin_entry_validation_check(FileDescriptorTable[fd].Entry) == false) { return; }
        FileDescriptorTable[fd].Entry->FileInfo->position = new_position;
    }

    const uint32_t __xin_ftell(XinEntry* File)
    {
        return File->FileInfo->position;
    }

    const uint32_t __xin_ltell(int fd)
    {
        if (FileDescriptorTable[fd].is_used == false || __xin_entry_validation_check(FileDescriptorTable[fd].Entry) == false) { return -1; }
        return __xin_ftell(FileDescriptorTable[fd].Entry);
    }

    void __xin_free_temporary_data(XinEntry* File)
    {
        if (__xin_entry_validation_check(File) == false)
            return;

        free(File->FileInfo->buffer);
        free(File->FileInfo);
        File->FileInfo = NULL;
    }

    bool __xin_fclose_with_given_size(XinEntry** File, uint32_t size)
    {

        if (__xin_entry_validation_check(*File) == false)
            false;

        if (!bstrncmp((*File)->FileInfo->rights, "r", 2)) // READ-ONLY OPTION
        {
            uint8_t* buf = (uint8_t*)calloc(int_to_sectors(size) * SECTOR_SIZE);
            memcpy(buf, VAL(File)->FileInfo->buffer, size);
            memset(&buf[size], 0, (int_to_sectors(size) * SECTOR_SIZE) - size);

            uint32_t number_of_sectors_to_deallocate = int_to_sectors(VAL(File)->size);
            if (!number_of_sectors_to_deallocate)
                number_of_sectors_to_deallocate++;

            uint32_t number_of_sectors_to_allocate = int_to_sectors(size);
            if (!number_of_sectors_to_allocate)
                number_of_sectors_to_allocate++;

            uint8_t* entry_deallocate = (uint8_t*)(VAL(File)->first_sector + XIN_FS_ENTRIES_TABLE_BEGIN);
            for (int i = 0; i < number_of_sectors_to_deallocate; i++)
                entry_deallocate[i] = XIN_UNALLOCATED;

            /* write Entry to xin Entry pointers table */
            uint8_t* entry_ptrs = __xin_find_free_pointer_with_given_size(number_of_sectors_to_allocate);

            for (int i = 0; i < number_of_sectors_to_allocate - 1; i++)
                entry_ptrs[i] = XIN_ALLOCATED;

            entry_ptrs[number_of_sectors_to_allocate - 1] = XIN_EOF;

            /* write Entry to xin Entry data table */

            __xin_entry_modification_fields_update(VAL(File));
            VAL(File)->size = size;

            // get all hard links
            XinEntriesPack* HardLinks = __xin_hard_links_get(VAL(File));

            for (int i = 0; i < HardLinks->length; i++)
            {
                __xin_entry_modification_fields_update(HardLinks->Entries[i]);
                HardLinks->Entries[i]->first_sector = entry_ptrs - XIN_FS_PTRS_TABLE_BEGIN;
                HardLinks->Entries[i]->size = size;
            }

            VAL(File)->first_sector = entry_ptrs - XIN_FS_PTRS_TABLE_BEGIN;

            disk_sectors_write(ATA_FIRST_BUS, ATA_MASTER, VAL(File)->first_sector, number_of_sectors_to_allocate, (uint16_t*)buf);
            __xin_tables_update();

            free(buf);
        }

        __xin_free_temporary_data(*File);

        for (int i = 0; i < XIN_OPENED_FILES_COUNTER; i++)
        {
            if (XinFilesOpened[i] == (*File))
                XinFilesOpened[i] = NULL;
        }

        *File = NULL;
        return true;
    }

    void __xin_fclose(XinEntry** File)
    {
        if (__xin_entry_validation_check(*File) == false)
            return;

        uint32_t newsize;

        if ((*File)->FileInfo->tmp_size > (*File)->size)
            newsize = (*File)->FileInfo->tmp_size;
        else
            newsize = (*File)->size;

        if (!bstrncmp((*File)->FileInfo->rights, "r", 2)) // READ-ONLY OPTION
            __xin_fclose_with_given_size(File, newsize);

        __xin_free_temporary_data(*File);

        for (int i = 0; i < XIN_OPENED_FILES_COUNTER; i++)
        {
            if (XinFilesOpened[i] == (*File))
                XinFilesOpened[i] = NULL;
        }

        (*File) = NULL;
    }

    void __xin_close(int fd)
    {

        if (!FileDescriptorTable[fd].is_used)
            return;

        XinEntry* File = FileDescriptorTable[fd].Entry;

        fclose(&File);

        FileDescriptorTable[fd].is_used = false;
    }

    void __xin_all_files_close(void)
    {
        EFlags Flags;
        INTERRUPTS_OFF(&Flags);

        for (int i = 0; i < XIN_OPENED_FILES_COUNTER; i++)
        {
            if (XinFilesOpened[i] != NULL)
                fclose(&XinFilesOpened[i]);
        }

        INTERRUPTS_ON(&Flags);
    }

    /* -------------------------------------------------------------------------------------- */

    char* getline_from_ptr(char* data, int line_id)
    {

        char* file_data = (char*)(data);
        char* line = (char*)calloc(200);

        int column = 0;
        int current_line = 0;
        int file_offset = 0;

        while (current_line < line_id)
        {
            memset((uint8_t*)line, 0, 200);

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

} //extern "C"