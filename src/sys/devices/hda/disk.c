
#include <sys/devices/hda/disk.h>
#include <sys/syslog/syslog.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/hal.h>
#include <fs/xin.h>

void init_disk(uint16_t base, uint8_t master)
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
        inwIO(base + ATA_DATA_REGISTER);    

}

void disk_read_single_sector(uint16_t base, uint16_t master, uint32_t sector_number, uint16_t* where)
{

    uint8_t disk_status;

    outbIO(base + ATA_DRIVE_REGISTER, ((master == ATA_MASTER ? 0x40 : 0x50)));
    outbIO(base + ATA_SECTOR_COUNT_REGISTER, 0 >> 8); //sector count high byte
    
    /* lba4 - lba6 */
    outbIO(base + ATA_SECTOR_NUMBER_LOW, 0);
	outbIO(base + ATA_SECTOR_NUMBER_MID, 0);
	outbIO(base + ATA_SECTOR_NUMBER_HIGH, 0);

    outbIO(base + ATA_SECTOR_COUNT_REGISTER, 1); // set sector count to 1
	  
    /* lba1 - lba3 */
    outbIO(base + ATA_SECTOR_NUMBER_LOW, sector_number & 0xFF);
	outbIO(base + ATA_SECTOR_NUMBER_MID, (sector_number >> 8) & 0xFF);
	outbIO(base + ATA_SECTOR_NUMBER_HIGH, (sector_number >> 16) & 0xFF);
	
	outbIO(base + ATA_COMMAND_REGISTER, ATA_EXTENDED_READ);

    disk_status = inbIO(base + ATA_STATUS_REGISTER);

    while((disk_status & 0x81) == 0x80)
        disk_status = inbIO(base + ATA_STATUS_REGISTER);


    for(int i = 0; i < 4; i++)
        inbIO(base + ATA_STATUS_REGISTER);

    for(int i = 0; i < 4; i++)
        io_wait();

  
    for(int j = 0; j < 256; j++)
    {
        uint16_t readed_data = inwIO(base + ATA_DATA_REGISTER);
    
        *where = readed_data;
        where++;
    
    }
}

void disk_read(uint16_t base, uint8_t master, uint32_t sector_number, 
                                uint16_t how_many_sectors, uint16_t* where)
{

    for(int i = 0; i < how_many_sectors; i++)
        disk_read_single_sector(base, master, sector_number + i, (uint16_t*)((uint32_t)where + (i * SECTOR_SIZE)));

}



void disk_flush(uint16_t base, uint8_t master)
{
    uint8_t disk_status;

    outbIO(base + ATA_DRIVE_REGISTER, (master == ATA_MASTER ? 0xE0 : 0xF0));
	outbIO(base + ATA_COMMAND_REGISTER, ATA_FLUSH);

    disk_status = inbIO(base + ATA_STATUS_REGISTER);
    if(!disk_status)
        return;

    while((disk_status & 0x81) == 0x80)
        disk_status = inbIO(base + ATA_STATUS_REGISTER);


}

void disk_write_single_sector(uint16_t base, uint8_t master, uint32_t sector_number, uint16_t* where)
{
    uint8_t disk_status;


    outbIO(base + ATA_DRIVE_REGISTER, ((master == ATA_MASTER ? 0x40 : 0x50)));
    outbIO(base + ATA_SECTOR_COUNT_REGISTER, 0 >> 8); // sector count high byte
    
    /* lba4 - lba6 */
    outbIO(base + ATA_SECTOR_NUMBER_LOW, 0);
	outbIO(base + ATA_SECTOR_NUMBER_MID, 0);
	outbIO(base + ATA_SECTOR_NUMBER_HIGH, 0);

    outbIO(base + ATA_SECTOR_COUNT_REGISTER, 1); // set sector count to 1
	  
    /* lba1 - lba3 */
    outbIO(base + ATA_SECTOR_NUMBER_LOW, sector_number & 0xFF);
	outbIO(base + ATA_SECTOR_NUMBER_MID, (sector_number >> 8) & 0xFF);
	outbIO(base + ATA_SECTOR_NUMBER_HIGH, (sector_number >> 16) & 0xFF);
	
	outbIO(base + ATA_COMMAND_REGISTER, ATA_EXTENDED_WRITE);


    for(int j = 0; j < 256; j++)
        outwIO(base + ATA_DATA_REGISTER, where[j]);
    disk_flush(ATA_FIRST_BUS, ATA_MASTER);
        
    if(disk_status & 0x1 == 1)
    	printk("Disk module writing error");
}


void disk_write(uint16_t base, uint8_t master, uint32_t sector_number, uint16_t how_many_sectors, uint16_t* where)
{


    for(int i = 0; i < how_many_sectors; i++)
        disk_write_single_sector(base, master, sector_number + i, (uint16_t*)((uint32_t)where + (i * SECTOR_SIZE)));


}
