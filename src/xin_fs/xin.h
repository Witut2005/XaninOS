
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
    SECTOR_SIZE = 512,
    XIN_FILE = 'F',
    XIN_DIRECTORY = 'D',
    XIN_LINK = 'L',
    XIN_HARD_LINK = 'H',

    XIN_READ_ONLY = 0x10

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
    uint32_t file_position;

}__attribute__((packed));


typedef struct xin_entry xin_entry;

char xin_current_path[40] = {'\0'};
char xin_current_directory[40] = {'\0'};


char* xin_set_current_directory(char* directory);
char* xin_get_current_directory(void);
char* xin_get_current_path(char* file_name);

bool xin_remove_entry(char* entry_name);
char* xin_get_current_path(char* file_name);

xin_entry* xin_init_fs(void);
xin_entry* xin_find_free_entry(void);

void xin_create_file(char* entry_name);
void xin_create_directory(char* entry_name);
xin_entry* xin_change_directory(char* new_directory);
void xin_entry_info(char* entry_name);

void xin_entry_resize(uint16_t last_sector);
xin_entry* fopen(char* file_path, const char* mode);

