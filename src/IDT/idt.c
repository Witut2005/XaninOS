
#include <stdint.h>
#include <handlers/handlers.c>
#include <keyboard/key_map.h>
#include <syscall/posix/posix.c>
#include <devices/MOUSE/mouse.h>
#include <IDT/idt.h>

//extern void mouse_handler(void);

extern void mouse_handler_init(void);
extern void i8254x_interrupt_handler_entry(void);
extern void syscall_entry(void);

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

/* configure interrupt descriptor table entry */
#define configure_idt_entry_from_array(idt_entry,off,seg)\
    idtEntries[idt_entry].off_0_15 = (uint16_t)(((uint32_t)off & 0x0000ffff));\
    idtEntries[idt_entry].off_16_31 = (uint16_t)((uint32_t)off >> 16);\
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

irq_handler interrupt_handlers[0x100];

void set_idt(void)

{
    asm("cli");

    /* configure IDT entries*/
    configure_idt_entry_from_array(0x0, interrupt_handlers[0], CODE_SEGMENT);
    configure_idt_entry_from_array(0x1, interrupt_handlers[1], CODE_SEGMENT);
    configure_idt_entry_from_array(0x2, interrupt_handlers[2],CODE_SEGMENT);
    configure_idt_entry_from_array(0x3, interrupt_handlers[3], CODE_SEGMENT);
    configure_idt_entry_from_array(0x4, interrupt_handlers[4], CODE_SEGMENT);
    configure_idt_entry_from_array(0x5, interrupt_handlers[5],CODE_SEGMENT);
    configure_idt_entry_from_array(0x6, interrupt_handlers[6],CODE_SEGMENT);
    configure_idt_entry_from_array(0x7, interrupt_handlers[7],CODE_SEGMENT);
    configure_idt_entry_from_array(0x8, interrupt_handlers[8],CODE_SEGMENT);
    configure_idt_entry_from_array(0x9, interrupt_handlers[9],CODE_SEGMENT);
    configure_idt_entry_from_array(0xa, interrupt_handlers[0xa],CODE_SEGMENT);
    configure_idt_entry_from_array(0xb, interrupt_handlers[0xb],CODE_SEGMENT);
    configure_idt_entry_from_array(0xc, interrupt_handlers[0xc],CODE_SEGMENT);
    configure_idt_entry_from_array(13, interrupt_handlers[13], CODE_SEGMENT);
    configure_idt_entry_from_array(0xe, interrupt_handlers[14], CODE_SEGMENT);
    configure_idt_entry_from_array(0xf, interrupt_handlers[15], CODE_SEGMENT);
    configure_idt_entry_from_array(17, interrupt_handlers[17], CODE_SEGMENT);
    configure_idt_entry_from_array(18, interrupt_handlers[18],CODE_SEGMENT);
    configure_idt_entry_from_array(19, interrupt_handlers[19], CODE_SEGMENT);
    configure_idt_entry_from_array(20, interrupt_handlers[20],CODE_SEGMENT);
    configure_idt_entry_from_array(21, interrupt_handlers[21],CODE_SEGMENT);
    configure_idt_entry_from_array(22, interrupt_handlers[22], CODE_SEGMENT);
    configure_idt_entry_from_array(23, interrupt_handlers[23], CODE_SEGMENT);
    configure_idt_entry_from_array(24, interrupt_handlers[24], CODE_SEGMENT);
    configure_idt_entry_from_array(25, interrupt_handlers[25], CODE_SEGMENT);
    configure_idt_entry_from_array(26, interrupt_handlers[26], CODE_SEGMENT);
    configure_idt_entry_from_array(27, interrupt_handlers[27], CODE_SEGMENT);
    configure_idt_entry_from_array(28, interrupt_handlers[28], CODE_SEGMENT);
    configure_idt_entry_from_array(29, interrupt_handlers[29], CODE_SEGMENT);
    configure_idt_entry_from_array(30, interrupt_handlers[30], CODE_SEGMENT);
    configure_idt_entry_from_array(31, interrupt_handlers[31], CODE_SEGMENT);
    configure_idt_entry_from_array(32, interrupt_handlers[32], CODE_SEGMENT);
    
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
    configure_idt_entry(0x2C, mouse_handler_init, CODE_SEGMENT);
    configure_idt_entry(0x50, elf_correctly_loaded,CODE_SEGMENT);

    configure_idt_entry(0x80, syscall_entry,CODE_SEGMENT);
    configure_idt_entry(0x81, no_handler,CODE_SEGMENT);
    configure_idt_entry(0xFF, reboot_interrupt,CODE_SEGMENT);

    struct idt_register idtr = {
        IDT_SIZE,
        (uint32_t)&idtEntries
    };

    /* load IDT Register with proper struct */
    asm("lidt %0" : : "m"(idtr));
    interrupt_enable();
}

void interrupt_register(uint32_t interrupt_id, irq_handler handler) 
{
    if(interrupt_id >= 0x20)
        return;
    interrupt_handlers[interrupt_id] = handler;
    set_idt();
}

void irq_register(uint32_t irq, irq_handler handler) 
{
    
    if(irq < 0x20)
        return;
    interrupt_handlers[irq] = handler;
    set_idt();
}
