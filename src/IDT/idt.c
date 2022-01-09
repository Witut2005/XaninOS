#pragma once

#include <stdint.h>
#include "../lib/hal.h"
#include "../handlers/handlers.c"
#include "../keyboard/keyMap.h"


#define IDT_HANDLERS 256
#define IDT_SIZE 256 * 8 - 1

#define CODE_SEGMENT 0x8


#define configure_idt_entry(idt_entry,off,seg)\
    idtEntries[idt_entry].off_0_15 = (uint16_t)(((uint32_t)&off & 0x0000ffff));\
    idtEntries[idt_entry].off_16_31 = (uint16_t)((uint32_t)&off >> 16);\
    idtEntries[idt_entry].segment = seg;\
    idtEntries[idt_entry].res = 0x0;\
    idtEntries[idt_entry].P_DPL = 0x8e




    /* IDT Register */
    struct idtReg
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



void set_idt(void)
{


    /* configure IDT entries*/
    configure_idt_entry(0x0,divide_by_zero_exception,CODE_SEGMENT);
    configure_idt_entry(0x6,invalid_opcode,CODE_SEGMENT);
    configure_idt_entry(0x20,pit_handler_init,CODE_SEGMENT);
    configure_idt_entry(0x21,keyboard_handler_init,CODE_SEGMENT);
    configure_idt_entry(0x26,floppy_interrupt,CODE_SEGMENT);


    
    struct idtReg idtr = {
        IDT_SIZE,
        (uint32_t)&idtEntries
    };
    
    /* loading IDT Register */
    asm("sti");
    asm("lidt %0" : : "m"(idtr));



}