
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

    keyboard_init();
    set_pit();


    //dma_controller_reset();


    clearScr();

    //disable_cursor();
    //enable_cursor(0x0,0x0);

    getCpuSpeed();

    getTime();

    srand(time.seconds);


    file_system_init();

    clearScr();

    xprintf("%s\n",get_acpi_address_base());


    
    /*    Detect USB devices    
    uint32_t pci_address_selector = 0x0;

    do
    {




        
            1111 1111
            pic_address_selector & 0xff <-- register offset
            USED 0-7

            111 0000 0000
            ((pic_address_selector & 0x700) >> 8) <-- function number
            USED 8-10

            1111 1000 0000 0000
            ((pic_address_selector & 0xf800) >> 11) <-- device number
            USED 11-15

            1111 1111 0000 0000 0000 0000
            ((pic_address_selector & 0xff0000) >> 16) <- bus num
            

        

         PCI NOT PIC 

        uint16_t var = pci_get_vendor_id(((pci_address_selector & 0xff0000) >> 16),
                            ((pci_address_selector & 0xf800) >> 11),
                            ((pci_address_selector & 0x700) >> 8),
                            pci_address_selector & 0xff);

        if((var && 0xff000000) >> 24 == 0x0C)
            xprintf("DETECTED USB DEVICE");

        pci_address_selector++;

    }   while(!pci_address_selector);

    asm("jmp $");

    */


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

    app_exited = false;




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
