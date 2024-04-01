
#pragma once

#include <stdint.h>

#define SECTOR_SIZE 0x200

enum ATA_REGISTERS
{

    ATA_MASTER = 0x1,
    ATA_SLAVE = 0x0,

    ATA_FIRST_BUS = 0x1F0,
    ATA_SECONDARY_BUS = 0x170,


    ATA_DATA_REGISTER = 0x0,

    ATA_ERROR_REGISTER = 0x1,
    ATA_FEATURES_REGISTER = 0x1,

    ATA_SECTOR_COUNT_REGISTER = 0x2,

    ATA_SECTOR_NUMBER_LOW = 0x3,
    ATA_SECTOR_NUMBER_MID = 0x4,
    ATA_SECTOR_NUMBER_HIGH = 0x5,

    ATA_DRIVE_REGISTER = 0x6,
    ATA_STATUS_REGISTER = 0x7,
    ATA_COMMAND_REGISTER = 0x7,

    ATA_CONTROL_REGISTER = 0x206

};

enum ATA_COMMANDS
{

    ATA_IDENTIFY = 0xEC,
    ATA_READ = 0x20,
    ATA_EXTENDED_READ = 0x24,
    ATA_WRITE = 0x30,
    ATA_EXTENDED_WRITE = 0x34,
    ATA_FLUSH = 0xE7

};

void disk_init(uint16_t base, uint8_t master);
void disk_sectors_read(uint16_t base, uint8_t master, uint32_t sector_number,
    uint16_t how_many_sectors, uint16_t* where);