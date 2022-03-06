
#pragma once

#include <stdint.h>

enum xin_fs_properties
{
    XIN_ENTRY_POINTERS = 0x800,
    XIN_ENTRY_TABLE = 0x1800,
    XIN_ALLOCATED = 0x1,
    XIN_UNALLOCATED = 0x0,
    XIN_EOF = 0xFF,
    MAX_PATH = 40,
    PERMISSION_MAX = 0xFF,
    SECTOR_SIZE = 512
};

struct xin_entry
{

    char entry_path[MAX_PATH];
    uint8_t entry_type;
    uint32_t creation_date;
    uint16_t creation_time;
    uint16_t os_specific;
    uint32_t modification_date;
    uint16_t modification_time;
    uint8_t entry_permissions;
    uint32_t entry_size;    
    uint32_t starting_sector;

}__attribute__((packed));


typedef struct xin_entry xin_entry;

xin_entry* xin_find_free_entry(void);
xin_entry* xin_create_file(char* entry_name);
xin_entry* xin_init_fs(void);
bool xin_remove_entry(char* entry_name);


