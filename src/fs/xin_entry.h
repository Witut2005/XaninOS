
#pragma once

#include <stdint.h>
#include <stdbool.h>

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

struct FileInformationBlock 
{
    char file_name[XIN_MAX_PATH_LENGTH];
    char rights[2];
    uint32_t position;
    uint8_t* buffer;
    uint32_t tmp_size;
    bool* sector_in_use;
}__attribute__((packed));

#ifndef __cplusplus
typedef struct FileInformationBlock FileInformationBlock;
#endif

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
}__attribute__((packed));

#ifndef __cplusplus
typedef struct XinEntry XinEntry;
typedef struct XinEntry XinBuffer;
#endif

