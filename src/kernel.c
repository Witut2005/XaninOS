
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


    
    uint32_t pci_address_selector = 0x0;

    xprintf("DETECTING USB CONTROLLERS. PLEASE WAIT...\n");

    for(pci_address_selector = 0x0; pci_address_selector < 2500000; pci_address_selector+=0x4) 
    {

        static uint32_t var, tmp; 
        tmp = var;


        pci_set_parameters(pci_config_address, pci_address_selector);

        var = pci_get_device_class(pci_address_selector);

 
        if(var == 0x0c03 && tmp != var)
        {


            xprintf("USB CONTROLLER DETECTED VENDOR ID: ");
            xprintf("0x%x\n",pci_get_vendor_id(pci_address_selector));          
            
            xprintf("USB CONTROLLER TYPE: %s\n", 
                    usb_controller_names[usb_controller_get_type(pci_address_selector) / 0x10]);
            
      

            //xprintf("write test: %d ", (uint16_t)pci_write_data32(pci_address_selector, 0x34, 0x0));
            

            xprintf("USB CONTROLLER BASE ADDRES 0x%x\n",pci_get_data32(pci_address_selector,0x20));  

        }
           
        if(var == 0x0106 && tmp != var)
        {
        
            xprintf("HARD DISK DETECTED\n");
            
        }
    

    }

    xprintf("\n\n%zUSB DETECTION TEST ENDED. PRESS ENTER TO START XANIN OS\n",
            set_output_color(green,white));
 
   

    keyboard_init();
    set_pit();

 
    xscanf("%d",x);

    //dma_controller_reset();

    clearScr();

    //disable_cursor();
    //enable_cursor(0x0,0x0);

    getCpuSpeed();

    getTime();

    srand(time.seconds);

    file_system_init();

    clearScr();   


	
    file_system_init();

    clearScr();

    x = 0x0;
    y = 0x0;

    
    //init_disk(ATA_SLAVE);
    //disk_read(ATA_SLAVE, 0x0,250, 0x7c00);
   
    init_disk(ATA_FIRST_BUS, ATA_MASTER);
    disk_read(ATA_FIRST_BUS, ATA_MASTER, 0x2, 512, 0x7c00);
    
    clearScr();

    uint16_t* tmp = (uint16_t*)0x7c00;
    
    for(int i = 0; i < 50; i++)
    	xprintf("%x %x ", tmp[i] & 0xFF, (tmp[i] >> 8) & 0xFF);


    //init_disk(ATA_SLAVE_BASE);


   

    tuiInit:

    clearScr();

    x = 0;y = 0;

    xprintf("xaninOS\n");
    xprintf("version 22.02v\n");

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
