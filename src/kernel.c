
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
#include <lib/assert.h>

extern void v86_mode_enter(void);

/*--------------------------------------/
|wesolego nowego roku :))               |
|niech xanin rośnie i rośnie            |
|ja, rok 2021, 31 grudzień, 23:52:35    |
/--------------------------------------*/

void _start(void)
{

    asm("cli");    //disable interrupts while IDT is not configured

    disable_cursor();

    screen_clear();


    screen_init(); //init screen management system


    set_idt();
    time_get();
    keyboard_init();
    

    set_pit();

    keyboard_command = command_buffer;

    xprintf("DETECTING USB CONTROLLERS. PLEASE WAIT...\n");

    usb_detect();

    xprintf("\n%zUSB DETECTION TEST ENDED. PRESS ENTER TO START XANIN OS\n",
            set_output_color(green,white));


    xprintf("\n\n");
   
 

    //zsk("100");

    srand(Time.seconds);
	
    xin_init_fs();
    
   
    init_disk(ATA_FIRST_BUS, ATA_MASTER);

    screen_clear();
    xprintf("your memory: %d bytes", memory_map_get_cmos());
    while(1);

    tuiInit:

    screen_clear();
    
    time_get();
    
    xprintf("%z  .GBJ     ?BBY.                     ,,                              \n", set_output_color(logo_back_color, logo_front_color));
    xprintf("%z   :B#Y  !G#P^   .,,.     ..  ..     ''   ..  ..                     \n", set_output_color(logo_back_color, logo_front_color));
    xprintf("%z    .G#//#G~    ?PYJJ5P   HuCJJ5G    55  HuCJJJPP.                   \n", set_output_color(logo_back_color, logo_front_color));
    xprintf("%z     >###<     ~.   .G#.  I#D   7#:  GP  I#D    P#:                  \n", set_output_color(logo_back_color, logo_front_color));
    xprintf("%z   .5#BG#P.    !JYYJ?G#.  I#^   ^#:  GP  I#^    Y#:                  \n", set_output_color(logo_back_color, logo_front_color));
    xprintf("%z .Y##/ i#G.   ?#^   :G#.  I#^   ~#:  GP  I#^    Y#:%z  version 22.03v\n", set_output_color(logo_back_color, logo_front_color), set_output_color(black,white));
    xprintf("%z7GB5.    5BG. .5G555J?B^  I#^   ^#:  P5  I#^    JB:%z  weekday: %s   %i:%i\n\n\n", set_output_color(logo_back_color, logo_front_color), set_output_color(black,white), weekDaysLUT[Time.weekDay], Time.hour, Time.minutes);


    Screen.cursor[8][0] = (uint16_t)('>' | ((black << 4) | white) << 8);
    Screen.x = 1;
    Screen.y = 8;
    character_blocked = '>';

    KeyInfo.scan_code = 0x0;
    KeyInfo.character = 0x0;

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
