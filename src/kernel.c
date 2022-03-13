
#include <limits.h>
#include <stdint.h>
#include <pit/pit.c>
#include <IDT/idt.c>
#include <lib/stdlibx.h>
#include <terminal/vty.c>
#include <terminal/interpreter.c>
#include <lib/math.h>
//#include <fs/ustar.c>
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
    asm("cli");    //disable interrupts while IDT is not configured
    screen_init(); //init screen management system


    xprintf("%z  .GBJ     ?BBY.                     ,, \n", set_output_color(black, magenta));
    xprintf("%z   :B#Y  !G#P^   .,,.     ..  ..     ''   ..  ..   \n", set_output_color(black, magenta));
    xprintf("%z    .G#\/#G~     ?PYJJ5P   HuCJJ5G\    55  HuCJJJPP\ \n", set_output_color(black, magenta));
    xprintf("%z     >###<     ~.   .G#.  I#D   7#:  GP  I#D    P#:\n", set_output_color(black, magenta));
    xprintf("%z   .5#BG#P.    !JYYJ?G#.  I#^   ^#:  GP  I#^    Y#:\n", set_output_color(black, magenta));
    xprintf("%z .Y##/ \i#G.   ?#^   :G#.  I#^   ~#:  GP  I#^    Y#:\n", set_output_color(black, magenta));
    xprintf("%z7GB5.    5BG. .5G555J?B^  I#^   ^#:  P5  I#^    JB:\n\n\n", set_output_color(black, magenta));



    keyboard_command = comBuf;

    set_idt();

    xprintf("DETECTING USB CONTROLLERS. PLEASE WAIT...\n");

    usb_detect();

    xprintf("\n%zUSB DETECTION TEST ENDED. PRESS ENTER TO START XANIN OS\n",
            set_output_color(green,white));


    keyboard_init();
    set_pit();

    while(getscan() != ENTER);

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
    

    getTime();

   
    //xprintf("xaninOS\n");
    
    xprintf("%z  .GBJ     ?BBY.                     ,,\n", set_output_color(logo_back_color, logo_front_color));
    xprintf("%z   :B#Y  !G#P^   .,,.     ..  ..     ''   ..  ..\n", set_output_color(logo_back_color, logo_front_color));
    xprintf("%z    .G#\/#G~     ?PYJJ5P   HuCJJ5G    55  HuCJJJPP.\n", set_output_color(logo_back_color, logo_front_color));
    xprintf("%z     >###<     ~.   .G#.  I#D   7#:  GP  I#D    P#:\n", set_output_color(logo_back_color, logo_front_color));
    xprintf("%z   .5#BG#P.    !JYYJ?G#.  I#^   ^#:  GP  I#^    Y#:\n", set_output_color(logo_back_color, logo_front_color));
    xprintf("%z .Y##/ i#G.   ?#^   :G#.  I#^   ~#:  GP  I#^    Y#:%z  version 22.03v\n", set_output_color(logo_back_color, logo_front_color), set_output_color(black,white));
    xprintf("%z7GB5.    5BG. .5G555J?B^  I#^   ^#:  P5  I#^    JB:%z  weekday: %s \n\n\n", set_output_color(logo_back_color, logo_front_color), set_output_color(black,white), weekDaysLUT[time.weekDay]);

//    xprintf("");

//    xprintf("weekday: %s\n\n",weekDaysLUT[time.weekDay]);


    Screen.cursor[8][0] = (uint16_t)('>' | ((black << 4) | white) << 8);
    Screen.x = 1;
    Screen.y = 8;
    character_blocked = '>';

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
        
        scan();

    }

}
