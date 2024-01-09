
#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <lib/libc/stdlibx.h>
#include <sys/macros.h>
#include <sys/flow/exit_codes.h>

#define XIN_SYSTEM_FOLDER '/'
#define XIN_FILE_BEGIN 0
#define XIN_ENTRY_SIZE 64

enum xin_fs_properties
{

    XIN_ENTRY_POINTERS = 0x800,
    XIN_ENTRY_TABLE = 0x1800,
    XIN_ENTRY_POINTERS_SECTORS = 8,
    XIN_ENTRY_TABLE_SECTORS = 50,

    XIN_ALLOCATED = 0x1,
    XIN_UNALLOCATED = 0x0,
    XIN_EOF = 0xFF,

    XIN_MAX_PATH_LENGTH = 38,
    PERMISSION_MAX = 0xFF,
    SECTOR_SIZE = 512,
    XIN_FILE = 'F',
    XIN_DIRECTORY = 'D',
    XIN_LINK = 'L',
    XIN_HARD_LINK = 'H',

};

struct XinFileSystemData
{
    uint32_t xin_fs_first_sector;
    uint32_t xin_fs_ptrs_size;    // in sectors
    uint32_t xin_fs_entries_size; // in sectors
};

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

enum XIN_RETURN_STATUS
{

    XIN_FILE_EXISTS = 0x4,
    XIN_BAD_FOLDER_NAME = 0x5,
    XIN_ENTRY_NOT_FOUND = 0x6,
    XIN_TO_LONG_PATH = 0x7,
    XIN_NOT_A_FOLDER = 0x8,

};

struct XinFileDescriptor
{
    uint8_t is_used;
    XinEntry *entry;
};

struct XinChildrenEntries
{
    XinEntry **children;
    uint32_t how_many;
};

struct XinEntriesPack
{
    XinEntry **entries;
    uint32_t length;
};

typedef struct XinFileDescriptor XinFileDescriptor;
typedef struct XinChildrenEntries XinChildrenEntries;
typedef struct XinEntriesPack XinEntriesPack;

extern XinFileDescriptor *FileDescriptorTable;

#ifdef __cplusplus
extern "C"
{
    namespace xin
    {
#endif

        bool __xin_current_directory_set(char *directory);
        char *__xin_current_directory_get(char *buf);
        char *__xin_path_get(char *file_name);

        __STATUS __xin_entry_remove(char *entry_name);
        char *__xin_get_current_path(char *file_name);

        void __xin_load_tables(void);
        void __xin_init_fs(void);
        XinEntry *____xin_find_free_entry(void);

        __STATUS __xin_file_create(char *entry_name);
        __STATUS __xin_folder_create(char *entry_name);
        __STATUS __xin_folder_change(char *new_directory);
        __STATUS __xin_entry_move(char *entry_name, char *new_name);

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
        XinEntry *__xin_get_file_pf(char *path); // pf = parent folder
        XinEntry *__xin_find_entry(char *entry_name);
        __STATUS __xin_folder_remove(char *folder_name);

        void fclose_with_given_size(XinEntry **file, uint32_t new_size);
        char *getline(XinEntry *file, int line_id);
        char *getline_from_ptr(char *data, int line_id);
        XinChildrenEntries *xin_get_children_entries(char *folder, bool show_hidden);
        XinChildrenEntries *xin_get_children_entries_type(char *folder, uint8_t type);
        char *__xin_entry_name_get(char *path);
        const uint32_t ftell(XinEntry *file);
        const uint32_t lteel(int fd);
        uint8_t *__xin_find_free_pointer(void);
        uint8_t *____xin_find_free_pointer_with_given_size(uint32_t size);
        void __xin_entry_resize(XinEntry *entry, uint32_t new_size);
        int __xin_file_size_in_sectors_get(XinEntry *File);
        void ____xin_free_temporary_data(XinEntry *File);
        XinEntriesPack *__xin_hard_links_get(const XinEntry *const File);
        bool __xin_file_to_xfo_add(XinEntry *File);
        void __xin_all_files_close(void);

        __STATUS __xin_link_remove(const char *linkname);
        __STATUS __xin_link_create(char *file_name, char *link_name);
        __STATUS __xin_copy(char *file_name, char *new_file_name);

#ifdef __cplusplus
    }
}
#endif
