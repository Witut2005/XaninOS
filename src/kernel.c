
#include <limits.h>
#include <stdint.h>
#include <IDT/idt.c>
#include <libc/stdlibx.h>
#include <terminal/vty.h>
#include <terminal/interpreter.c>
#include <libc/math.h>
#include <keyboard/keyboard_init.c>
#include <devices/PCI/pci.c>
#include <devices/ACPI/ACPI.c>
#include <devices/USB/usb.c>
#include <devices/HARD_DISK/disk.c>
#include <devices/VGA/vga.c>
#include <xin_fs/xin.h>
#include <libc/assert.h>
#include <libc/alloc.h>
#include <devices/APIC/apic_registers.h>
#include <devices/APIC/apic.h>
#include <devices/IOAPIC/ioapic.h>
#include <elf/elf_loader.c>
#include <devices/ACPI/ACPI.h>
#include <libc/syslog.h>
#include <xanin_info/info_block.c>
#include <xin_fs/xin.h>
#include <devices/NIC/8254x.h>
// #include <network_protocols/ethernet_frame/ethernet_frame.h>
#include <netapi/network_device.h>
#include <network_protocols/arp/arp.h>
// #include <network_protocols/internet_protocol/ipv4/ip.h>
#include <libc/instruction_pointer.h>
#include <libc/xanin_state.h>

extern void v86_mode_enter(void);
extern void mouse_enable(void);
extern void com_port_init(uint16_t divisor);
extern bool com_status(void);

/*--------------------------------------/
|wesolego nowego roku :))               |
|niech xanin rośnie i rośnie            |
|ja, rok 2021, 31 grudzień, 23:52:35    |
/--------------------------------------*/
float pit_time = 0x0;

