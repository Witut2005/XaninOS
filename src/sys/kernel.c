
#include <sys/net/network_protocols/internet_protocol/ipv4/ip.h>
#include <lib/ascii/ascii.h>
#include <sys/devices/pit/pit.h>
#include <lib/libc/data_structures.h>
#include <limits.h>
#include <stdint.h>
#include <sys/interrupts/idt/idt.h>
#include <lib/libc/stdlibx.h>
#include <lib/screen/screen.h>
#include <sys/terminal/interpreter/interpreter.c>
#include <lib/libc/math.h>
#include <sys/devices/keyboard/keyboard_init.c>
#include <sys/devices/pci/pci.h>
#include <sys/devices/acpi/acpi.h>
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
#include <sys/log/syslog.h>
#include <sys/devices/nic/8254x.h>
#include <sys/devices/pcspk/pc_speaker.h>
#include <sys/devices/acpi/fadt/fadt.h>
// #include <sys/net/network_protocols/ethernet_frame/ethernet_frame.h>
#include <sys/net/netapi/network_device.h>
#include <sys/net/network_protocols/arp/arp.h>
// #include <sys/net/network_protocols/internet_protocol/ipv4/ip.h>
#include <lib/libc/xanin_state.h>
#include <lib/libc/system.h>
#include <sys/interrupts/handlers/entries/handler_entries.h>
#include <sys/call/xanin_sys/calls/pmmngr/alloc.h>
#include <sys/terminal/backend/backend.h>
#include <sys/terminal/frontend/frontend.h>
#include <lib/cpu/headers/cpu_state_info.h>
#include <sys/macros.h>
#include <lib/libc/stdiox_legacy.h>

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

uint8_t* const zeros;

uint32_t stdio_refresh_rate;
interval_id stdio_refresh_interval_id;

void stdio_refresh(address_t* args)
{
    if((stdio_mode_get() == STDIO_MODE_TERMINAL) && (__xtb_get()->is_flushable))
        __sys_xtb_flush(__sys_vty_get());
}

void kernel_loop(void)
{
    while(1)
    {

        xtb_enable_flushing();
        stdio_mode_set(STDIO_MODE_TERMINAL);
        __xtb_flush_all(__vty_get());

        all_intervals_clear(); // clear all intervals added by apps during execution

        interval_set(stdio_refresh, stdio_refresh_rate, NULL); // refresh interval
        
        memset(null_memory_region, 0, SECTOR_SIZE);
        __xtf_scrolling_on(__vty_get());

        // screen_clear();
        time_get(&SystemTime);

        puts("\n");

        for(int i = 0; xin_current_directory[i + 1] != '\0'; i++)
            xprintf("%z%c", OUTPUT_COLOR_SET(black, lblue), xin_current_directory[i]);

        puts(">");

        app_exited = false;

        xin_close_all_files();

        if(app_exited)
            app_exited = false;


        char scanf_str[] = "%s %s %s %s %s";

        for(int i = 0; i < 5; i++)
            memset(argv[i], 0, XANIN_PMMNGR_BLOCK_SIZE * 2);

        xscanf(scanf_str,argv[0], argv[1], argv[2], argv[3], argv[4]);

        for(int i = 0; i < 5; i++)
            erase_spaces(argv[i]);
        
        scan();
    }
}

uint8_t kernel_mmngr_mmap[PMMNGR_MEMORY_BLOCKS];

Xtf* VtyFront;

