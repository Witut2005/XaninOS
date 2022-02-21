#include <devices/HARD_DISK/disk.h>

__attribute__((section("disk_drvr"))) void init_disk(uint16_t base, uint8_t master)
{

    uint8_t disk_status;

    
    outbIO(base + ATA_DRIVE_REGISTER, master == ATA_MASTER ? 0xA0 : 0xB0);
    outbIO(base + ATA_CONTROL_REGISTER, 0x0); 



    outbIO(base + ATA_DRIVE_REGISTER, master == ATA_MASTER ? 0xA0 : 0xB0);
    disk_status = inbIO(ATA_COMMAND_REGISTER);

    if(disk_status == 0xFF)
    {
        xprintf("NO DISK\n");
        return;
    }

    outbIO(base + ATA_DRIVE_REGISTER, master == ATA_MASTER ? 0xA0 : 0xB0);
	  outbIO(base + ATA_SECTOR_COUNT_REGISTER, 0x0);
	  outbIO(base + ATA_SECTOR_NUMBER_LOW, 0x0);
	  outbIO(base + ATA_SECTOR_NUMBER_MID, 0x0);
	  outbIO(base + ATA_SECTOR_NUMBER_HIGH, 0x0);
	  
	  outbIO(base + ATA_COMMAND_REGISTER, ATA_IDENTIFY);
	 
    disk_status = inbIO(base + ATA_COMMAND_REGISTER);
	  
    while(disk_status & 0x81 == 80)
		    disk_status = inbIO(base + ATA_COMMAND_REGISTER);  
    
    for(int i = 0; i < 256; i++)
        indwIO(base + ATA_DATA_REGISTER);    

}



void disk_read(uint16_t base, uint8_t master, uint32_t sector_number, 
                                uint32_t bytes_number, uint16_t* where)
{


    uint8_t disk_status;


    outbIO(base + ATA_DRIVE_REGISTER, ((master == ATA_MASTER ? 0xE0 : 0xF0) | 
                                        ((sector_number >> 24) & 0x0F) | 0x40));

    outbIO(base + ATA_ERROR_REGISTER, 0x0);
	  outbIO(base + ATA_SECTOR_COUNT_REGISTER, 0x1);
	  
    outbIO(base + ATA_SECTOR_NUMBER_LOW, sector_number & 0xFF);
	  outbIO(base + ATA_SECTOR_NUMBER_MID, (sector_number >> 8) & 0xFF);
	  outbIO(base + ATA_SECTOR_NUMBER_HIGH, (sector_number >> 16) & 0xFF);

	 
	  outbIO(base + ATA_COMMAND_REGISTER, ATA_READ);


    while((disk_status & 0x81) == 0x80)
        disk_status = inbIO(base + ATA_STATUS_REGISTER);


    /*

    for(uint32_t i = 0x0; i < 0x1FFFFFF; i++)
        asm("nop");

    */

    for(int i = 0; i < 4; i++)
        inbIO(base + ATA_STATUS_REGISTER);

    for(int i = 0; i < 0xFFFFFFF; i++)
        asm("nop");

    bytes_number = bytes_number / 2;	

    for(int i = 0; i < 256; i++)
    {
        uint16_t readed_data = indwIO(base + ATA_DATA_REGISTER);
        
      	*where = readed_data;
      	where++;
        
    }
    
    //disk_status = inbIO(base + ATA_STATUS_REGISTER);
    
    if(disk_status & 0x1 == 1)
    	xprintf("%zDISK ERROR", set_output_color(red,white));


    xprintf("\n\n\n");

}

