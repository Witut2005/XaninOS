

#pragma once



#include <stdint.h>




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

char* fileTable[2];
char* fileData[2];

