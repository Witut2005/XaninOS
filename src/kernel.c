
#include <stdint.h>
#include "./pit/pit.c"
#include "./IDT/idt.c"
#include "./lib/stdlibx.h"
#include "./terminal/vty.c"
#include "./terminal/interpreter.c"
#include "./lib/math.h"
#include "./ustar/ustar.c"

char* tmpStr;

/*--------------------------------------/
|wesolego nowego roku :))               |
|niech xanin rośnie i rośnie            |
|ja, rok 2021, 31 grudzień, 23:52:35    |     
/--------------------------------------*/



void _start(void)
{

    asm("cli");

    COMMAND = comBuf;
    
    kbInit();
    setPit();
    setIdt();
    ustarInit();

    clearScr();


    while(1);
    
    bool KEYBOARD_TEST_STATUS = keyboardSelfTest();


    if(KEYBOARD_TEST_STATUS == 0xfc) 
    {
        sprint(red,white,"keyboard self test failed. Halting execution\n");
        asm("cli");
        asm("hlt");
    }

    else if(KEYBOARD_TEST_STATUS == 0x55)
        sprint(black,white,"keyboard self test passed :))\n");


    clearScr();

    disableCursor();

   
    getCpuSpeed();

    getTime();

    srand(time.seconds);







    tuiInit:

    clearScr();

    x = 0;y = 0;

    xprintf("xaninOS\n");
    xprintf("version 22.01v\n");

    getTime();
    xprintf("weekday: %s\n\n",weekDaysLUT[time.weekDay]);


    *cursor = (uint16_t)('>' | ((black << 4) | white) << 8); 
    cursor++;
    x++;

    exitApp = false;
    

    while(1)
    {


        if(exitApp)
        {
            exitApp = false;
            for(int i = 0; i < sizeof(comBuf); i++)
                COMMAND[i] = '\0';
            goto tuiInit;
        }

      
        if(scanCode == ENTER)
        {
            xprintf("%z%s",getColors(blue,white),COMMAND);
            scan();
        }

    }
    
}