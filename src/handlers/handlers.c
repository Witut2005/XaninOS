
#pragma once

 
#include <libc/hal.h>
#include <libc/stdiox.h>
#include <headers/macros.h>
#include <keyboard/key_map.h>
#include <keyboard/keyboard_driver.c>
#include <handlers/handlers.h>
#include <pit/pit.h>

//#include <devices/DMA/dma.c>



extern void pit_handler_init(void);
extern void keyboard_handler_init(void);


void floppy_interrupt(void)
{
    xprintf("FLOPPY ERROR");
    
    interrupt_disable();
    asm("hlt");
}

void gowno(void)
{
    xprintf("gowno");
    xprintf("%zNIC INTERRUPT\n", set_output_color(green,white));
    while(1);
}

void invalid_opcode(void)
{
    //screen_clear();
    xprintf("\n%zINVALID OPCODE",set_output_color(red,white));
    //reg_dump();
    interrupt_disable();
    asm("hlt");
}

void divide_by_zero_exception(void)
{
    screen_clear();
    int tmp;
    asm("mov %0, [esp]"
        : 
        : "r"(tmp));
        
    xprintf("\n%zDIVIDE BY ZERO ERROR",set_output_color(red,white));
    xprintf("0x%x\n", tmp);
    // reg_dump();
    interrupt_disable();
    asm("hlt");
}

void pit_handler(void)
{
    //xprintf("h");
    pit_tick(0xFFFF);
    outbIO(0x20, 0x20);
}

void keyboard_handler(void)
{
    keyStatus = inbIO(KEYBOARD_STATUS_REG); // if status & 1 (ON)
    KeyInfo.scan_code = inbIO(KEYBOARD_DATA_REG); // get KeyInfo.scan_code
    keyboard_driver(KeyInfo.scan_code);
    outbIO(0x20,0x20);
}
 
void debug_exception(void)
{
    screen_clear();
    xprintf("%zDEBUG EXCEPTION", set_output_color(red,white));
    reg_dump();
    interrupt_disable();
    asm("hlt");
}

void nmi_interrupt(void)
{
    // screen_clear();
    // xprintf("%zNMI INTERRUPT", set_output_color(red,white));
    // reg_dump();
    interrupt_disable();
    asm("hlt");
}

void breakpoint_exception(void)
{
    screen_clear();
    xprintf("%zBREAKPOINT EXCEPTION", set_output_color(red,white));
    reg_dump();
    interrupt_disable();
    asm("hlt");
}

void overflow_exception(void)
{
    screen_clear();
    xprintf("%zOVERFLOW EXCEPTION", set_output_color(red,white));
    reg_dump();
    interrupt_disable();
    asm("hlt");
}

void bound_range_exceeded_exception(void)
{
    screen_clear();
    xprintf("\n%zBOUND Range Exceeded Exception", set_output_color(red,white));
    reg_dump();
    interrupt_disable();
    asm("hlt");
}

void device_not_available_exception(void)
{
    screen_clear();
    xprintf("\n%zDevice not available exception", set_output_color(red,white));
    reg_dump();
    interrupt_disable();
    asm("hlt");
}

void double_fault_exception(void)
{
    screen_clear();
    xprintf("\n%zDouble fault exception", set_output_color(red,white));
    reg_dump();
    interrupt_disable();
    asm("hlt");
}

void coprocessor_segment_overrun(void)
{
    screen_clear();
    xprintf("\n%zCoprocessor segment overrun", set_output_color(red,white));
    reg_dump();
    interrupt_disable();
    asm("hlt");
}

void invalid_tss_exception(void)
{
    screen_clear();
    xprintf("%zInvalid TSS exception", set_output_color(red,white));
    reg_dump();
    interrupt_disable();
    asm("hlt");
}

void segment_not_present(void)
{
    screen_clear();
    xprintf("\n%zSegment not present", set_output_color(red,white));
    reg_dump();
    interrupt_disable();
    asm("hlt");
}

void stack_fault_exception(void)
{
    screen_clear();
    xprintf("\n%zStack fault exception", set_output_color(red,white));
    reg_dump();
    interrupt_disable();
    asm("hlt");
}

void general_protection_exception(void)
{
    screen_clear();
    xprintf("\n%zGeneral protection exception\n", set_output_color(red,white));
    reg_dump();
    interrupt_disable();
    asm("hlt");
}

void page_fault_exception(void)
{
    screen_clear();
    xprintf("%zPage fault exception", set_output_color(red,white));
    reg_dump();
    interrupt_disable();
    asm("hlt");
}
    
void x86_fpu_floating_point_exception(void)
{
    screen_clear();
    xprintf("%zx86 fpu floating point exception", set_output_color(red,white));
    reg_dump();
    interrupt_disable();
    asm("hlt");
}

void aligment_check_exception(void)
{
    screen_clear();
    xprintf("%zAligment check exception", set_output_color(red,white));
    reg_dump();
    interrupt_disable();
    asm("hlt");
}

void machine_check_exception(void)
{
    screen_clear();
    xprintf("%zMachine check exception", set_output_color(red,white));
    reg_dump();
    interrupt_disable();
    asm("hlt");
}

void simd_floating_point_exception(void)
{
    screen_clear();
    xprintf("%zSIMD floating point exception", set_output_color(red,white));
    reg_dump();
    interrupt_disable();
    asm("hlt");
}

void virtualization_exception(void)
{
    screen_clear();
    xprintf("%zVirtualization exception", set_output_color(red,white));
    reg_dump();
    interrupt_disable();
    asm("hlt");
}

void elf_correctly_loaded(void)
{
    xprintf("\n%zELF loaded", set_output_color(black,green));
    eoi_send();
    interrupt_enable();
    return;
}

void control_protection_exception(void)
{
    screen_clear();
    xprintf("%zControl protection exception", set_output_color(red,white));
    reg_dump();
    interrupt_disable();
    asm("hlt");
}

void no_handler(void)
{
    asm("out 0x20, al":: "ax"(0x20));
    
    while(1);
}

void reboot_interrupt(void)
{
    asm("int 0xFF");
}
