
#pragma once

#include <lib/signal.h>
#include <lib/hal.h>
#include <lib/stdiox.h>
#include <terminal/vty.c>
#include <headers/macros.h>
#include <keyboard/keyMap.h>
#include <keyboard/keyboard_driver.c>
#include <handlers/handlers.h>

//#include <devices/DMA/dma.c>



extern void pit_handler_init(void);
extern void keyboard_handler_init(void);


void floppy_interrupt(void)
{
    sprint(red,white,"FLOPPY ERROR");
    
    asm("cli");
    asm("hlt");
}

void invalid_opcode(void)
{
    screen_clear();
    xprintf("\n%zINVALID OPCODE",set_output_color(red,white));
    reg_dump();
    asm("cli");
    asm("hlt");
}

void divide_by_zero_exception(void)
{
    screen_clear();
    xprintf("\n%DIVIDE BY ZERO ERROR",set_output_color(red,white));
    reg_dump();
    asm("cli");
    asm("hlt");
}

void pit_handler(void)
{
    pitActive = true;

    pit_current_time++;

}

void keyboard_handler(void)
{
    keyStatus = inbIO(KEYBOARD_STATUS_REG); // if status & 1 (ON)
    keyboard_scan_code = inbIO(KEYBOARD_DATA_REG); // get keyboard_scan_code
    keyboard_driver(keyboard_scan_code);
}
 
void debug_exception(void)
{
    screen_clear();
    xprintf("%zDEBUG EXCEPTION", set_output_color(red,white));
    reg_dump();
    asm("cli");
    asm("hlt");
}

void nmi_interrupt(void)
{
    screen_clear();
    xprintf("%zNMI INTERRUPT", set_output_color(red,white));
    reg_dump();
    asm("cli");
    asm("hlt");
}

void breakpoint_exception(void)
{
    screen_clear();
    xprintf("%zBREAKPOINT EXCEPTION", set_output_color(red,white));
    reg_dump();
    asm("cli");
    asm("hlt");
}

void overflow_exception(void)
{
    screen_clear();
    xprintf("%zOVERFLOW EXCEPTION", set_output_color(red,white));
    reg_dump();
    asm("cli");
    asm("hlt");
}

void bound_range_exceeded_exception(void)
{
    screen_clear();
    xprintf("\n%zBOUND Range Exceeded Exception", set_output_color(red,white));
    reg_dump();
    asm("cli");
    asm("hlt");
}

void device_not_available_exception(void)
{
    screen_clear();
    xprintf("\n%zDevice not available exception", set_output_color(red,white));
    reg_dump();
    asm("cli");
    asm("hlt");
}

void double_fault_exception(void)
{
    screen_clear();
    xprintf("\n%zDouble fault exception", set_output_color(red,white));
    reg_dump();
    asm("cli");
    asm("hlt");
}

void coprocessor_segment_overrun(void)
{
    screen_clear();
    xprintf("\n%zCoprocessor segment overrun", set_output_color(red,white));
    reg_dump();
    asm("cli");
    asm("hlt");
}

void invalid_tss_exception(void)
{
    screen_clear();
    xprintf("%zInvalid TSS exception", set_output_color(red,white));
    reg_dump();
    asm("cli");
    asm("hlt");
}

void segment_not_present(void)
{
    screen_clear();
    xprintf("\n%zSegment not present", set_output_color(red,white));
    reg_dump();
    asm("cli");
    asm("hlt");
}

void stack_fault_exception(void)
{
    screen_clear();
    xprintf("\n%zStack fault exception", set_output_color(red,white));
    reg_dump();
    asm("cli");
    asm("hlt");
}

void general_protection_exception(void)
{
    screen_clear();
    xprintf("\n%zGeneral protection exception\n", set_output_color(red,white));
    reg_dump();
    asm("cli");
    asm("hlt");
}

void page_fault_exception(void)
{
    screen_clear();
    xprintf("%zPage fault exception", set_output_color(red,white));
    reg_dump();
    asm("cli");
    asm("hlt");
}
    
void x86_fpu_floating_point_exception(void)
{
    screen_clear();
    xprintf("%zx86 fpu floating point exception", set_output_color(red,white));
    reg_dump();
    asm("cli");
    asm("hlt");
}

void aligment_check_exception(void)
{
    screen_clear();
    xprintf("%zAligment check exception", set_output_color(red,white));
    reg_dump();
    asm("cli");
    asm("hlt");
}

void machine_check_exception(void)
{
    screen_clear();
    xprintf("%zMachine check exception", set_output_color(red,white));
    reg_dump();
    asm("cli");
    asm("hlt");
}

void simd_floating_point_exception(void)
{
    screen_clear();
    xprintf("%zSIMD floating point exception", set_output_color(red,white));
    reg_dump();
    asm("cli");
    asm("hlt");
}

void virtualization_exception(void)
{
    screen_clear();
    xprintf("%zVirtualization exception", set_output_color(red,white));
    reg_dump();
    asm("cli");
    asm("hlt");
}

void control_protection_exception(void)
{
    screen_clear();
    xprintf("%zControl protection exception", set_output_color(red,white));
    reg_dump();
    asm("cli");
    asm("hlt");
}


void reboot_interrupt(void)
{
    asm("int 0xFF");
}