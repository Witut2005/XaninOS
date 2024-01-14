
#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <lib/libc/stdlibx.h>
#include <sys/macros.h>
#include <sys/flow/exit_codes.h>

#define XIN_SYSTEM_FOLDER_STR "/"

enum XIN_FS_PROPERTIES
{
    XIN_SYSTEM_FOLDER = '/',
    XIN_FILE_BEGIN = 0,
    XIN_ENTRY_SIZE = 64,
    XIN_ALLOCATED = 0x1,
    XIN_UNALLOCATED = 0x0,
    XIN_EOF = 0xFF,

    XIN_MAX_PATH_LENGTH = 38,
    PERMISSION_MAX = 0xFF,
    SECTOR_SIZE = 512,
    XIN_FIRST_SECTOR_NOT_DEFINED = UINT32_MAX
};

typedef enum XIN_FS_ENTRY_TYPES
{
    XIN_FILE = 'F',
    XIN_DIRECTORY = 'D',
    XIN_LINK = 'L',
    XIN_HARD_LINK = 'H'
} XIN_FS_ENTRY_TYPES;

typedef uint8_t xin_entry_type_t;

struct XinFileSystemData
{
    uint32_t first_sector;
    uint32_t ptrs_size;    // in sectors
    uint32_t entries_size; // in sectors
    uint8_t *tables;
    char current_folder[XIN_MAX_PATH_LENGTH];
} __attribute__((packed));

typedef struct XinFileSystemData XinFileSystemData;

struct FileInformationBlock
{
    char file_name[XIN_MAX_PATH_LENGTH];
    char rights[2];
    uint32_t position;
    uint8_t *buffer;
    uint32_t tmp_size;
    bool *sector_in_use;
} __attribute__((packed));

typedef struct FileInformationBlock FileInformationBlock;

struct XinEntry
{
    char path[XIN_MAX_PATH_LENGTH];
    uint8_t type;
    uint32_t creation_date;
    uint16_t creation_time;
    uint32_t modification_date;
    uint16_t modification_time;
    uint8_t permissions;
    uint32_t size;
    uint32_t first_sector;
    FileInformationBlock *FileInfo;
} __attribute__((packed));

typedef struct XinEntry XinEntry;
typedef struct XinEntry XinBuffer;

typedef enum
{
    XIN_OK = 0,
    XIN_ERROR = 3,

    XIN_ENTRY_EXISTS = 0x4,
    XIN_BAD_FOLDER_NAME = 0x5,
    XIN_ENTRY_NOT_FOUND = 0x6,
    XIN_TO_LONG_PATH = 0x7,
    XIN_NOT_A_FOLDER,
    XIN_NOT_A_FILE,
    XIN_NOT_A_HARD_LINK,
    XIN_ENTRY_BAD_TYPE

} XIN_FS_RETURN_STATUSES;

struct XinFileDescriptor
{
    uint8_t is_used;
    XinEntry *Entry;
};

struct XinChildrenEntries
{
    XinEntry **Children;
    uint32_t how_many;
};

struct XinEntriesPack
{
    XinEntry **Entries;
    uint32_t length;
};

struct XinEntryCreateArgs
{
    char *entryname;
    char *linksource;
};

typedef struct XinEntryCreateArgs XinEntryCreateArgs;
typedef struct XinFileDescriptor XinFileDescriptor;
typedef struct XinChildrenEntries XinChildrenEntries;
typedef struct XinEntriesPack XinEntriesPack;

extern XinFileDescriptor *FileDescriptorTable;

typedef uint8_t xin_ptr_t;

#ifdef __cplusplus
extern "C"
{
    namespace xin
    {
#endif

        bool __xin_entry_alignment_check(XinEntry *Entry);
        bool __xin_entry_address_check(XinEntry *Entry);
        bool __xin_entry_validation_check(XinEntry *Entry);
        void __xin_entry_modification_fields_update(XinEntry *Entry);
        char *__xin_absolute_path_get(char *rpath, char *buf, XIN_FS_ENTRY_TYPES type);
        bool __xin_is_relative_path_used(char *path);
        char *__xin_entry_name_extern(char *path);

        void __xin_tables_update(void);
        void __xin_fs_tables_set(uint8_t *tables);
        void __xin_fs_load_tables_from_disk(void); // oj calkiem risky
        uint8_t *__xin_fs_ptrs_get(void);
        uint32_t __xin_fs_entries_size_get(void);
        uint32_t __xin_fs_ptrs_size_get(void);
        uint8_t *__xin_fs_entries_get(void);
        XinEntry *__xin_fs_entries_end_get(void);
        uint8_t *__xin_fs_tables_get(void);
        XinFileSystemData __xin_fs_data_get(void);
        void __xin_detect_file_system(void);

        char *__xin_current_directory_get(char *buf);

        XIN_FS_RETURN_STATUSES __xin_file_remove(char *entry_name);

        void __xin_init();
        XinEntry *____xin_find_free_entry(void);

        XIN_FS_RETURN_STATUSES __xin_folder_change(char *new_directory);

        XIN_FS_RETURN_STATUSES __xin_entry_create(XinEntryCreateArgs *Args, XIN_FS_ENTRY_TYPES type);
        XIN_FS_RETURN_STATUSES __xin_file_create(char *entry_name);
        XIN_FS_RETURN_STATUSES __xin_link_create(char *file_name, char *link_name);
        XIN_FS_RETURN_STATUSES __xin_folder_create(char *entry_name);
        XIN_FS_RETURN_STATUSES __xin_entry_move(char *entry_name, char *new_name);

        XIN_FS_RETURN_STATUSES __xin_folder_remove(char *folder_name);
        XIN_FS_RETURN_STATUSES __xin_link_remove(const char *linkname);

        XIN_FS_RETURN_STATUSES __xin_copy(char *file_name, char *new_file_name);

        XinEntry *__xin_fopen(char *file_path, char *mode);
        size_t __xin_fwrite(XinEntry *entry, void *buf, size_t count);
        size_t __xin_fread(XinEntry *entry, void *buf, size_t count);
        void __xin_fclose(XinEntry **file);

        int __xin_open(char *file_path, uint32_t options);
        size_t __xin_write(int fd, void *buf, size_t count);
        size_t __xin_read(int fd, void *buf, size_t count);
        void __xin_close(int fd);

        void fseek(XinEntry *file, uint32_t new_position);
        void lseek(int fd, uint32_t new_position);
        XinEntry *__xin_entry_pf_get(char *path); // pf = parent folder
        XinEntry *__xin_find_entry(char *entry_name);

        void fclose_with_given_size(XinEntry **file, uint32_t new_size);
        char *getline(XinEntry *file, int line_id);
        char *getline_from_ptr(char *data, int line_id);
        XinChildrenEntries *xin_children_entries_get(char *folder, bool show_hidden);
        XinChildrenEntries *xin_children_entries_type_get(char *folder, uint8_t type);
        char *__xin_entry_name_get(char *path);
        const uint32_t ftell(XinEntry *file);
        const uint32_t lteel(int fd);
        uint8_t *__xin_find_free_pointer(void);
        uint8_t *____xin_find_free_pointer_with_given_size(uint32_t size);
        void __xin_entry_resize(XinEntry *entry, uint32_t new_size);
        void ____xin_free_temporary_data(XinEntry *File);
        XinEntriesPack *__xin_hard_links_get(const XinEntry *const File);
        bool __xin_file_to_xfo_add(XinEntry *File);
        void __xin_all_files_close(void);

#ifdef __cplusplus
    }
}
#endif
