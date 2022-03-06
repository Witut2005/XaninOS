
#pragma once

#include <lib/stdiox.h>


#define FAT_START 0x800
#define ROOT_DIRECTORY_START 0x1800 

union fat_file
{
    
    char fat_file_id[12];

    struct
    {
        char file_name[8];
        char file_extension[4];
   
    };
}file;

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

struct file_entry
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

typedef struct file_entry file_entry;

struct fat_folder
{

    char entry_name[11];
    char entry_attr;
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

typedef struct fat_folder fat_folder;
fat_folder* used_folder;


struct fat_metadata
{
    uint8_t* current_folder; 
    uint16_t number_of_files;
};

struct fat_metadata fat;

//fat.current_folder =  ROOT_DIRECTORY;

typedef struct root_directory_entry root_directory_entry;
typedef struct fat_folder_entry fat_folder_entry;

uint8_t fat_entries = 0x0;
root_directory_entry* root_directory_table=(root_directory_entry*)ROOT_DIRECTORY_START; 


typedef uint8_t fat_cell;
fat_cell* file_allocation_table = (fat_cell*)FAT_START;


void fat_read_cluster(uint16_t cluster_number);
uint8_t fat_find_unallocated_cluster(void);
root_directory_entry* fat_find_free_root_entry(void);
uint8_t fat_get_name_length(char* entry_name);

void fat_entry_name_padding(char* entry_name, uint8_t name_length);
void fat_save_entry_name(char* entry_name, root_directory_entry* entry);
fat_folder* fat_find_free_folder_entry(fat_folder* current_folder);

enum fat_macros
{
    FAT_FILE = 0x20,
    FAT_FOLDER = 0x40,
    FILENAME_MAX_LENGTH = 0x8, 
    FAT_END_FILE = 0xF8,
    FAT_UNALLOCATED = 0x0,
    FAT_ALLOCATED = 0x1,
    CLUSTER_SIZE = 0x200,
    ROOT_DIRECTORY = 0x0
};

char fat_current_folder[8];