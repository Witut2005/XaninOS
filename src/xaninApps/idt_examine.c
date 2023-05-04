
#include <IDT/idt.h>
#include <libc/stdlibx.h>

extern void divide_by_zero_exception(void);
extern void keyboard_handler_init(void);
extern irq_handler interrupt_handlers[0x100];

int idt_examine(void)
{

    // interrupt_handlers[0] = divide_by_zero_exception;

    // xprintf("device by zero:");
    // xprintf("0x%x ", divide_by_zero_exception);
    // xprintf("0x%x\n", interrupt_handlers[0]);

    // xprintf("0x%x ", keyboard_handler_init);
    // xprintf("0x%x\n", interrupt_handlers[0x21]);

    // // while(inputg().scan_code != ENTER);
    return XANIN_OK;
}