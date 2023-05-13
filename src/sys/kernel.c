
#include <sys/devices/pit/pit.h>
#include <sys/terminal/interface/terminal.h>
#include <lib/libc/data_structures.h>
#include <limits.h>
#include <stdint.h>
#include <sys/interrupts/idt/idt.h>
#include <lib/libc/stdlibx.h>
#include <sys/terminal/vty/vty.h>
#include <sys/terminal/interpreter/interpreter.c>
#include <lib/libc/math.h>
#include <sys/devices/keyboard/keyboard_init.c>
#include <sys/devices/pci/pci.h>
#include <sys/devices/acpi/ACPI.h>
#include <sys/devices/usb/usb.h>
#include <sys/devices/hda/disk.h>
#include <sys/devices/vga/vga.h>
#include <fs/xin.h>
#include <lib/libc/assert.h>
#include <sys/pmmngr/alloc.h>
#include <sys/devices/apic/apic_registers.h>
#include <sys/devices/apic/apic.h>
#include <sys/devices/ioapic/ioapic.h>
#include <fs/loaders/elf/elf_loader.h>
#include <sys/devices/acpi/ACPI.h>
#include <sys/log/syslog.h>
#include <sys/devices/nic/8254x.h>
#include <sys/devices/pcspk/pc_speaker.h>
// #include <sys/net/network_protocols/ethernet_frame/ethernet_frame.h>
#include <sys/net/netapi/network_device.h>
#include <sys/net/network_protocols/arp/arp.h>
// #include <sys/net/network_protocols/internet_protocol/ipv4/ip.h>
#include <lib/libc/instruction_pointer.h>
#include <lib/libc/xanin_state.h>
#include <lib/libc/system.h>
#include <sys/interrupts/handlers/entries/handler_entries.h>

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

terminal_t* kernel_terminal;
uint8_t* const zeros;

#define PMMNGR_MEMORY_BLOCKS 10000

void terminal_time_update(address_t* args)
{
    time_get(&SystemTime);
    
    for(int i = 67; i < VGA_WIDTH; i++)
        Screen.cursor[4][i] = BLANK_SCREEN_CELL;

    xprintf("%h%s: %i:%i:%i\n\n\n", OUTPUT_POSITION_SET(4, 67), daysLUT[SystemTime.weekday], SystemTime.hour, SystemTime.minutes, SystemTime.seconds);
}

void kernel_loop(void)
{


    while(1)
    {

        all_intervals_clear(); // clear all intervals added by apps during execution
        interval_set(terminal_time_update, 1000, NULL); // refresh current time every second
        memset(null_memory_region, 0, SECTOR_SIZE);

        screen_clear();
        time_get(&SystemTime);

        xprintf("%z    _/      _/                      _/              _/_/      _/_/_/       \n", OUTPUT_COLOR_SET(logo_back_color, logo_front_color));
        xprintf("%z     _/  _/      _/_/_/  _/_/_/        _/_/_/    _/    _/  _/              \n", OUTPUT_COLOR_SET(logo_back_color, logo_front_color));
        xprintf("%z      _/      _/    _/  _/    _/  _/  _/    _/  _/    _/    _/_/           \n", OUTPUT_COLOR_SET(logo_back_color, logo_front_color));
        xprintf("%z   _/  _/    _/    _/  _/    _/  _/  _/    _/  _/    _/        _/%z   version 1.0v\n",OUTPUT_COLOR_SET(logo_back_color, logo_front_color), OUTPUT_COLOR_SET(black,white) );
        xprintf("%z_/      _/    _/_/_/  _/    _/  _/  _/    _/    _/_/    _/_/_/     %z%s: %i:%i:%i\n\n\n", OUTPUT_COLOR_SET(logo_back_color, logo_front_color), OUTPUT_COLOR_SET(black,white), daysLUT[SystemTime.weekday], SystemTime.hour, SystemTime.minutes, SystemTime.seconds);                                       

        Screen.x = 0;
        Screen.y = 8;

        for(int i = 0; xin_current_directory[i + 1] != '\0'; i++)
            xprintf("%z%c", OUTPUT_COLOR_SET(black, lblue), xin_current_directory[i]);

        xprintf(">");

        app_exited = false;

        // xin_close_all_files();

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
            

            scan();
        }   

    }

}

uint8_t kernel_mmngr_mmap[PMMNGR_MEMORY_BLOCKS];

