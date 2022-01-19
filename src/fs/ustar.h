

#pragma once


#include <lib/string.h>
#include <stdint.h>


char* fileTable[2];
char* fileData[2];


#define FS_START_OFFSET 0x7c05
#define FILE_ENTRIES_NUMBER 0x7c0d

#define FILE_ENTRIES sizeof(fileTable) / sizeof(fileTable[0])
#define FILE_DATA_ENTRIES sizeof(fileData) / sizeof(fileData[0])

#define BYTES_PER_FILE_SIZE_FIELD 12

#define USTAR_SECTOR_SIZE 512

#define nullptr 0x0


char* set_current_directory(char*);
char* get_current_directory(void);
char* get_current_path(char*);


struct 
{
    uint8_t file_entries_number;
    char* file_system_start;
}static FileSystem;


struct FileSystemEntryStruct
{
    char entry_type;
    
    uint32_t entry_size;
    uint32_t owner;
    uint32_t group;

    char* entry_name;
    char* entry_size_pointer;
    char* entry_data_pointer;

    struct FileSystemEntryStruct* sub_entries;
};

typedef struct FileSystemEntryStruct FileSystemEntryStruct;

FileSystemEntryStruct fs_entry[10];

char current_directory_buffer[30]; 
char current_path_buffer[50];


char* current_directory;
char* current_file_path;


enum ustar_entry_types
{
    FILE = '0',
    HARD_LINK = '1',
    SOFT_LINK = '2',
    DEVICE = '3',
    BLOCK_DEVICE = '4',
    DIRECTORY = '5',
    FIFO = '6'
};