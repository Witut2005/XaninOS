

#include "./pit/pit.c"
#include "./IDT/idt.c"
#include "./lib/stdlibx.h"
#include "./terminal/vty.c"
#include "./headers/inttypes.h"
//#include "./floppyDriver/floppyDriver.c"
#include "./terminal/interpreter.c"
#include "./lib/math.h"

uint16_t keyStatus;

char* tmpStr;

static char* floppyType[8] = 
{

    "error",
    "360kb",
    "1.2mb",
    "740kb",

    "1.44mb",
    "2.88mb",
    "undefined",
    "undefined"

};

static char* pw= "wiktoro";
static uint32_t foo;

//wesolego nowego roku :))
//niech xanin rośnie i rośnie 
// ja, rok 2021, 31 grudzień, 22:52:35 

void _start(void)
{



    //asm("cli");

    //DETECT FLOPPY TYPE
    //http://bos.asmhackers.net/docs/floppy/docs/detecting_floppy_drives.php.htm

    
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
        sprint(red,white,"keyboard self test failed. Halting execution");
        asm("cli");
        asm("hlt");
    }

    else if(KEYBOARD_TEST_STATUS == 0x55)
        sprint(black,white,"keyboard self test passed :))");



    tuiInit:

    COMMAND[0] = 'g';
    index++;

    clearScr();

    x = 0;y = 0;

    sprint(black,white,"xaninOS version 0.1v");


    sprint(black,white,"weekday:");
    getTime();
    sprint(black,white,weekDaysLUT[time.weekDay]);


    sprint(white,black,"press enter to start:");


    exitApp = false;





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