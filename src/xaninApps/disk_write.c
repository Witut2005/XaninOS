#pragma once

#include <devices/HARD_DISK/disk.h>

int disk_write_data(char* sector_str, char* how_many_sectors_str, char* address_str)
{
    uint32_t sector = strtoi(sector_str, HEXADECIMAL);
    uint8_t how_many_sectors = strtoi(how_many_sectors_str, HEXADECIMAL);
    uint32_t address = strtoi(address_str, HEXADECIMAL);

    disk_write(ATA_FIRST_BUS, ATA_MASTER, sector, how_many_sectors, (uint16_t*)address);
    return XANIN_OK;
}
