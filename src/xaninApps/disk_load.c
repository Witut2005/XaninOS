#pragma once

#include <devices/HARD_DISK/disk.h>

void disk_load(char* sector_str, char* how_many_sectors_str, char * address_str)
{
    uint32_t sector = strtoi(sector_str, 16); 
    uint8_t how_many_sectors = strtoi(how_many_sectors_str, 16);
    uint32_t address = strtoi(address_str, 16);
    disk_read(ATA_FIRST_BUS, ATA_MASTER, sector, how_many_sectors, address);
}
