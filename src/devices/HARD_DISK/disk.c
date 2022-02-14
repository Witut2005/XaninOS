#include <devices/HARD_DISK/disk.h>

void init_disk(uint16_t base)
{

    uint8_t disk_status;


    outbIO(base + ATA_DRIVE_REGISTER, base == ATA_MASTER_BASE ? 0xA0 : 0xB0);
    outbIO(ATA_COMMAND_REGISTER, 0x0);



    outbIO(base + ATA_DRIVE_REGISTER, base == ATA_MASTER_BASE ? 0xA0 : 0xB0);

    disk_status = inbIO(ATA_COMMAND_REGISTER);

    if(disk_status == 0xFF)
    {
        xprintf("NO DISK\n");
        return;
    }

	  outbIO(base + ATA_SECTOR_COUNT_REGISTER, 0x1);
	  outbIO(base + ATA_SECTOR_NUMBER_LOW, 0x0);
	  outbIO(base + ATA_SECTOR_NUMBER_MID, 0x0);
	  outbIO(base + ATA_SECTOR_NUMBER_HIGH, 0x0);

    disk_status = inbIO(base + ATA_COMMAND_REGISTER);


	  
	  outbIO(base + ATA_COMMAND_REGISTER, 0xEC);
	 

        
    while(((disk_status & 0x80) == 0x80)
       && ((disk_status & 0x01) != 0x01))
        disk_status = inbIO(base + ATA_COMMAND_REGISTER);
        


}



void disk_read(uint16_t base, uint32_t sector_number, uint32_t bytes_number)
{

    uint8_t disk_status;


    outbIO(base + ATA_DRIVE_REGISTER, (base == ATA_MASTER_BASE ? 0xE0 : 0xF0) | 
                                        ((sector_number >> 24) & 0x0F) | 0x40);

    outbIO(base + ATA_ERROR_REGISTER, 0x0);
	  outbIO(base + ATA_SECTOR_COUNT_REGISTER, 0x1);
	  
    outbIO(base + ATA_SECTOR_NUMBER_LOW, sector_number & 0xFF);
	  outbIO(base + ATA_SECTOR_NUMBER_MID, (sector_number >> 8) & 0xFF);
	  outbIO(base + ATA_SECTOR_NUMBER_HIGH, (sector_number >> 16) & 0xFF);

	 
	  outbIO(base + ATA_COMMAND_REGISTER, ATA_READ);


    while((disk_status & 0x81) == 0x80)
        disk_status = inbIO(base + ATA_COMMAND_REGISTER);


    for(uint32_t i = 0x0; i < 0x1FFFFFF; i++)
        asm("nop");

    bytes_number = bytes_number / 2;

	uint16_t* print_data = VGA_TEXT_MEMORY;

    for(int i = 0; i < bytes_number; i++)
    {
        uint16_t readed_data = indwIO(base + ATA_DATA_REGISTER);
        
        //xprintf("%x %x", readed_data & 0xFF, (readed_data >> 8) & 0xFF);
        xprintf("%x %x ",(readed_data >> 8) & 0xFF, readed_data & 0xFF);
    }
    


    xprintf("\n\n\n");

}

