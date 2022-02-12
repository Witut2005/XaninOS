#include <devices/HARD_DISK/disk.h>

void init_disk(void)
{

    uint8_t disk_status;


    outbIO(ATA_DRIVE_REGISTER, 0xA0);
	  outbIO(ATA_SECTOR_COUNT_REGISTER, 0x0);
	  outbIO(ATA_SECTOR_NUMBER_LOW, 0x0);
	  outbIO(ATA_SECTOR_NUMBER_MID, 0x0);
	  outbIO(ATA_SECTOR_NUMBER_HIGH, 0x0);

    disk_status = inbIO(ATA_COMMAND_REGISTER);

    if(disk_status == 0xFF)
    {
        xprintf("NO DISK\n");
        return;
    }

	  /* IDENTIFY */
	  outbIO(ATA_COMMAND_REGISTER, 0xEC);
	  //xprintf("%x\n",inbIO(0x1F7));

        
    while((disk_status & 0x81) == 0x80)
        disk_status = inbIO(ATA_COMMAND_REGISTER);


    for(int i = 0; i < 0x100; i++)
    {

    
        uint16_t readed_data = indwIO(ATA_DATA_REGISTER);
        xprintf("%d ",readed_data);


    }

}


void ata_read_sectors(uint8_t sectors_to_read)
{


}
