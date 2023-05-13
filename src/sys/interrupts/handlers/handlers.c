
#pragma once

 
#include <lib/libc/hal.h>
#include <sys/log/syslog.h>
#include <lib/libc/stdiox.h>
#include <sys/devices/keyboard/key_map.h>
#include <sys/devices/keyboard/keyboard_driver.c>
#include <sys/interrupts/handlers/handlers.h>
#include <sys/devices/pit/pit.h>

//#include <sys/devices/DMA/dma.c>



extern void kernel_loop(void);
extern void pit_handler_init(void);
extern void keyboard_handler_init(void);

void exception_print(const char* message) 
{
    if(Screen.y != VGA_HEIGHT - 1)
        Screen.y++;
    else
        Screen.y = VGA_HEIGHT - 1;

    Screen.x = 0;
    xprintf("%zERROR: %s\n", stderr, message);
    interrupt_enable();
    while(inputg().scan_code != ENTER);
}

void invalid_opcode_exception_handler(void)
{
    printk("ERROR: INVALID OPCODE");
    exception_print("INVALID OPCODE EXCEPTION");
}

void divide_by_zero_exception_handler(void)
{
    printk("ERROR: DIVIDE BY ZERO EXCEPTION");
    exception_print("DIVIDE BY ZERO EXCEPTION");
}

 
void debug_exception_handler(void)
{
    printk("ERROR: DEBUG EXCEPTION");
    exception_print("DEBUG EXCEPTION");
}

void nmi_interrupt_exception_handler(void)
{
    printk("ERROR: NMI EXCEPTION");
    exception_print("NMI EXCEPTION");
}

void breakpoint_exception_handler(void)
{
    printk("ERROR: BREAKPOINT EXCEPTION");
    exception_print("BREAKPOINT EXCEPTION");
}

void overflow_exception_handler(void)
{
    printk("ERROR: OVERFLOW EXCEPTION");
    exception_print("OVERFLOW EXCEPTION");
}

void bound_range_exceeded_exception_handler(void)
{
    printk("ERROR: BOUND RANGE EXCEEDED EXCEPTION");
    exception_print("BOUND RANGE EXCEEDED EXCEPTION");
}

void device_not_available_exception_handler(void)
{
    printk("ERROR: DEVICE NOT AVAIBLE EXCEPTION");
    exception_print("DEVICE NOT AVAILABLE EXCEPTION");
}

void double_fault_exception_handler(void)
{
    printk("ERROR: DOUBLE FAULT EXCEPTION");
    exception_print("DOUBLE FAULT EXCEPTION");
}

void coprocessor_segment_overrun_exception_handler(void)
{
    printk("ERROR: COPROCESSOR SEGMENT OVERRUN");
    exception_print("COPROCESSOR SEGMENT OVERRUN");
}

void invalid_tss_exception_handler(void)
{
    printk("ERROR: INVALID TSS EXCEPTION");
    exception_print("INVALID TSS EXCETPION");
}

void segment_not_present_exception_handler(void)
{
    printk("ERROR: SEGMENT NOT PRESENT");
    exception_print("SEGMENT NOT PRESENT");
}

void stack_fault_exception_handler(void)
{
    printk("ERROR: STACK FAULT EXCEPTION");
    exception_print("STACK FAULT EXCEPTION");
}

void general_protection_exception_handler(void)
{   
    printk("ERROR: GENERAL PROTECTION EXCEPTION");
    exception_print("GENERAL PROTECTION EXCEPTION");
}

void page_fault_exception_handler(void)
{
    printk("ERROR: PAGE FAULT EXCEPTION");
    exception_print("PAGE FAULT EXCEPTION");
}
    
void x86_fpu_floating_point_exception_handler(void)
{
    printk("ERROR: x86 FPU FLOATING POINT EXCEPTION");
    exception_print("x86 FPU FLOATING POINT EXCEPTION");
}

void aligment_check_exception_handler(void)
{
    printk("ERROR: ALIGMENT CHECK EXCEPTION");
    exception_print("ALIGMNET CHECK EXCEPTION");
}

void machine_check_exception_handler(void)
{
    printk("ERROR: MACHINE CHECK EXCEPTION");
    exception_print("MACHINE CHECK EXCEPTION");
}

void simd_floating_point_exception_handler(void)
{
    printk("ERROR: SIMD FLOATING POINT EXCEPTION");
    exception_print("SIMD FLOATING POINT EXCEPTION");
}

void virtualization_exception_handler(void)
{
    printk("ERROR: VIRTUALIZATION EXCEPTION");
    exception_print("VIRTUALIZATION EXCEPTION");
}

void elf_correctly_loaded_handler(void)
{
    printk("ELF OK");
}

void control_protection_exception_handler(void)
{
    printk("ERROR: CONTROL PROTECTION EXCEPTION");
    exception_print("CONTROL PROTECTION EXCETPION");
}


void reboot_interrupt(void)
{
    asm("int 0xFF");
}
