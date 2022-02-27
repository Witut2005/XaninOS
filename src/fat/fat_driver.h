
#pragma once

#include <lib/stdiox.h>


#define FAT_START 0x800
#define ROOT_DIRECTORY_START 0x1800 


struct root_directory_entry
{

    char file_name[8];
    char file_extension[3];
    char file_attr;
    char entry_case;
    char creation_time_miliseconds;
    uint16_t creation_time;
    uint16_t creation_date;
    uint16_t last_access_date;
    uint16_t reserved;
    uint16_t last_modification_time;
    uint16_t last_modification_date;
    uint16_t starting_cluster;
    uint32_t file_size;
}__attribute__((packed));

typedef struct root_directory_entry root_directory_entry;
uint8_t fat_entries = 0x0;
root_directory_entry* root_directory_table=(root_directory_entry*)ROOT_DIRECTORY_START; 


typedef uint8_t fat_cell;
fat_cell* file_allocation_table = (fat_cell*)FAT_START;


void fat_read_cluster(uint16_t cluster_number);
uint8_t* fat_find_unallocated_cluster(void);

enum fat_macros
{
    FILENAME_MAX_LENGTH = 0x8, 
    FAT_END_FILE = 0xF8,
    FAT_UNALLOCATED = 0x0,
    FAT_ALLOCATED = 0x1,
    CLUSTER_SIZE = 0x200
    
};
