
#pragma once

#include <stdint.h>

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



typedef struct xin_entry xin_entry;



struct file_information_block
{
    char file_name[MAX_PATH];
    char rights[2];
    uint32_t position;


}__attribute__((packed, aligned(64)));

typedef struct file_information_block file_information_block;

struct xin_entry
{
    char entry_path[MAX_PATH];          //38
    uint8_t entry_type;                 //43
    uint32_t creation_date;             //47
    uint16_t creation_time;             //49
    uint32_t modification_date;         //53
    uint16_t modification_time;         //55
    uint8_t entry_permissions;          //56
    uint32_t entry_size;                //60
    uint32_t starting_sector;           //64
    file_information_block* file_info;  //42
}__attribute__((packed));