void _start(void)
{
    
    interrupt_disable();

    uint32_t* p = (uint32_t*)VGA_TEXT_MEMORY;
    *p = 0x42424242;
    
    interrupt_disable();            //disable interrupts while IDT is not configured

    disable_cursor();
    screen_clear();


    set_idt();
    screen_init();                  //init screen management system

    xanin_cpu_backup_make();

    pmmngr_init(0x20000, 0xFFFFFF);
    pmmngr_init_region(0x20000, 0xFFFFFF);
    init_disk(ATA_FIRST_BUS, ATA_MASTER);
    
    time_get(&SystemTime);
   
    keyboard_init();
    set_pit();
    keyboard_command = command_buffer;

    //usb_detect();

    // while(KeyInfo.scan_code != ENTER);
    
    rsdp = get_acpi_rsdp_address_base();


    xprintf("%z----------------------------\n", set_output_color(black, green));
    
    xprintf("CHECKSUM CHECK RSDP: ");
    1 == acpi_rsdp_checksum_check(rsdp) ? xprintf("%zVALID", set_output_color(green,white)) : xprintf("%zINVALID", set_output_color(red,white));
    xprintf("\nRSDP address: 0x%x\n", rsdp);

    rsdt = rsdp->rsdt_address;

    xprintf("%z----------------------------\n", set_output_color(black, green));

    xprintf("CHECKSUM CHECK RSDT: ");
    1 == acpi_rsdt_checksum_check(rsdt) ? xprintf("%zVALID", set_output_color(green,white)) : xprintf("%zINVALID", set_output_color(red,white));
    xprintf("\nRSDT address: 0x%x\n", rsdt);


    apic_sdt = apic_sdt_find();

    xprintf("%z----------------------------\n", set_output_color(black, green));

    xprintf("CHECKSUM CHECK MADT: ");
    1 == acpi_rsdt_checksum_check(rsdt) ? xprintf("%zVALID", set_output_color(green,white)) : xprintf("%zINVALID", set_output_color(red,white));
    xprintf("\nMADT address: 0x%x\n", rsdt);

    pic_disable();
    pic_mode_disable();
    

    madt_entries_get(apic_sdt);


    xprintf("%z----------------------------\n", set_output_color(black, green));
    
    xprintf("YOUR IOAPIC\n");
    for(int i = 0; (*madt_entry_type1_ptr[i]).entry_type == 1; i++)
    {        
        if((*madt_entry_type1_ptr[i]).length == 0xC)
        {
            xprintf("ENTRY_TYPE     0x%x\n", (*madt_entry_type1_ptr[i]).entry_type);
            xprintf("ENTRY LENGTH   0x%x\n", (*madt_entry_type1_ptr[i]).length);        
            xprintf("ID             0x%x\n", (*madt_entry_type1_ptr[i]).io_apic_id);
            xprintf("RES            0x%x\n", (*madt_entry_type1_ptr[i]).reserved);
            xprintf("IOAPIC BASE    0x%x\n", (*madt_entry_type1_ptr[i]).io_apic_base);
            xprintf("GSIB           0x%x\n", (*madt_entry_type1_ptr[i]).global_system_int_table);
            ioapic_init((*madt_entry_type1_ptr[i]).io_apic_base);
            break;
        }
    }
    xprintf("%z----------------------------\n", set_output_color(black, green));

    apic_enable();
    xprintf("apic state: 0x%x\n", *(uint32_t*)APIC_SPURIOUS_INTERRUPT_VECTOR_REGISTER);

    static uint32_t ioapic_iso_couter; //iso = interrupt source override 

    for(int i = 0; (*madt_entry_type2_ptr[i]).entry_type == 2; i++)
    {
        xprintf(" %d %d |", (*madt_entry_type2_ptr[i]).irq_source, (*madt_entry_type2_ptr[i]).global_system_int_table);
        ioapic_iso_couter++;        
    }




    madt_entry_type2* apic_keyboard_redirect = nullptr; 
    madt_entry_type2* apic_pit_redirect      = nullptr; 
    madt_entry_type2* apic_nic_redirect      = nullptr; 


    for(int i = 0; (*madt_entry_type2_ptr[i]).entry_type == 2; i++)
    {
        if((*madt_entry_type2_ptr[i]).irq_source == 1)
            apic_keyboard_redirect = madt_entry_type2_ptr[i];

        else if((*madt_entry_type2_ptr[i]).irq_source == 0)
            apic_pit_redirect = madt_entry_type2_ptr[i];

        else if((*madt_entry_type2_ptr[i]).irq_source == 0xB)
            apic_nic_redirect = madt_entry_type2_ptr[i];

    }


    ioapic_ioredtbl_configure((apic_keyboard_redirect != nullptr ? apic_keyboard_redirect->global_system_int_table + APIC_IRQ_BASE : PIC_KEYBOARD_VECTOR)
                                    << APIC_VECTOR | 0x0 << APIC_DELIVERY_MODE | 0x0 << APIC_DESTINATION_MODE 
                                        | 0x0 << APIC_INT_PIN_POLARITY | 0x0 << APIC_INT_MASK, ioapic_id_get());

    ioapic_ioredtbl_configure((apic_pit_redirect != nullptr ? apic_pit_redirect->global_system_int_table + APIC_IRQ_BASE : PIC_PIT_VECTOR)
                                    << APIC_VECTOR | 0x0 << APIC_DELIVERY_MODE | 0x0 << APIC_DESTINATION_MODE 
                                        | 0x0 << APIC_INT_PIN_POLARITY | 0x0 << APIC_INT_MASK, ioapic_id_get());

    ioapic_ioredtbl_configure((apic_nic_redirect != nullptr ? apic_nic_redirect->global_system_int_table + APIC_IRQ_BASE : PIC_NIC_VECTOR)
                                    << APIC_VECTOR | 0x0 << APIC_DELIVERY_MODE | 0x0 << APIC_DESTINATION_MODE 
                                        | 0x0 << APIC_INT_PIN_POLARITY | 0x0 << APIC_INT_MASK, ioapic_id_get());

    xprintf("\n%z----------------------------\n", set_output_color(black, green));
    xprintf("NIC interrupt line: 0x%x", (apic_nic_redirect != nullptr ? apic_nic_redirect->global_system_int_table + APIC_IRQ_BASE : PIC_NIC_VECTOR));

    xanin_info_ptr = xanin_information_block_get();
    
    // com_port_init(0x00C0);

    xprintf("\n%z----------------------------\n", set_output_color(black, green));
    xprintf("Com port status: 0x%x\n", com_status());

    bootloader_program_buffer = (uint8_t*) malloc(sizeof(uint8_t) * SECTOR_SIZE); //must be before xin_init_fs
    memcpy(bootloader_program_buffer, (uint8_t*)0x7C00, SECTOR_SIZE);

    xprintf("Press ENTER to continue...\n");

    static int number_of_cores;

    for(int i = 0; i < 10; i++)
    {
        if(madt_entry_type0_ptr[i] != nullptr)
            number_of_cores++;
    }

    xprintf("Number of CPU cores: %d\n", number_of_cores);

    srand(SystemTime.seconds);


    for(int i = 0; i < 70; i++)
        disk_read(ATA_FIRST_BUS, ATA_MASTER, 0x1a + i, 1, (uint16_t*)(0x1800 + (i * SECTOR_SIZE)));

    
    disk_read(ATA_FIRST_BUS, ATA_MASTER, 0x1, 0x1, (uint16_t*)0x600);
    disk_read(ATA_FIRST_BUS, ATA_MASTER, 0x2, 0x1, (uint16_t*)0x400);

    //disk_read(ATA_FIRST_BUS, ATA_MASTER, 0x0, 0x1, (uint16_t*)0x7C00);

    
    disk_read(ATA_FIRST_BUS, ATA_MASTER, 0x4, 1, (uint16_t*)(0x500 * SECTOR_SIZE));
    disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x500, 1, (uint16_t*)(0x500 * SECTOR_SIZE));

    kernel_load_backup = (uint8_t*)calloc(512);
    memcpy(kernel_load_backup, (uint8_t*)0x20000, SECTOR_SIZE);

    argv[0] = program_name;
    argv[1] = program_parameters;
    argv[2] = program_parameters1;
    argv[3] = program_parameters2;
    argv[4] = program_parameters3;

    xin_init_fs();


    create_file_kernel("/system_space1");
    create_file_kernel("/system_space2");
    create_file_kernel("/system_space3");
    create_file_kernel("/system_space4");
    create_file_kernel("/system_space5");
    create_file_kernel("/system_space6");
    create_file_kernel("/system_space7");
    create_file_kernel("/system_space8");
    create_file_kernel("/system_space9");
    create_file_kernel("/system_space10");
    create_file_kernel("/system_space20");
    create_file_kernel("/system_space30");
    create_file_kernel("/system_space11");
    create_file_kernel("/system_space21");
    create_file_kernel("/system_space31");
    create_file_kernel("/system_space12");
    create_file_kernel("/system_space22");
    create_file_kernel("/system_space32");
    create_file_kernel("/system_space13");
    create_file_kernel("/system_space225");
    create_file_kernel("/system_space325");
    create_file_kernel("/system_space135");
    create_file_kernel("/system_space235");
    create_file_kernel("/system_space33");

    memset((uint8_t*)&ArpTable[0], 0xFF, sizeof(ArpTable[0]));
    current_arp_entry++;
    

    netapi_init();
    i8254x_init();

    
    // interrupt_enable();
    KeyInfo.is_ctrl = false;
    KeyInfo.is_shift = false;


    // __sys_xin_file_create("/syslog");
    xin_create_file("/syslog");
    printk("To wszystko dla Ciebie Babciu <3");
    printk("Nigdy sie nie poddam");
    // xin_entry* aha = fopen("/syslog", "rw");
    // write(aha, "kernel successfully loaded", 20);
    while(KeyInfo.scan_code != ENTER);


    while(1)
    {


        screen_clear();
        
        time_get(&SystemTime);

        xprintf("%z    _/      _/                      _/              _/_/      _/_/_/       \n", set_output_color(logo_back_color, logo_front_color));
        xprintf("%z     _/  _/      _/_/_/  _/_/_/        _/_/_/    _/    _/  _/              \n", set_output_color(logo_back_color, logo_front_color));
        xprintf("%z      _/      _/    _/  _/    _/  _/  _/    _/  _/    _/    _/_/           \n", set_output_color(logo_back_color, logo_front_color));
        xprintf("%z   _/  _/    _/    _/  _/    _/  _/  _/    _/  _/    _/        _/%z  version 22.10v\n",set_output_color(logo_back_color, logo_front_color), set_output_color(black,white) );
        xprintf("%z_/      _/    _/_/_/  _/    _/  _/  _/    _/    _/_/    _/_/_/     %z%s: %i:%i:%i\n\n\n", set_output_color(logo_back_color, logo_front_color), set_output_color(black,white), daysLUT[SystemTime.weekday], SystemTime.hour, SystemTime.minutes, SystemTime.seconds);                                       




        Screen.x            = 0;
        Screen.y            = 8;
        
        
        //xprintf("%z%s%z >", set_output_color(black, green), xin_current_directory, set_output_color(black, white));

        for(int i = 0; xin_current_directory[i + 1] != '\0'; i++)
            xprintf("%z%c", set_output_color(black, lblue), xin_current_directory[i]);

        xprintf(">");

        character_blocked   = '>';

        KeyInfo.scan_code   = 0x0;
        KeyInfo.character   = 0x0;

        app_exited          = false;


        //keyboard_handle = terminal_keyboard;

        while(1)
        {
            if(app_exited)
            {
                app_exited = false;
                for(int i = 0; i < sizeof(command_buffer); i++)
                    keyboard_command[i] = '\0';
                break;
            }

            /*
            if(xanin_info_ptr->program_to_execute != nullptr)
            {
                xprintf("%s\n", xanin_info_ptr->signature);
                xprintf("0x%x\n", xanin_info_ptr->program_to_execute);
                void(*program_to_execute)(void) = (void(*)(void))xanin_info_ptr->program_to_execute;
                program_to_execute();
            }
            */
            char scanf_str[40] = "%s %s %s %s %s";

            for(int i = 0; i < 40; i++)
                program_name[i] = '\0';
            
            for(int i = 0; i < 40; i++)
                program_parameters[i] = '\0';

            xscanf(scanf_str,program_name, program_parameters, program_parameters1, program_parameters2, program_parameters3);

            memcpy(last_used_commands, program_name, sizeof(program_name));
            memcpy(last_used_parameters, program_parameters, sizeof(program_parameters));


            erase_spaces(program_name);
            erase_spaces(program_parameters);
            

            character_blocked = '\0';

            KeyInfo.character = 0x0;
            KeyInfo.scan_code = 0x0;
            scan();
        }   

    }



}
