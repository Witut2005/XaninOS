
#include <stdint.h>
#include "./pit/pit.c"
#include "./IDT/idt.c"
#include "./lib/stdlibx.h"
#include "./terminal/vty.c"
#include "./terminal/interpreter.c"
#include "./lib/math.h"
#include "./ustar/ustar.c"

char* tmpStr;


static char* pw= "wiktoro";
static uint32_t foo;

//wesolego nowego roku :))
//niech xanin rośnie i rośnie 
// ja, rok 2021, 31 grudzień, 23:52:35 



void _start(void)
{

    asm("cli");
    
    clearScr();

    kbInit();
    setPit();

    setIdt();



    clearScr();

    disableCursor();

   
    getCpuSpeed();

    getTime();

    srand(time.seconds);


    //outbIO(PIC1_DATA_REG, 0xFD); //mask pit ints

    bool KEYBOARD_TEST_STATUS = keyboardSelfTest();


    if(KEYBOARD_TEST_STATUS == 0xfc) 
    {
        sprint(red,white,"keyboard self test failed. Halting execution\n");
        asm("cli");
        asm("hlt");
    }

    else if(KEYBOARD_TEST_STATUS == 0x55)
        sprint(black,white,"keyboard self test passed :))\n");



    tuiInit:

    COMMAND[0] = 'g';
    index++;

    clearScr();

    x = 0;y = 0;

    sprint(black,white,"xaninOS version 22.01v\n");


    sprint(black,white,"weekday:");
    getTime();
    sprint(black,white,weekDaysLUT[time.weekDay]);


    sprint(white,black,"press enter to start:\n");


    exitApp = false;

    ustarInit();

    



    while(1)
    {


        if(exitApp)
            {exitApp = false;goto tuiInit;}

      
        if(scanCode == ENTER)
        {
            scan();
        }

    }
    
}