
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
#include <devices/VGA/vga.h>
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
#include <devices/PCSPK/pc_speaker.h>
// #include <network_protocols/ethernet_frame/ethernet_frame.h>
#include <netapi/network_device.h>
#include <network_protocols/arp/arp.h>
// #include <network_protocols/internet_protocol/ipv4/ip.h>
#include <libc/instruction_pointer.h>
#include <libc/xanin_state.h>
#include <libc/system.h>

extern void v86_mode_enter(void);
extern void mouse_enable(void);
extern void com_port_init(uint16_t divisor);
extern bool com_status(void);

/*--------------------------------------/
|wesolego nowego roku :))               |
|niech xanin rośnie i rośnie            |
|ja, rok 2021, 31 grudzień, 23:52:35    |
/--------------------------------------*/

/*----------------------------------------------/
|No i pierwszy rok na mną. Kto by się           | 
|spodziewał! Babciu byłabyś ze mnie dumna       |
|gdybyś mogła to wszystko zobaczyć              |
|To wszystko w końcu przez Ciebie hah <3        |
|Nigdy się nie poddam, tak jak Ci obiecywałem!  |
|Ja, rok 2022, 19 grudzień, 21:17:30            |
/----------------------------------------------*/

/*--------------------------------------/
|Wesolego nowego roku :))               |
|Oby był tak samo płodny jak poprzedni  |
|Niech Xanin rośnie i rośnie            |
|Nigdy się nie poddawaj mordo           |
|Przecież przysięgnąłeś Babci, że       |
|Zobaczy to co stworzysz!!!             |
|Ja, rok 2022, 31 grudzień, 17:00:45    |
/--------------------------------------*/

float pit_time = 0x0;

