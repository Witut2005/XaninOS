
#include "./disk.h"
#include "./hal.h"
#include "./bootio.h"

void disk_init(uint16_t base, uint8_t master)
{

    uint8_t disk_status;


    outbIO(base + ATA_DRIVE_REGISTER, master == ATA_MASTER ? 0xA0 : 0xB0);
    outbIO(base + ATA_CONTROL_REGISTER, 0x0);

    outbIO(base + ATA_DRIVE_REGISTER, master == ATA_MASTER ? 0xA0 : 0xB0);
    disk_status = inbIO(ATA_COMMAND_REGISTER);

    if (disk_status == 0xFF)
    {
        print("NO DISK\n", OUTPUT_COLOR_SET(red, white));
        return;
        while (1);
    }

    outbIO(base + ATA_DRIVE_REGISTER, master == ATA_MASTER ? 0xA0 : 0xB0);
    outbIO(base + ATA_SECTOR_COUNT_REGISTER, 0x0);
    outbIO(base + ATA_SECTOR_NUMBER_LOW, 0x0);
    outbIO(base + ATA_SECTOR_NUMBER_MID, 0x0);
    outbIO(base + ATA_SECTOR_NUMBER_HIGH, 0x0);

    outbIO(base + ATA_COMMAND_REGISTER, ATA_IDENTIFY);

    disk_status = inbIO(base + ATA_COMMAND_REGISTER);

    while ((disk_status & 0x81) == 0x80)
        disk_status = inbIO(base + ATA_COMMAND_REGISTER);

    for (int i = 0; i < 256; i++)
        inwIO(base + ATA_DATA_REGISTER);

}


void disk_sectors_read(uint16_t base, uint8_t master, uint32_t sector_number,
    uint16_t how_many_sectors, uint16_t* where)
{


    uint8_t disk_status;

    outbIO(base + ATA_DRIVE_REGISTER, ((master == ATA_MASTER ? 0x40 : 0x50)));
    outbIO(base + ATA_SECTOR_COUNT_REGISTER, how_many_sectors >> 8);

    /* lba4 - lba6 */
    outbIO(base + ATA_SECTOR_NUMBER_LOW, 0);
    outbIO(base + ATA_SECTOR_NUMBER_MID, 0);
    outbIO(base + ATA_SECTOR_NUMBER_HIGH, 0);

    outbIO(base + ATA_SECTOR_COUNT_REGISTER, how_many_sectors & 0xFF);

    /* lba1 - lba3 */
    outbIO(base + ATA_SECTOR_NUMBER_LOW, sector_number & 0xFF);
    outbIO(base + ATA_SECTOR_NUMBER_MID, (sector_number >> 8) & 0xFF);
    outbIO(base + ATA_SECTOR_NUMBER_HIGH, (sector_number >> 16) & 0xFF);

    outbIO(base + ATA_COMMAND_REGISTER, ATA_EXTENDED_READ);

    disk_status = inbIO(base + ATA_STATUS_REGISTER);

    while ((disk_status & 0x81) == 0x80)
        disk_status = inbIO(base + ATA_STATUS_REGISTER);


    for (int i = 0; i < 4; i++)
        inbIO(base + ATA_STATUS_REGISTER);

    for (int i = 0; i < 4; i++)
        io_wait();

    for (int i = 0; i < how_many_sectors; i++)
    {
        for (int j = 0; j < 256; j++)
        {
            uint16_t readed_data = inwIO(base + ATA_DATA_REGISTER);

            *where = readed_data;
            where++;

        }
    }

}