void _start(void)
{

    interrupt_disable();

    INTERRUPT_REGISTER(0, divide_by_zero_exception_entry);
    INTERRUPT_REGISTER(1, debug_exception_entry);
    INTERRUPT_REGISTER(2, nmi_interrupt_exception_entry);
    INTERRUPT_REGISTER(3, breakpoint_exception_entry);
    INTERRUPT_REGISTER(4, overflow_exception_entry);
    INTERRUPT_REGISTER(5, bound_range_exceeded_exception_entry);
    INTERRUPT_REGISTER(6, invalid_opcode_exception_entry);
    INTERRUPT_REGISTER(7, device_not_available_exception_entry);
    INTERRUPT_REGISTER(8, double_fault_exception_entry);
    INTERRUPT_REGISTER(9, coprocessor_segment_overrun_exception_entry);
    INTERRUPT_REGISTER(10, invalid_tss_exception_entry);
    INTERRUPT_REGISTER(11, segment_not_present_exception_entry);
    INTERRUPT_REGISTER(12, stack_fault_exception_entry);
    INTERRUPT_REGISTER(13, general_protection_exception_entry);
    INTERRUPT_REGISTER(14, page_fault_exception_entry);
    INTERRUPT_REGISTER(15, x86_fpu_floating_point_exception_entry);
    INTERRUPT_REGISTER(17, aligment_check_exception_entry);
    INTERRUPT_REGISTER(18, machine_check_exception_entry);
    INTERRUPT_REGISTER(19, simd_floating_point_exception_entry);
    INTERRUPT_REGISTER(20, virtualization_exception_entry);
    INTERRUPT_REGISTER(21, general_protection_exception_entry); 
    INTERRUPT_REGISTER(22, general_protection_exception_entry); 
    INTERRUPT_REGISTER(23, general_protection_exception_entry); 
    INTERRUPT_REGISTER(24, general_protection_exception_entry); 
    INTERRUPT_REGISTER(25, general_protection_exception_entry); 
    INTERRUPT_REGISTER(26, general_protection_exception_entry);
    INTERRUPT_REGISTER(27, general_protection_exception_entry);
    INTERRUPT_REGISTER(28, general_protection_exception_entry);
    INTERRUPT_REGISTER(29, general_protection_exception_entry);
    INTERRUPT_REGISTER(30, general_protection_exception_entry);
    INTERRUPT_REGISTER(31, general_protection_exception_entry);
    INTERRUPT_REGISTER(32, general_protection_exception_entry);
    

    
    // INTERRUPT_REGISTER(0x21, keyboard_handler_init);

    screen_init(); // init screen management system
    screen_clear();
    keyboard_init(0x21);
    set_pit(0x20);
    // idt_examine();

    disable_cursor();

    // pmmngr_init(0x10000, ppmngr_bitmap);
    // pmmngr_init_region(0x1000, XANIN_PMMNGR_BLOCK_SIZE * (0x10000 / XANIN_PMMNGR_BLOCK_SIZE));

    // static uint8_t ppmngr_bitmap[0x10000];

    // pmmngr_init(0x10000, ppmngr_bitmap);
    // pmmngr_init_region(0x20000, XANIN_PMMNGR_BLOCK_SIZE * 500);

    // uint8_t ppmngr_bitmap[0x2000] = {0};

    // pmmngr_init(0x10000, ppmngr_bitmap);
    // pmmngr_init_region(0x0, 0xFFFFFF);

    mmngr_init(kernel_mmngr_mmap, 0x100000, PMMNGR_MEMORY_BLOCKS);


    time_get(&SystemTime);


    // set_pit();
    keyboard_command = command_buffer;
    screen_clear();

    null_memory_region = (uint8_t*)kcalloc(VGA_SCREEN_RESOLUTION);
    // xprintf("Memory Block Size Allocated: 0x%x\n", null_memory_region);
    // free(null_memory_region);
    // null_memory_region = (uint8_t*)calloc(VGA_SCREEN_RESOLUTION);
    // kernel_terminal = terminal_create();
    // terminal_set((terminal_t*)null_memory_region, kernel_terminal);

    // xprintf("Memory Block Size Allocated: 0x%x\n", null_memory_region);

    rsdp = get_acpi_rsdp_address_base();

    xprintf("%z----------------------------\n", OUTPUT_COLOR_SET(black, green));

    xprintf("CHECKSUM CHECK RSDP: ");
    1 == acpi_rsdp_checksum_check(rsdp) ? xprintf("%zVALID", OUTPUT_COLOR_SET(green, white)) : xprintf("%zINVALID", OUTPUT_COLOR_SET(red, white));
    xprintf("\nRSDP address: 0x%x\n", rsdp);

    rsdt = rsdp->rsdt_address;

    xprintf("%z----------------------------\n", OUTPUT_COLOR_SET(black, green));

    xprintf("CHECKSUM CHECK RSDT: ");
    1 == acpi_rsdt_checksum_check(rsdt) ? xprintf("%zVALID", OUTPUT_COLOR_SET(green, white)) : xprintf("%zINVALID", OUTPUT_COLOR_SET(red, white));
    xprintf("\nRSDT address: 0x%x\n", rsdt);

    apic_sdt = apic_sdt_find();

    xprintf("%z----------------------------\n", OUTPUT_COLOR_SET(black, green));

    xprintf("CHECKSUM CHECK MADT: ");
    1 == acpi_rsdt_checksum_check(rsdt) ? xprintf("%zVALID", OUTPUT_COLOR_SET(green, white)) : xprintf("%zINVALID", OUTPUT_COLOR_SET(red, white));
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
    xprintf("%z----------------------------\n", OUTPUT_COLOR_SET(black, green));

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

    madt_entry_type2 *apic_keyboard_redirect = NULL;
    madt_entry_type2 *apic_pit_redirect = NULL;
    madt_entry_type2 *apic_nic_redirect = NULL;
    madt_entry_type2 *apic_mouse_redirect = NULL;

    for (int i = 0; (*madt_entry_type2_ptr[i]).entry_type == 2; i++)
    {
        if ((*madt_entry_type2_ptr[i]).irq_source == 1)
            apic_keyboard_redirect = madt_entry_type2_ptr[i];

        else if ((*madt_entry_type2_ptr[i]).irq_source == 0)
            apic_pit_redirect = madt_entry_type2_ptr[i];

        else if ((*madt_entry_type2_ptr[i]).irq_source == 0xB)
            apic_nic_redirect = madt_entry_type2_ptr[i];
        
    }

    interrupt_disable();

    ioapic_ioredtbl_configure((apic_keyboard_redirect != NULL ? apic_keyboard_redirect->global_system_int_table + APIC_IRQ_BASE : PIC_KEYBOARD_VECTOR)
                                      << APIC_VECTOR |
                                  0x0 << APIC_DELIVERY_MODE | 0x0 << APIC_DESTINATION_MODE | 0x0 << APIC_INT_PIN_POLARITY | 0x0 << APIC_INT_MASK,
                              ioapic_id_get());

    ioapic_ioredtbl_configure((apic_pit_redirect != NULL ? apic_pit_redirect->global_system_int_table + APIC_IRQ_BASE : PIC_PIT_VECTOR)
                                      << APIC_VECTOR |
                                  0x0 << APIC_DELIVERY_MODE | 0x0 << APIC_DESTINATION_MODE | 0x0 << APIC_INT_PIN_POLARITY | 0x0 << APIC_INT_MASK,
                              ioapic_id_get());

    ioapic_ioredtbl_configure((apic_nic_redirect != NULL ? apic_nic_redirect->global_system_int_table + APIC_IRQ_BASE : PIC_NIC_VECTOR)
                                      << APIC_VECTOR |
                                  0x0 << APIC_DELIVERY_MODE | 0x0 << APIC_DESTINATION_MODE | 0x0 << APIC_INT_PIN_POLARITY | 0x0 << APIC_INT_MASK,
                              ioapic_id_get());
                        
    set_pit(apic_pit_redirect != NULL ? apic_pit_redirect->global_system_int_table + APIC_IRQ_BASE : PIC_PIT_VECTOR);
    keyboard_init(apic_keyboard_redirect != NULL ? apic_keyboard_redirect->global_system_int_table + APIC_IRQ_BASE : PIC_KEYBOARD_VECTOR);
    i8254x_init(apic_nic_redirect != NULL ? apic_nic_redirect->global_system_int_table + APIC_IRQ_BASE : PIC_NIC_VECTOR);
                
    interrupt_enable();

    // ioapic_ioredtbl_configure((APIC_IRQ_BASE + 0xC)
    //                                   << APIC_VECTOR |
    //                               0x0 << APIC_DELIVERY_MODE | 0x0 << APIC_DESTINATION_MODE | 0x0 << APIC_INT_PIN_POLARITY | 0x0 << APIC_INT_MASK,
    //                           ioapic_id_get());

    xprintf("\n%z----------------------------\n", OUTPUT_COLOR_SET(black, green));
    xprintf("NIC interrupt line: 0x%x", (apic_nic_redirect != NULL ? apic_nic_redirect->global_system_int_table + APIC_IRQ_BASE : PIC_NIC_VECTOR));

    xprintf("\n%z----------------------------\n", OUTPUT_COLOR_SET(black, green));
    xprintf("Com port status: 0x%x\n", com_status());

    xprintf("Press ENTER to continue...\n");

    static int number_of_cores;

    // for (int i = 0; i < 10; i++)
    // {
    //     if (madt_entry_type0_ptr[i] != NULL)
    //         number_of_cores++;
    // }

    // xprintf("Number of CPU cores: %d\n", number_of_cores);

    srand(SystemTime.seconds);

    disk_read(ATA_FIRST_BUS, ATA_MASTER, 0x1, 0x1, (uint16_t *)0x600);
    disk_read(ATA_FIRST_BUS, ATA_MASTER, 0x2, 0x1, (uint16_t *)0x400);

    // disk_read(ATA_FIRST_BUS, ATA_MASTER, 0x0, 0x1, (uint16_t*)0x7C00);

    // disk_read(ATA_FIRST_BUS, ATA_MASTER, 0x4, 1, (uint16_t *)(0x500 * SECTOR_SIZE));
    // disk_write(ATA_FIRST_BUS, ATA_MASTER, 0x500, 1, (uint16_t *)(0x500 * SECTOR_SIZE));

    argv[0] = program_name;
    argv[1] = program_parameters;
    argv[2] = program_parameters1;
    argv[3] = program_parameters2;
    argv[4] = program_parameters3;

    disk_read(ATA_FIRST_BUS, ATA_MASTER, 0x12, 8, (uint16_t *)XIN_ENTRY_POINTERS);
    disk_read(ATA_FIRST_BUS, ATA_MASTER, 0x1a, 10, (uint16_t *)XIN_ENTRY_TABLE);

    xin_init_fs();

    memset(XIN_ENTRY_POINTERS, 1, 0x280);

    FileDescriptorTable = (XinFileDescriptor*)kcalloc(sizeof(XinFileDescriptor) * 512);
    // xprintf("nicho");

    memset((uint8_t *)ArpTable, 0xFF, sizeof(ArpTable[0]));

    __sys_xin_file_create("/syslog");
    printk("To wszystko dla Ciebie Babciu <3");

    __sys_xin_folder_create("/config/");
    // xprintf("nicho");

    // xprintf("YOUR IP ADDRESS: ");
    // uint32_t base_ip = xanin_ip_get();
    // for(uint8_t i = 3; i > 0; i--)
    // {
    //     uint8_t* tmp = (uint8_t*)&base_ip;
    //     xprintf("%d.", tmp[i]);
    // }

    // xprintf("%d\n", base_ip & 0xF

    arp_table_add_entry(LOOPBACK_IP_ADDRESS, null_memory_region);
    arp_module_init();
    icmp_module_init();
    
    uint8_t* zeros = (uint8_t*)kcalloc(SECTOR_SIZE);

    while (inputg().scan_code != ENTER);

    // system_variable_get(&bufsys, "HOME");
    // xprintf("bufsys: %s\n", bufsys);

    // stack_t* MyStack = stack_create();
    // for(int i = 0; i < 10; i++)
    //     stack_push(MyStack, i);
    // screen_clear();
    // xprintf("poped value: \n");
    // for(int i = 0; stack_is_empty(MyStack) != true; i++)
    // {
    //     xprintf("%d\n", stack_pop(MyStack));
    // }
    // time_offset_t* omg = start();
    // stop(omg);
    // xprintf("time offset: %d\n", *omg);

    // char* hm = (char*)calloc(10);
    // strcpy(hm, "piwko");
    // xprintf("%s\n", strconcat("/", hm);
    // inputg();

    // xprintf("ARP module status: %d\n", arp_module_status());

    // xprintf("nicho");

    // screen_clear();
    //     xprintf("fsadf");
    //     uint8_t good = 0x02;
    //     while (good & 0x02)
    //         good = inbIO(0x64);
    //     outbIO(0x64, 0xFE);
    // // address_t* as = NULL;
    
    // msleep(10000);

    // for(int i = 0; i < 10; i++)
    // {
    //     xprintf("a");
    //     msleep(1000);
    // }

    // while (inputg().scan_code != ENTER);

    kernel_loop();

}
