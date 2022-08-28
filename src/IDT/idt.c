#pragma once

#include <stdint.h>
#include <handlers/handlers.c>
#include <keyboard/key_map.h>
#include <syscall/syscall.c>
#include <devices/MOUSE/mouse.c>


//extern void mouse_handler(void);

extern void mouse_handler_init(void);
extern void i8254x_interrupt_handler_entry(void);

#define IDT_HANDLERS 256
#define IDT_SIZE 256 * 8 - 1


#define CODE_SEGMENT 0x8

/* configure interrupt descriptor table entry */
#define configure_idt_entry(idt_entry,off,seg)\
    idtEntries[idt_entry].off_0_15 = (uint16_t)(((uint32_t)&off & 0x0000ffff));\
    idtEntries[idt_entry].off_16_31 = (uint16_t)((uint32_t)&off >> 16);\
    idtEntries[idt_entry].segment = seg;\
    idtEntries[idt_entry].res = 0x0;\
    idtEntries[idt_entry].P_DPL = 0x8e


//extern void _syscall(void);

/* IDT Register */
struct idt_register
{
    uint16_t limit;
    uint32_t base;
}__attribute__((packed));

/* IDT entry structure */
typedef struct
{
    uint16_t off_0_15;
    uint16_t segment;
    uint8_t res;
    uint8_t P_DPL;
    uint16_t off_16_31;
}__attribute__((packed)) IDT;


__attribute__((aligned(0x8))) IDT idtEntries[IDT_HANDLERS];


// void configure_idt_entry(uint8_t idt_entry, uint32_t offset, uint16_t segment)
// {
//     idtEntries[idt_entry].off_0_15 = (uint16_t)(((uint32_t)&offset & 0x0000ffff));
//     idtEntries[idt_entry].off_16_31 = (uint16_t)((uint32_t)&offset >> 16);
//     idtEntries[idt_entry].segment = segment;
//     idtEntries[idt_entry].res = 0x0;
//     idtEntries[idt_entry].P_DPL = 0x8e;
// }

void set_idt(void)
{

    asm("cli");

    /* configure IDT entries*/
    configure_idt_entry(0x0, divide_by_zero_exception,CODE_SEGMENT);
    configure_idt_entry(0x1, debug_exception, CODE_SEGMENT);
    configure_idt_entry(0x2, nmi_interrupt,CODE_SEGMENT);
    configure_idt_entry(0x3, breakpoint_exception, CODE_SEGMENT);
    configure_idt_entry(0x4, overflow_exception, CODE_SEGMENT);
    configure_idt_entry(0x5, nmi_interrupt,CODE_SEGMENT);
    configure_idt_entry(0x6, invalid_opcode,CODE_SEGMENT);
    configure_idt_entry(0x7, device_not_available_exception,CODE_SEGMENT);
    configure_idt_entry(0x8, double_fault_exception,CODE_SEGMENT);
    configure_idt_entry(0x9, coprocessor_segment_overrun,CODE_SEGMENT);
    configure_idt_entry(0xa, invalid_tss_exception,CODE_SEGMENT);
    configure_idt_entry(0xb, segment_not_present,CODE_SEGMENT);
    configure_idt_entry(0xc, stack_fault_exception,CODE_SEGMENT);
    configure_idt_entry(13, general_protection_exception, CODE_SEGMENT);
    configure_idt_entry(0xe, page_fault_exception, CODE_SEGMENT);
    configure_idt_entry(0xf, x86_fpu_floating_point_exception, CODE_SEGMENT);
    configure_idt_entry(17, aligment_check_exception, CODE_SEGMENT);
    configure_idt_entry(18, machine_check_exception,CODE_SEGMENT);
    configure_idt_entry(19, simd_floating_point_exception, CODE_SEGMENT);
    configure_idt_entry(20, virtualization_exception,CODE_SEGMENT);
    configure_idt_entry(21, control_protection_exception,CODE_SEGMENT);
    
    configure_idt_entry(0x22, pit_handler_init,CODE_SEGMENT);
    configure_idt_entry(0x21, keyboard_handler_init,CODE_SEGMENT);
    
    configure_idt_entry(0x26, floppy_interrupt,CODE_SEGMENT);
    configure_idt_entry(0x2B, i8254x_interrupt_handler_entry, CODE_SEGMENT);
    configure_idt_entry(0x2B + 1, gowno, CODE_SEGMENT);
    configure_idt_entry(0x2B + 2, gowno, CODE_SEGMENT);
    configure_idt_entry(0x2B + 3, gowno, CODE_SEGMENT);
    configure_idt_entry(0x2B + 4, gowno, CODE_SEGMENT);
    configure_idt_entry(0x2B + 5, gowno, CODE_SEGMENT);
    configure_idt_entry(0x2B + 6, gowno, CODE_SEGMENT);
    configure_idt_entry(0x2B + 7, gowno, CODE_SEGMENT);

    // configure_idt_entry(0x2C, mouse_handler_init, CODE_SEGMENT);

    configure_idt_entry(0x80, syscall,CODE_SEGMENT);
    configure_idt_entry(0x50, elf_correctly_loaded,CODE_SEGMENT);
    //configure_idt_entry(0x81, no_handler,CODE_SEGMENT);
    //configure_idt_entry(0xFF, reboot_interrupt,CODE_SEGMENT);

    struct idt_register idtr = {
        IDT_SIZE,
        (uint32_t)&idtEntries
    };

    /* load IDT Register with proper struct */
    interrupt_enable();
    asm("lidt %0" : : "m"(idtr));



}
