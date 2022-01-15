

#pragma once



#include <stdint.h>


char* fileTable[2];
char* fileData[2];


#define FS_START_OFFSET 0x7c05
#define FILE_ENTRIES_NUMBER 0x7c0d

#define FILE_ENTRIES sizeof(fileTable) / sizeof(fileTable[0])
#define FILE_DATA_ENTRIES sizeof(fileData) / sizeof(fileData[0])

#define BYTES_PER_FILE_SIZE_FIELD 12

struct 
{
    uint8_t file_entries_number;
    char* file_system_start;
}fileSystem;



typedef struct 
{

    uint64_t fileMode;
    uint64_t ownerID;
    uint64_t groupID;

    uint32_t fileSize[3];
    
    uint32_t lastModification[3];
    uint64_t checksum;

    uint8_t typeflag;

    /* --------------------------------------------------/
    |file name is in fileTable array                     |
    |Ustar string field doesnt matter                    |
    |UStar version field doesnt matter                   |           
    |                                                    |
    |fileName prefix is data stored in file.             |
    |This data is pointer by fileData array of pointer   |
    /---------------------------------------------------*/


}__attribute__((packed)) fileMetadata;


fileMetadata* fileMeta[2];


