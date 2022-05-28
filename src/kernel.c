
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
#include <devices/VGA/vga.c>
#include <xin_fs/xin.c>
#include <lib/algorithm.h>
#include <lib/assert.h>
#include <lib/alloc.h>
#include <devices/APIC/apic_registers.h>
#include <devices/APIC/apic.c>

extern void v86_mode_enter(void);
extern void mouse_enable(void);

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


    set_idt();
    screen_init(); //init screen management system

    pmmngr_init(0x200000, 0xF00000);
    pmmngr_init_region(0x200000, 0xFFFFFF);

    init_disk(ATA_FIRST_BUS, ATA_MASTER);
    

    time_get();
    //mouse_install();
   
    keyboard_init();
    set_pit();
    keyboard_command = command_buffer;

    xprintf("DETECTING USB CONTROLLERS. PLEASE WAIT...\n");
    usb_detect();
    
    
    rsdp = get_acpi_rsdp_address_base();
    
    //xprintf("RSDP: 0x%x\n", rsdp);


    //acpi_print_rsdp();
    
    
    xprintf("CHECKSUM CHECK RSDP: ");
    1 == acpi_rsdp_checksum_check(rsdp) ? xprintf("%zVALID", set_output_color(green,white)) : xprintf("%zVALID", set_output_color(red,white));
    xprintf("\n");

    //while(1);

    rsdt = rsdp->rsdt_address;

    xprintf("CHECKSUM CHECK RSDT: ");
    1 == acpi_rsdt_checksum_check(rsdt) ? xprintf("%zVALID", set_output_color(green,white)) : xprintf("%zVALID", set_output_color(red,white));
    xprintf("\n");



    //apic_sdt = apic_sdt_find();

    //xprintf("ACPI APIC: %s\n", apic_sdt->signature);


    //xprintf("apic sdt addr: 0x%x\n", apic_sdt);
    //acpi_print_sdt((sdt*)rsdt);
    
    
    //madt_entries_get(apic_sdt);

    /*

    for(int i = 0; (*madt_entry_type1_ptr[i]).entry_type == 1; i++)
    { 
        //if((*madt_entry_type1_ptr[i]).io_apic_id != (*madt_entry_type1_ptr[i - 1]).io_apic_id)
        {
            xprintf("entry type: 0x%x\n", (*madt_entry_type1_ptr[i]).entry_type);
            xprintf("int: 0x%x\n", (*madt_entry_type1_ptr[i]).global_system_int_table);
        }
    }

    
    xprintf("ff: 0x%x\n", *(uint16_t*)0x40e);
    xprintf("ugabnuga: 0x%x\n", acpi_rsdp_checksum_check(rsdp));

    */

    uint8_t* xanin_info_ptr = xanin_information_block_get();



    while(KeyInfo.scan_code != ENTER);

    xprintf("\n\n");
   
    srand(Time.seconds);

    disk_read(ATA_FIRST_BUS, ATA_MASTER, 0x1a, 1, (uint16_t*)0x1800);
    disk_read(ATA_FIRST_BUS, ATA_MASTER, 0x1b, 1, (uint16_t*)0x1a00);

    for(int i = 0; i < 70; i++)
        disk_read(ATA_FIRST_BUS, ATA_MASTER, 0x1a + i, 1, (uint16_t*)(0x1800 + (i * SECTOR_SIZE)));


    xin_init_fs();
    


    screen_clear();

    char* omgtmp = (char*)pmmngr_alloc_block();
    xprintf("block test: 0x%x\n", omgtmp);
    //while(1);
    
    pmmngr_free_block(omgtmp);

    //0x7000
    omgtmp = (char*)realloc(omgtmp, 0, 4096 + 512);
    xprintf("block test: 0x%x\n", omgtmp);

    //0x9000
    omgtmp = (char*)pmmngr_alloc_block();
    xprintf("block test: 0x%x\n", omgtmp);


    //mouse_install();


    xprintf("apic: 0x%x", *(uint32_t*)0xFEE000F0);

    screen_clear();


    tuiInit:


    screen_clear();
    
    time_get();

    xprintf("%z    _/      _/                      _/              _/_/      _/_/_/       \n", set_output_color(logo_back_color, logo_front_color));
    xprintf("%z     _/  _/      _/_/_/  _/_/_/        _/_/_/    _/    _/  _/              \n", set_output_color(logo_back_color, logo_front_color));
    xprintf("%z      _/      _/    _/  _/    _/  _/  _/    _/  _/    _/    _/_/           \n", set_output_color(logo_back_color, logo_front_color));
    xprintf("%z   _/  _/    _/    _/  _/    _/  _/  _/    _/  _/    _/        _/%z  version 22.05v\n",set_output_color(logo_back_color, logo_front_color), set_output_color(black,white) );
    xprintf("%z_/      _/    _/_/_/  _/    _/  _/  _/    _/    _/_/    _/_/_/ %z%s %i:%i:%i\n\n\n", set_output_color(logo_back_color, logo_front_color), set_output_color(black,white), daysLUT[Time.weekday], Time.hour, Time.minutes, Time.seconds);                                       


    Screen.cursor[8][0] = (uint16_t)('>' | ((black << 4) | white) << 8);
    Screen.x = 1;
    Screen.y = 8;
    character_blocked = '>';

    KeyInfo.scan_code = 0x0;
    KeyInfo.character = 0x0;

    app_exited = false;
    arrows_navigate = true;


    //keyboard_handle = terminal_keyboard;

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
