#include <lib/stdiox.h>

#define SECTOR_SIZE 0x200

/* all ports all 8bit with exeception of DATA_REGISTER (16bit) */

enum AtaRegisters
{
    
    ATA_IO_BASE = 0x1F0,
    ATA_DATA_REGISTER = 0x1F0,

    ATA_ERROR_REGISTER = 0x1F1,
    ATA_FEATURES_REGISTER = 0x1F1,

    ATA_SECTOR_COUNT_REGISTER = 0x1F2,

    ATA_SECTOR_NUMBER_LOW = 0x1F3,
    ATA_SECTOR_NUMBER_MID = 0x1F4,
    ATA_SECTOR_NUMBER_HIGH = 0x1F5,

    ATA_DRIVE_REGISTER = 0x1F6,
    ATA_STATUS_REGISTER = 0x1F7,
    ATA_COMMAND_REGISTER = 0x1F7
    
};

void init_disk(void);
 
	
