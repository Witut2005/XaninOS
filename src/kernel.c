
#include <limits.h>
#include <stdint.h>
#include <pit/pit.c>
#include <IDT/idt.c>
#include <lib/stdlibx.h>
#include <terminal/vty.c>
#include <terminal/interpreter.c>
#include <lib/math.h>
#include <keyboard/keyboardInit.c>
#include <devices/PCI/pci.c>
#include <devices/ACPI/ACPI.c>
#include <devices/USB/usb.c>
#include <devices/HARD_DISK/disk.c>
#include <xin_fs/xin.c>
#include <lib/algorithm.h>



/*--------------------------------------/
|wesolego nowego roku :))               |
|niech xanin rośnie i rośnie            |
|ja, rok 2021, 31 grudzień, 23:52:35    |
/--------------------------------------*/

void _start(void)
{



    disable_cursor();

    screen_clear();
    asm("cli");    //disable interrupts while IDT is not configured
    screen_init(); //init screen management system


    //*(char*)VGA_TEXT_MEMORY = 0x41;
    //*(char*)(VGA_TEXT_MEMORY + 1) = 0x42;
    set_idt();
    time_get();
    keyboard_init();
    
    for(char* i = (char*)0x0; (uint32_t)i < 0x20000; i++)
    	*i = 0x0;



    /*

    char a[20],b[20];
    screen_clear();

    for(int i = 0; i < 20; i++)
        a[i] = '\0';
    
    for(int i = 0; i < 20; i++)
        b[i] = '\0';

    xscanf("%s %s",a, b);

    xprintf("\n%s\n", a);
    xprintf("%s\n", b);

    xprintf("\n");

    while(1);

    */

    set_pit();


    while(!pitActive)
    {
        cpu_mhz++;
    }

    cpu_mhz = cpu_mhz / 55;
    cpu_mhz = cpu_mhz * 1000; //Cycles per second
    cpu_khz = cpu_mhz / 1000;
    cpu_mhz = cpu_mhz / 1000000; //from Hz to MHz

    cpu_khz = cpu_khz * 7;
    cpu_mhz = cpu_mhz * 7;

    /*
    uint16_t dd = 10;
    xprintf("0x%x\n", dd);
    bit_clear(&dd, 1);
    xprintf("0x%x\n", dd);
    */

    
    /*

    xprintf("%z  .GBJ     ?BBY.                     ,,\n", set_output_color(logo_back_color, logo_front_color));
    xprintf("%z   :B#Y  !G#P^   .,,.     ..  ..     ''   ..  ..\n", set_output_color(logo_back_color, logo_front_color));
    xprintf("%z    .G#//#G~    ?PYJJ5P   HuCJJ5G    55  HuCJJJPP.\n", set_output_color(logo_back_color, logo_front_color));
    xprintf("%z     >###<     ~.   .G#.  I#D   7#:  GP  I#D    P#:\n", set_output_color(logo_back_color, logo_front_color));
    xprintf("%z   .5#BG#P.    !JYYJ?G#.  I#^   ^#:  GP  I#^    Y#:\n", set_output_color(logo_back_color, logo_front_color));
    xprintf("%z .Y##/ i#G.   ?#^   :G#.  I#^   ~#:  GP  I#^    Y#:%z  version 22.03v\n", set_output_color(logo_back_color, logo_front_color), set_output_color(black,white));
    xprintf("%z7GB5.    5BG. .5G555J?B^  I#^   ^#:  P5  I#^    JB:%z  weekday: %s \n\n\n", set_output_color(logo_back_color, logo_front_color), set_output_color(black,white), weekDaysLUT[time.weekDay]);

    */

    keyboard_command = command_buffer;

    xprintf("DETECTING USB CONTROLLERS. PLEASE WAIT...\n");

    usb_detect();

    xprintf("\n%zUSB DETECTION TEST ENDED. PRESS ENTER TO START XANIN OS\n",
            set_output_color(green,white));


    xprintf("\n\n");
   

    //xprintf("cpu mhz %dMHz\n", cpu_mhz);
    //xprintf("cpu khz %dKHz\n", cpu_khz);

    zsk("100");

    //while(getscan() != ENTER);
    //getCpuSpeed();

    srand(time.seconds);
	
    xin_init_fs();
    
   
    init_disk(ATA_FIRST_BUS, ATA_MASTER);

    keyboard_scan_code = 0x0;
    while(!keyboard_scan_code);


    tuiInit:

    screen_clear();
    
    //asm("int 0xFF");
    //asm("int 13");// <-- GENERAL PROTECTION EXCEPTION TEST

    time_get();

   
    //xprintf("xaninOS\n");
    
    xprintf("%z  .GBJ     ?BBY.                     ,,\n", set_output_color(logo_back_color, logo_front_color));
    xprintf("%z   :B#Y  !G#P^   .,,.     ..  ..     ''   ..  ..\n", set_output_color(logo_back_color, logo_front_color));
    xprintf("%z    .G#//#G~    ?PYJJ5P   HuCJJ5G    55  HuCJJJPP.\n", set_output_color(logo_back_color, logo_front_color));
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


    keyboard_handle = terminal_keyboard;

    while(1)
    {

        if(app_exited)
        {
            app_exited = false;
            for(int i = 0; i < sizeof(command_buffer); i++)
                keyboard_command[i] = '\0';
            goto tuiInit;
        }
        
        scan();

    }

}
