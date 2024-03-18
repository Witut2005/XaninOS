
#pragma once
#include <stdint.h>

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
    XIN_HARD_LINK = 'H',
    XIN_STDIN = 'I',
    XIN_STDOUT = 'O',
    XIN_STDERR = 'E'
} XIN_FS_ENTRY_TYPES;

typedef uint8_t xin_entry_type_t;

struct XinFileSystemData
{
    uint32_t first_sector;
    uint32_t ptrs_size;    // in sectors
    uint32_t entries_size; // in sectors
    uint8_t* tables;
    char current_folder[XIN_MAX_PATH_LENGTH];
} __attribute__((packed));

typedef struct XinFileSystemData XinFileSystemData;

struct FileInformationBlock
{
    char rights[2];
    uint32_t position;
    uint8_t* buffer;
    uint32_t tmp_size;
    bool* sector_in_use;
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
    FileInformationBlock* FileInfo;
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
    XIN_ENTRY_BAD_TYPE,
    XIN_ENTRY_VALIDATION_FAILED

} XIN_FS_RETURN_STATUSES;

struct XinFileDescriptor
{
    uint8_t is_used;
    XinEntry* Entry;
};

struct XinChildrenEntries
{
    XinEntry** Children;
    uint32_t how_many;
};

struct XinEntriesPack
{
    XinEntry** Entries;
    uint32_t length;
};

struct XinEntryCreateArgs
{
    char* entryname;
    char* linksource;
};

typedef struct XinEntryCreateArgs XinEntryCreateArgs;
typedef struct XinFileDescriptor XinFileDescriptor;
typedef struct XinChildrenEntries XinChildrenEntries;
typedef struct XinEntriesPack XinEntriesPack;

extern XinFileDescriptor* FileDescriptorTable;
typedef uint8_t xin_ptr_t;