
#pragma once

#include <stdint.h>

/* all ports all 8bit with exeception of DATA_REGISTER (16bit) */

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

#ifdef __cplusplus
extern "C" {
#endif

    void disk_init(uint16_t base, uint8_t master);
    void disk_sectors_read(uint16_t base, uint8_t master, uint32_t sector_number, uint16_t how_many_sectors, uint16_t* where);
    void disk_read_bytes(uint16_t base, uint16_t master, uint32_t sector_number, uint16_t offset, uint32_t amount, uint8_t* buf);
    void disk_sectors_write(uint16_t base, uint8_t master, uint32_t sector_number, uint16_t how_many_sectors, uint16_t* where);
    void disk_flush(uint16_t base, uint8_t master);

    void disk_read(uint32_t first_sector, uint32_t how_many, uint16_t* buf);
    void disk_write(uint32_t first_sector, uint32_t how_many, uint16_t* buf);

#ifdef __cplusplus
}
#endif