void _start(void)
{

    syslog_disable();
    
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


    interrupt_enable();
    
    keyboard_init(0x21);
    set_pit(0x20);
    // idt_examine();

    disable_cursor();
    mmngr_init(kernel_mmngr_mmap, (uint8_t*)0x100000, PMMNGR_MEMORY_BLOCKS);

    // SCREEN MANAGER USES CALLOC

    vga_text_mode_height = 25;
    vga_text_mode_width = 80;

    screen_init(); // init screen management system
    screen_clear();

    __xtb_init(__vga_text_mode_width_get(), __vga_text_mode_height_get(), (uint16_t*)__vga_buffer_segment_get());
    __vty_set(xtf_init(100));
    stdio_mode_set(STDIO_MODE_TERMINAL);

    time_get(&SystemTime);

    null_memory_region = (uint8_t*)kcalloc(VGA_SCREEN_RESOLUTION);
    // xprintf("Memory Block Size Allocated: 0x%x\n", null_memory_region);
    // free(null_memory_region);
    // null_memory_region = (uint8_t*)calloc(VGA_SCREEN_RESOLUTION);
    // kernel_terminal = terminal_create();
    // terminal_set((terminal_t*)null_memory_region, kernel_terminal);

    xprintf("%z----------------------------\n", OUTPUT_COLOR_SET(black, green));

    puts("CHECKSUM CHECK RSDP: ");

    SystemAcpiRSDP* rsdp = acpi_rsdp_find();

    1 == acpi_rsdp_checksum_check(rsdp) ? xprintf("%zVALID", OUTPUT_COLOR_SET(green, white)) : xprintf("%zINVALID", OUTPUT_COLOR_SET(red, white));
    xprintf("\nRSDP address: 0x%x\n", rsdp);

    acpi_rsdt_set((SystemAcpiRSDT*)rsdp->rsdt_address);

    xprintf("%z----------------------------\n", OUTPUT_COLOR_SET(black, green));

    puts("CHECKSUM CHECK RSDT: ");
    const SystemAcpiRSDT* const rsdt = acpi_rsdt_get();
    1 == acpi_rsdt_checksum_check(rsdt) ? xprintf("%zVALID", OUTPUT_COLOR_SET(green, white)) : xprintf("%zINVALID", OUTPUT_COLOR_SET(red, white));
    xprintf("\nRSDT address: 0x%x\n", rsdt);

    SystemAcpiSDT* AcpiApicSDT = apic_sdt_find();

    xprintf("%z----------------------------\n", OUTPUT_COLOR_SET(black, green));

    SystemAcpiFADT* AcpiFADT = acpi_fadt_find();

    puts("FADT address: ");
    xprintf("%z0x%x\n", OUTPUT_COLOR_SET(black, acpi_sdt_checksum_check((uint8_t*)AcpiFADT, AcpiFADT->length) == true ? green : red), AcpiFADT);
    
    puts("MADT address: ");
    xprintf("%z0x%x\n", OUTPUT_COLOR_SET(black, acpi_sdt_checksum_check((uint8_t*)AcpiApicSDT, AcpiApicSDT->length) == true ? green : red), AcpiApicSDT);

    xprintf("MADT entries: 0x%x\n", (uint8_t*)AcpiApicSDT + 0x28);

    pic_disable();
    pic_mode_disable();

    madt_entries_get(AcpiApicSDT);

    puts("YOUR IOAPIC\n");
    for (int i = 0; (*AcpiMADT1Pointers[i]).entry_type == 1; i++) // ignore not initialized enttries
    {
        if ((*AcpiMADT1Pointers[i]).length == 0xC)
        {
            xprintf("ENTRY_TYPE     0x%x\n", (*AcpiMADT1Pointers[i]).entry_type);
            xprintf("ENTRY LENGTH   0x%x\n", (*AcpiMADT1Pointers[i]).length);
            xprintf("ID             0x%x\n", (*AcpiMADT1Pointers[i]).io_apic_id);
            xprintf("RES            0x%x\n", (*AcpiMADT1Pointers[i]).reserved);
            xprintf("IOAPIC BASE    0x%x\n", (*AcpiMADT1Pointers[i]).io_apic_base);
            xprintf("GSIB           0x%x\n", (*AcpiMADT1Pointers[i]).global_system_int_table);
            ioapic_init((*AcpiMADT1Pointers[i]).io_apic_base);
            break;
        }
    }
    xprintf("%z----------------------------\n", OUTPUT_COLOR_SET(black, green));

    apic_enable();
    xprintf("apic state: 0x%x\n", *(uint32_t *)APIC_SPURIOUS_INTERRUPT_VECTOR_REGISTER);

    static uint32_t ioapic_iso_couter; // iso = interrupt source override
    uint8_t used_irqs[32] = {0xFF};
    uint8_t used_irqs_counter = 0;
    
    for (int i = 0; (*AcpiMADT2Pointers[i]).entry_type == 2; i++)
    {
        bool is_already_used = false;

        for(int j = 0; j < 32; j++)
        {
            if(used_irqs[j] == (*AcpiMADT2Pointers[i]).irq_source)
            {
                is_already_used = true;
                break;
            }
        }

        if(!is_already_used)
        {
            xprintf(" %d %d |", (*AcpiMADT2Pointers[i]).irq_source, (*AcpiMADT2Pointers[i]).global_system_int_table);
            ioapic_iso_couter++;
            used_irqs[used_irqs_counter++] = (*AcpiMADT2Pointers[i]).irq_source;
        }
    }

    SystemAcpiMADT2 *apic_keyboard_redirect = NULL;
    SystemAcpiMADT2 *apic_pit_redirect = NULL;
    SystemAcpiMADT2 *apic_nic_redirect = NULL;
    // SystemAcpiMADT2 *apic_mouse_redirect = NULL;

    for (int i = 0; (*AcpiMADT2Pointers[i]).entry_type == 2; i++)
    {
        if ((*AcpiMADT2Pointers[i]).irq_source == 1)
            apic_keyboard_redirect = AcpiMADT2Pointers[i];

        else if ((*AcpiMADT2Pointers[i]).irq_source == 0)
            apic_pit_redirect = AcpiMADT2Pointers[i];

        else if ((*AcpiMADT2Pointers[i]).irq_source == 0xB)
            apic_nic_redirect = AcpiMADT2Pointers[i];
        
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
                
    interrupt_disable();

    // ioapic_ioredtbl_configure((APIC_IRQ_BASE + 0xC)
    //                                   << APIC_VECTOR |
    //                               0x0 << APIC_DELIVERY_MODE | 0x0 << APIC_DESTINATION_MODE | 0x0 << APIC_INT_PIN_POLARITY | 0x0 << APIC_INT_MASK,
    //                           ioapic_id_get());

    xprintf("\n%z----------------------------\n", OUTPUT_COLOR_SET(black, green));
    xprintf("NIC interrupt line: 0x%x", (apic_nic_redirect != NULL ? apic_nic_redirect->global_system_int_table + APIC_IRQ_BASE : PIC_NIC_VECTOR));

    xprintf("\n%z----------------------------\n", OUTPUT_COLOR_SET(black, green));
    xprintf("Com port status: 0x%x\n", com_status());

    puts("Press ENTER to continue...\n");

    // static int number_of_cores;

    // for (int i = 0; i < 10; i++)
    // {
    //     if (AcpiMADT0Pointers[i] != NULL)
    //         number_of_cores++;
    // }

    // xprintf("Number of CPU cores: %d\n", number_of_cores);

    srand(SystemTime.seconds);

    disk_read(ATA_FIRST_BUS, ATA_MASTER, 0x1, 0x1, (uint16_t *)0x600);
    disk_read(ATA_FIRST_BUS, ATA_MASTER, 0x2, 0x1, (uint16_t *)0x400);

    argv[0] = (char*)calloc(XANIN_PMMNGR_BLOCK_SIZE * 2); 
    argv[1] = (char*)calloc(XANIN_PMMNGR_BLOCK_SIZE * 2); 
    argv[2] = (char*)calloc(XANIN_PMMNGR_BLOCK_SIZE * 2); 
    argv[3] = (char*)calloc(XANIN_PMMNGR_BLOCK_SIZE * 2); 
    argv[4] = (char*)calloc(XANIN_PMMNGR_BLOCK_SIZE * 2); 

    // LOAD XIN TABLES
    // disk_read(ATA_FIRST_BUS, ATA_MASTER, 0x12, 8, (uint16_t *)XIN_ENTRY_POINTERS);
    // disk_read(ATA_FIRST_BUS, ATA_MASTER, 0x1a, 10, (uint16_t *)XIN_ENTRY_TABLE);
    xin_init_fs();

    disk_write(ATA_FIRST_BUS, ATA_MASTER, xin_find_entry("/ivt")->first_sector, 2, 0x0); // load ivt to /ivt file

    // xin_init_fs();
    xin_folder_change("/");
    FileDescriptorTable = (XinFileDescriptor*)kcalloc(SIZE_OF(XinFileDescriptor) * 200); // 200 = number o entries
    

    memset((uint8_t *)ArpTable, 0xFF, SIZE_OF(ArpTable[0]));

    __sys_xin_file_create("/syslog");
    syslog_enable();
    printk("To wszystko dla Ciebie Babciu <3");
    __sys_xin_folder_create("/config/");

    arp_table_add_entry(LOOPBACK_IP_ADDRESS, null_memory_region);
    arp_module_init();
    icmp_module_init();
    
    // uint8_t* zeros = (uint8_t*)kcalloc(SECTOR_SIZE);

    interrupt_enable();

    // screen_clear();
    // vga_mode_set(VGA_GRAPHICS_320x200x256);
    // vga_mode_set(VGA_TEXT_80x25);

    // screen_clear();
    // Registers Regs;
    // xprintf("0x%x\n", &Regs);
    // __asm_registers_values_get(&Regs);

    // for(uint32_t i = 0; i < 9; i++)
    // {
    //     xprintf("0x%x\n", *((uint32_t*)(&Regs) + i));
    // }

    // uint16_t* seg_regs = (uint16_t*)((uint32_t*)(&Regs) + 9);

    // for(uint32_t i = 0; i < 4; i++)
    // {
    //     xprintf("0x%x\n", seg_regs[i]);
    // }

    // INIT AUTOMATIC STDIO FLUSH

    char* buffer = (char*)kcalloc(100 * SIZE_OF(char));

    XinEntry* StdioRefreshRateConfig = fopen("/etc/stdio/refresh_rate.conf", "r");
    fseek(StdioRefreshRateConfig, ARRAY_LENGTH("STDIO_REFRESH_RATE: ") - 1);
    fread(StdioRefreshRateConfig, buffer, 99);

    stdio_refresh_rate = strtoi(buffer, 10);

    if(stdio_refresh_rate <= 100)
        xprintf("Stdio new refresh rate: %d\n", stdio_refresh_rate);

    else
    {
        puts_error("Stdio refresh rate cannot be more than 100ms\n");
        stdio_refresh_rate = 100;
    }

    kfree(buffer);

    ////////////////////////////////

    stdio_refresh(NULL);
    // xprintf("dupa: %d\n", strtoi("1", 10));

    while(getxchar().scan_code != ENTER);
    screen_clear();

    if(vga_text_mode_width == 80)
    {
        xprintf("%z    _/      _/                      _/              _/_/      _/_/_/       \n", OUTPUT_COLOR_SET(logo_back_color, logo_front_color));
        xprintf("%z     _/  _/      _/_/_/  _/_/_/        _/_/_/    _/    _/  _/              \n", OUTPUT_COLOR_SET(logo_back_color, logo_front_color));
        xprintf("%z      _/      _/    _/  _/    _/  _/  _/    _/  _/    _/    _/_/           \n", OUTPUT_COLOR_SET(logo_back_color, logo_front_color));
        xprintf("%z   _/  _/    _/    _/  _/    _/  _/  _/    _/  _/    _/        _/%z   version 1.9v", OUTPUT_COLOR_SET(logo_back_color, logo_front_color), OUTPUT_COLOR_SET(black,white));
        xprintf("%z_/      _/    _/_/_/  _/    _/  _/  _/    _/    _/_/    _/_/_/     %z%s: %i:%i:%i\n", OUTPUT_COLOR_SET(logo_back_color, logo_front_color), OUTPUT_COLOR_SET(black,white), daysLUT[SystemTime.weekday], SystemTime.hour, SystemTime.minutes, SystemTime.seconds);                                       
    }

    char stdio_legacy_config_buf[6] = {0};
    XinEntry* StdioLegacyConfig = fopen("/etc/help/stdio_legacy.conf", "rw");
    fseek(StdioLegacyConfig, ARRAY_LENGTH("PRINT_LEGACY_STDIO_INFO: ") - 1);
    fread(StdioLegacyConfig, stdio_legacy_config_buf, 5);

    if(bstrncmp(stdio_legacy_config_buf, "TRUE", 4))
    {
        fseek(StdioLegacyConfig, 25);
        fwrite(StdioLegacyConfig, "FALSE", 6);

        puts_warning("SINCE V1.8, XANIN USES TWO DIFFERENT GRAPHIC MODES. IF YOU WANT\nTO RUN THE PROGRAM IN A GIVEN MODE, HOLD CTRL WHILE SUBMITTING A COMMAND\n");

    }

    fclose(&StdioLegacyConfig);

    kernel_loop();

}
