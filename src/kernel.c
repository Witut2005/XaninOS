
#include <limits.h>
#include <stdint.h>
#include <pit/pit.c>
#include <IDT/idt.c>
#include <lib/stdlibx.h>
#include <terminal/vty.c>
#include <terminal/interpreter.c>
#include <lib/math.h>
#include <fs/ustar.c>
#include <keyboard/keyboardInit.c>
#include <devices/PCI/pci.c>
#include <devices/ACPI/ACPI.c>
#include <devices/USB/usb.c>
#include <devices/HARD_DISK/disk.c>
//#include <fat/fat_driver.c>
#include <xin_fs/xin.c>

//extern void init_disk(uint16_t base, uint8_t master);
//extern void disk_read(uint16_t base, uint8_t master, uint32_t sector_number, uint32_t bytes_number, uint16_t* where);


//#include <devices/DMA/dma.c>

char* tmpStr;


/*--------------------------------------/
|wesolego nowego roku :))               |
|niech xanin rośnie i rośnie            |
|ja, rok 2021, 31 grudzień, 23:52:35    |
/--------------------------------------*/



void _start(void)
{


    disable_cursor();

    clearScr();
    asm("cli");

    keyboard_command = comBuf;

    set_idt();

    xprintf("DETECTING USB CONTROLLERS. PLEASE WAIT...\n");

    usb_detect();

    xprintf("\n\n%zUSB DETECTION TEST ENDED. PRESS ENTER TO START XANIN OS\n",
            set_output_color(green,white));

   
    keyboard_init();
    set_pit();

 
    getCpuSpeed();
    getTime();
    srand(time.seconds);
	
    xin_init_fs();
    
   
    init_disk(ATA_FIRST_BUS, ATA_MASTER);

    keyboard_scan_code = 0x0;
    while(!keyboard_scan_code);

    for(char* i = (char*)0x0; (uint32_t)i < 0x1000; i++)
    	*i = 0x0;


    tuiInit:

    clearScr();

   
    xprintf("xaninOS\n");
    xprintf("version 22.03v\n");

    getTime();
    xprintf("weekday: %s\n\n",weekDaysLUT[time.weekDay]);


    *cursor = (uint16_t)('>' | ((black << 4) | white) << 8);
    cursor++;
    x++;

    app_exited = false;
    arrows_navigate = true;

    while(1)
    {

        if(app_exited)
        {
            app_exited = false;
            for(int i = 0; i < sizeof(comBuf); i++)
                keyboard_command[i] = '\0';
            goto tuiInit;
        }

        index = 0x0;
        for(int i = 0; i < sizeof(comBuf); i++)
            keyboard_command[i] = '\0';

        scan();

    }

}
