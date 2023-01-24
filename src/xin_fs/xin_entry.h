
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

struct MemoryBlocksUsed
{
    uint16_t begin;
    uint16_t end;
};

#ifndef __cplusplus
typedef struct MemoryBlocksUsed MemoryBlocksUsed;
#endif

struct file_information_block
{
    char file_name[MAX_PATH];
    char rights[2];
    uint32_t position;
    uint8_t* base_address_memory;
    bool* sector_in_use;

}__attribute__((packed, aligned(64)));

#ifndef __cplusplus
typedef struct file_information_block file_information_block;
#endif

struct xin_entry
{
    char entry_path[MAX_PATH];          //38
    uint8_t entry_type;                 //39
    uint32_t creation_date;             //43
    uint16_t creation_time;             //45
    uint32_t modification_date;         //49
    uint16_t modification_time;         //51
    uint8_t entry_permissions;          //52
    uint32_t entry_size;                //56
    uint32_t starting_sector;           //60
    file_information_block* file_info;  //64
}__attribute__((packed));

#ifndef __cplusplus
typedef struct xin_entry xin_entry;
#endif