void kernel_loop(void)
{


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

        for(int i = 0; xin_current_directory[i + 1] != '\0'; i++)
            xprintf("%z%c", set_output_color(black, lblue), xin_current_directory[i]);

        xprintf(">");

        character_blocked   = '>';

        KeyInfo.scan_code   = 0x0;
        KeyInfo.character   = 0x0;

        app_exited          = false;


        while(1)
        {
            if(app_exited)
            {
                app_exited = false;
                for(int i = 0; i < sizeof(command_buffer); i++)
                    keyboard_command[i] = '\0';
                break;
            }


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

void _start(void)
{

    screen_init(); // init screen management system
    set_idt();

    disable_cursor();
    screen_clear();


    // xanin_cpu_backup_make();

    pmmngr_init(0x20000, 0xFFFFFF);
    pmmngr_init_region(0x20000, 0xFFFFFF);
    // init_disk(ATA_FIRST_BUS, ATA_MASTER);

    time_get(&SystemTime);

    keyboard_init();

    set_pit_divisor(0x8000);

    // set_pit();
    keyboard_command = command_buffer;

    rsdp = get_acpi_rsdp_address_base();

    xprintf("%z----------------------------\n", set_output_color(black, green));

    xprintf("CHECKSUM CHECK RSDP: ");
    1 == acpi_rsdp_checksum_check(rsdp) ? xprintf("%zVALID", set_output_color(green, white)) : xprintf("%zINVALID", set_output_color(red, white));
    xprintf("\nRSDP address: 0x%x\n", rsdp);

    rsdt = rsdp->rsdt_address;

    xprintf("%z----------------------------\n", set_output_color(black, green));

    xprintf("CHECKSUM CHECK RSDT: ");
    1 == acpi_rsdt_checksum_check(rsdt) ? xprintf("%zVALID", set_output_color(green, white)) : xprintf("%zINVALID", set_output_color(red, white));
    xprintf("\nRSDT address: 0x%x\n", rsdt);

    apic_sdt = apic_sdt_find();

    xprintf("%z----------------------------\n", set_output_color(black, green));

    xprintf("CHECKSUM CHECK MADT: ");
    1 == acpi_rsdt_checksum_check(rsdt) ? xprintf("%zVALID", set_output_color(green, white)) : xprintf("%zINVALID", set_output_color(red, white));
    xprintf("\nMADT address: 0x%x\n", rsdt);

    pic_disable();
    pic_mode_disable();

    madt_entries_get(apic_sdt);

    xprintf("YOUR IOAPIC\n");
    for (int i = 0; (*madt_entry_type1_ptr[i]).entry_type == 1; i++)
    {
        if ((*madt_entry_type1_ptr[i]).length == 0xC)
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
    xprintf("apic state: 0x%x\n", *(uint32_t *)APIC_SPURIOUS_INTERRUPT_VECTOR_REGISTER);

    static uint32_t ioapic_iso_couter; // iso = interrupt source override
    uint8_t used_irqs[32] = {0xFF};
    uint8_t used_irqs_counter = 0;
    
    for (int i = 0; (*madt_entry_type2_ptr[i]).entry_type == 2; i++)
    {
        bool is_already_used = false;

        for(int j = 0; j < 32; j++)
        {
            if(used_irqs[j] == (*madt_entry_type2_ptr[i]).irq_source)
            {
                is_already_used = true;
                break;
            }
        }

        if(!is_already_used)
        {
            xprintf(" %d %d |", (*madt_entry_type2_ptr[i]).irq_source, (*madt_entry_type2_ptr[i]).global_system_int_table);
            ioapic_iso_couter++;
            used_irqs[used_irqs_counter++] = (*madt_entry_type2_ptr[i]).irq_source;
        }
    }

    xprintf("\n");

    madt_entry_type2 *apic_keyboard_redirect = nullptr;
    madt_entry_type2 *apic_pit_redirect = nullptr;
    madt_entry_type2 *apic_nic_redirect = nullptr;
    madt_entry_type2 *apic_mouse_redirect = nullptr;

    for (int i = 0; (*madt_entry_type2_ptr[i]).entry_type == 2; i++)
    {
        if ((*madt_entry_type2_ptr[i]).irq_source == 1)
            apic_keyboard_redirect = madt_entry_type2_ptr[i];

        else if ((*madt_entry_type2_ptr[i]).irq_source == 0)
            apic_pit_redirect = madt_entry_type2_ptr[i];

        else if ((*madt_entry_type2_ptr[i]).irq_source == 0xB)
            apic_nic_redirect = madt_entry_type2_ptr[i];
        
    }

    ioapic_ioredtbl_configure((apic_keyboard_redirect != nullptr ? apic_keyboard_redirect->global_system_int_table + APIC_IRQ_BASE : PIC_KEYBOARD_VECTOR)
                                      << APIC_VECTOR |
                                  0x0 << APIC_DELIVERY_MODE | 0x0 << APIC_DESTINATION_MODE | 0x0 << APIC_INT_PIN_POLARITY | 0x0 << APIC_INT_MASK,
                              ioapic_id_get());

    ioapic_ioredtbl_configure((apic_pit_redirect != nullptr ? apic_pit_redirect->global_system_int_table + APIC_IRQ_BASE : PIC_PIT_VECTOR)
                                      << APIC_VECTOR |
                                  0x0 << APIC_DELIVERY_MODE | 0x0 << APIC_DESTINATION_MODE | 0x0 << APIC_INT_PIN_POLARITY | 0x0 << APIC_INT_MASK,
                              ioapic_id_get());

    ioapic_ioredtbl_configure((apic_nic_redirect != nullptr ? apic_nic_redirect->global_system_int_table + APIC_IRQ_BASE : PIC_NIC_VECTOR)
                                      << APIC_VECTOR |
                                  0x0 << APIC_DELIVERY_MODE | 0x0 << APIC_DESTINATION_MODE | 0x0 << APIC_INT_PIN_POLARITY | 0x0 << APIC_INT_MASK,
                              ioapic_id_get());

    ioapic_ioredtbl_configure((APIC_IRQ_BASE + 0xC)
                                      << APIC_VECTOR |
                                  0x0 << APIC_DELIVERY_MODE | 0x0 << APIC_DESTINATION_MODE | 0x0 << APIC_INT_PIN_POLARITY | 0x0 << APIC_INT_MASK,
                              ioapic_id_get());

    xprintf("\n%z----------------------------\n", set_output_color(black, green));
    xprintf("NIC interrupt line: 0x%x", (apic_nic_redirect != nullptr ? apic_nic_redirect->global_system_int_table + APIC_IRQ_BASE : PIC_NIC_VECTOR));

    xanin_info_ptr = xanin_information_block_get();

    xprintf("\n%z----------------------------\n", set_output_color(black, green));
    xprintf("Com port status: 0x%x\n", com_status());

    bootloader_program_buffer = (uint8_t *)malloc(sizeof(uint8_t) * SECTOR_SIZE); // must be before xin_init_fs
    memcpy(bootloader_program_buffer, (uint8_t *)0x7C00, SECTOR_SIZE);

    xprintf("Press ENTER to continue...\n");

    static int number_of_cores;

    // for (int i = 0; i < 10; i++)
    // {
    //     if (madt_entry_type0_ptr[i] != nullptr)
    //         number_of_cores++;
    // }

    // xprintf("Number of CPU cores: %d\n", number_of_cores);

    srand(SystemTime.seconds);

    disk_read(ATA_FIRST_BUS, ATA_MASTER, 0x1, 0x1, (uint16_t *)0x600);
    disk_read(ATA_FIRST_BUS, ATA_MASTER, 0x2, 0x1, (uint16_t *)0x400);

    // disk_read(ATA_FIRST_BUS, ATA_MASTER, 0x0, 0x1, (uint16_t*)0x7C00);

    disk_read(ATA_FIRST_BUS, ATA_MASTER, 0x4, 1, (uint16_t *)(0x500 * SECTOR_SIZE));
    disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x500, 1, (uint16_t *)(0x500 * SECTOR_SIZE));

    kernel_load_backup = (uint8_t *)calloc(512);
    memcpy(kernel_load_backup, (uint8_t *)0x20000, SECTOR_SIZE);

    argv[0] = program_name;
    argv[1] = program_parameters;
    argv[2] = program_parameters1;
    argv[3] = program_parameters2;
    argv[4] = program_parameters3;

    for (int i = 0; i < 5; i++)
        disk_read(ATA_FIRST_BUS, ATA_MASTER, 0x12 + i, 1, (uint16_t *)(0x800 + (i * SECTOR_SIZE)));

    for (int i = 0; i < 10; i++)
        disk_read(ATA_FIRST_BUS, ATA_MASTER, 0x1a + i, 1, (uint16_t *)(0x1800 + (i * SECTOR_SIZE)));

    xin_init_fs();

    create_file_kernel("/.system_space1");
    create_file_kernel("/.system_space2");
    create_file_kernel("/.system_space3");
    create_file_kernel("/.system_space4");
    create_file_kernel("/.system_space5");
    create_file_kernel("/.system_space6");
    create_file_kernel("/.system_space7");
    create_file_kernel("/.system_space8");
    create_file_kernel("/.system_space9");
    create_file_kernel("/.system_space10");
    create_file_kernel("/.system_space11");
    create_file_kernel("/.system_space12");
    create_file_kernel("/.system_space13");
    create_file_kernel("/.system_space14");
    create_file_kernel("/.system_space15");
    create_file_kernel("/.system_space16");
    create_file_kernel("/.system_space17");
    create_file_kernel("/.system_space18");
    create_file_kernel("/.system_space19");
    create_file_kernel("/.system_space20");
    create_file_kernel("/.system_space21");
    create_file_kernel("/.system_space22");
    create_file_kernel("/.system_space23");
    create_file_kernel("/.system_space24");
    create_file_kernel("/.system_space25");
    create_file_kernel("/.system_space26");
    create_file_kernel("/.system_space27");
    create_file_kernel("/.system_space28");
    create_file_kernel("/.system_space29");
    create_file_kernel("/.system_space30");
    create_file_kernel("/.system_space31");
    create_file_kernel("/.system_space32");
    create_file_kernel("/.system_space33");
    create_file_kernel("/.system_space34");
    create_file_kernel("/.system_space35");
    create_file_kernel("/.system_space36");
    create_file_kernel("/.system_space37");
    create_file_kernel("/.system_space38");
    create_file_kernel("/.system_space39");
    create_file_kernel("/.system_space40");
    create_file_kernel("/.system_space41");
    create_file_kernel("/.system_space42");
    create_file_kernel("/.system_space43");
    create_file_kernel("/.system_space44");
    create_file_kernel("/.system_space45");
    create_file_kernel("/.system_space46");
    create_file_kernel("/.system_space47");
    create_file_kernel("/.system_space48");
    create_file_kernel("/.system_space49");
    create_file_kernel("/.system_space50");
    create_file_kernel("/.system_space51");
    create_file_kernel("/.system_space52");
    create_file_kernel("/.system_space53");
    create_file_kernel("/.system_space54");
    create_file_kernel("/.system_space55");
    create_file_kernel("/.system_space56");
    create_file_kernel("/.system_space57");
    create_file_kernel("/.system_space58");
    create_file_kernel("/.system_space59");
    create_file_kernel("/.system_space60");
    create_file_kernel("/.system_space61");
    create_file_kernel("/.system_space62");
    create_file_kernel("/.system_space63");
    create_file_kernel("/.system_space64");
    create_file_kernel("/.system_space65");
    create_file_kernel("/.system_space66");
    create_file_kernel("/.system_space67");
    create_file_kernel("/.system_space68");
    create_file_kernel("/.system_space69");
    create_file_kernel("/.system_space70");
    create_file_kernel("/.system_space71");
    create_file_kernel("/.system_space72");
    create_file_kernel("/.system_space73");
    create_file_kernel("/.system_space74");
    create_file_kernel("/.system_space75");
    create_file_kernel("/.system_space76");
    create_file_kernel("/.system_space77");
    create_file_kernel("/.system_space78");
    create_file_kernel("/.system_space79");
    create_file_kernel("/.system_space80");

    FileDescriptorTable = (XinFileDescriptor*)calloc(sizeof(XinFileDescriptor) * 512);

    memset((uint8_t *)&ArpTable[0], 0xFF, sizeof(ArpTable[0]));
    current_arp_entry++;

    netapi_init();
    i8254x_init();

    // interrupt_enable();
    KeyInfo.is_ctrl = false;
    KeyInfo.is_shift = false;

    // __sys_xin_file_create("/syslog");
    xin_create_file("/syslog");
    printk("To wszystko dla Ciebie Babciu <3");

    // __sys_xin_folder_create("/config/");
    // __sys_xin_file_create("/config/nic.conf");
    // xin_entry* nic_config = fopen("/config/nic.conf", "rw");
    // fwrite(nic_config, "192.168.019.012  //XaninOS nic IP address(USE ALWAYS FULL OCTETS)", ARRAY_LENGTH("192.168.019.012  //XaninOS nic IP address(USE ALWAYS FULL OCTETS"));
    // fclose(&nic_config);
    
    xprintf("YOUR IP ADDRESS: ");
    uint32_t base_ip = xanin_ip_get();
    // xprintf("%d", base_ip);
    for(uint8_t i = 3; i > 0; i--)
    {
        uint8_t* tmp = (uint8_t*)&base_ip;
        xprintf("%d.", tmp[i]);
    }

    xprintf("%d\n", ((uint8_t*)&base_ip)[0]);

    char* bufsys = (char*)calloc(512);

    system_variable_get(&bufsys, "HOME");
    xprintf("bufsys: %s\n", bufsys);

    while (KeyInfo.scan_code != ENTER);

    kernel_loop();

}
