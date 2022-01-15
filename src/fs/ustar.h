

#pragma once



#include <stdint.h>


char* fileTable[2];
char* fileData[2];


#define FS_START_OFFSET 0x7c05
#define FILE_ENTRIES_NUMBER 0x7c0d

#define FILE_ENTRIES sizeof(fileTable) / sizeof(fileTable[0])
#define FILE_DATA_ENTRIES sizeof(fileData) / sizeof(fileData[0])

#define BYTES_PER_FILE_SIZE_FIELD 12

#define USTAR_SECTOR_SIZE 512

struct 
{
    uint8_t file_entries_number;
    char* file_system_start;
}FileSystem;


struct FileSystemEntryStruct
{
    char* entry_name;
    char* entry_size_pointer;
    char* entry_data_pointer;

    uint32_t entry_size;
    uint32_t owner;
    uint32_t group;

    struct FileSystemEntryStruct* sub_entries;


}__attribute__((packed));

typedef struct FileSystemEntryStruct FileSystemEntryStruct;

static FileSystemEntryStruct fs_entry[2];

