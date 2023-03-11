
#pragma once

#include <stdint.h>
#include <stdbool.h>

enum xin_fs_properties
{
    
    XIN_ENTRY_POINTERS = 0x800,
    XIN_ENTRY_TABLE = 0x1800,

    XIN_ALLOCATED = 0x1,
    XIN_UNALLOCATED = 0x0,
    XIN_EOF = 0xFF,
    
    MAX_PATH = 38,
    PERMISSION_MAX = 0xFF,
    SECTOR_SIZE = 512,
    XIN_FILE = 'F',
    XIN_DIRECTORY = 'D',
    XIN_LINK = 'L',
    XIN_HARD_LINK = 'H',

};

struct FileInformationBlock 
{
    char file_name[MAX_PATH];
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
    char path[MAX_PATH];          //38
    uint8_t type;                       //39
    uint32_t creation_date;             //43
    uint16_t creation_time;             //45
    uint32_t modification_date;         //49
    uint16_t modification_time;         //51
    uint8_t permissions;          //52
    uint32_t size;                      //56
    uint32_t first_sector;           //60
    FileInformationBlock* FileInfo; //64
}__attribute__((packed));

#ifndef __cplusplus
typedef struct XinEntry XinEntry;
#endif
