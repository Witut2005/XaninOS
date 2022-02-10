#include <devices/HARD_DISK/disk.h>

void init_disk(void)
{

    outbIO(ATA_HEAD_REGISTER, 0xA0);
	  outbIO(ATA_SECTOR_COUNT_REGISTER, 0x0);
	  outbIO(ATA_SECTOR_NUMBER_LOW, 0x0);
	  outbIO(ATA_SECTOR_NUMBER_MID, 0x0);
	  outbIO(ATA_SECTOR_NUMBER_HIGH, 0x0);

	  /* IDENTIFY */
	  outbIO(ATA_COMMAND_REGISTER, 0xEC);
	  //xprintf("%x\n",inbIO(0x1F7));
    

}
