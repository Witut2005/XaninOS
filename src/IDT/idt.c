#pragma once

#include <stdint.h>
#include "../lib/hal.h"
#include "../handlers/handlers.c"
#include "../keyboard/keyMap.h"


#define IDT_HANDLERS 256
#define IDT_SIZE 256 * 8 - 1

#define CODE_SEGMENT 0x8


#define confIDT(idt_entry,off,seg)\
    idtEntries[idt_entry].off_0_15 = (uint16_t)(((uint32_t)&off & 0x0000ffff));\
    idtEntries[idt_entry].off_16_31 = (uint16_t)((uint32_t)&off >> 16);\
    idtEntries[idt_entry].segment = seg;\
    idtEntries[idt_entry].res = 0x0;\
    idtEntries[idt_entry].P_DPL = 0x8e


extern void keyboardHandlerInit();

    /* IDT Register */
    struct idtReg
    {
        uint16_t limit;
        uint32_t base;
    }__attribute__((packed));
    
    /* IDT entry structure */
    struct IDT
    {
        uint16_t off_0_15;
        uint16_t segment;
        uint8_t res;
        uint8_t P_DPL;
        uint16_t off_16_31;
    }__attribute__((packed));

    __attribute__((aligned(0x8)))
    typedef struct IDT IDT;
    
    IDT idtEntries[IDT_HANDLERS];



void setIdt(void)
{



    /* configure IDT entries*/
    confIDT(0x6,invalidOpcode,CODE_SEGMENT);
    confIDT(0x0,divError,CODE_SEGMENT);
    confIDT(0x20,pitHandlerInit,CODE_SEGMENT);
    confIDT(0x21,keyboardHandlerInit,CODE_SEGMENT);
    confIDT(0x26,floppyIrq,CODE_SEGMENT);


    
    struct idtReg idtr = {
        IDT_SIZE,
        (uint32_t)&idtEntries
    };
    
    /* loading IDT Register */
    asm("sti");
    asm("lidt %0" : : "m"(idtr));



}