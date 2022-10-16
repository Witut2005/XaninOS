
#pragma once

 
#include <stdint.h>
#include <libc/hal.h>
#include <devices/HARD_DISK/disk.h>



int reboot(void)
{

uint16_t idt_16[3] = {0x0, 0x0, 0x0};

    // disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x12, 8, (uint16_t*)0x800);

    // for(int i = 0; i < 8; i++)
    //     disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x1a + i, 1, (uint16_t*)(0x1800 + (i * SECTOR_SIZE)));

    // disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x82, 1, (uint16_t*)kernel_load_backup);

    // xin_entry* files = (xin_entry*)0x1800;
    // uint8_t* file_allocation_table = (uint8_t*)0x800;

    // files += 12;

    // while(files->entry_path[0] != '\0')
    // {
    //     xin_entry* current_file = fopen(files->entry_path, "rw");
    //     uint8_t* buf = (uint8_t*)calloc(SECTOR_SIZE * 16);

    //     read(current_file, buf, SECTOR_SIZE * 16);
        
    //     if(files->entry_type == XIN_FILE)
    //     {
    //         int i;
    //         for(i = 0; file_allocation_table[i + files->starting_sector] != XIN_EOF; i++) 
    //             disk_write(ATA_FIRST_BUS, ATA_MASTER, i + files->starting_sector, 1, (uint16_t*)(buf + (i * SECTOR_SIZE)));
    //         disk_write(ATA_FIRST_BUS, ATA_MASTER, i + files->starting_sector, 1, (uint16_t*)(buf + (i * SECTOR_SIZE)));
    //     }
    //     free(buf);

    //     files++;
    // }

    asm("lidt %0" :: "m"(idt_16));
    
    uint8_t s = 12;
    s = s / 0;

    return XANIN_OK;

}
