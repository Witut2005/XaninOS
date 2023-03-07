
#include <IDT/idt.h>
#include <libc/stdlibx.h>

extern void divide_by_zero_exception(void);
extern irq_handler interrupt_handlers[0x100];

int idt_examine(void)
{

    // interrupt_handlers[0] = divide_by_zero_exception;

    xprintf("device by zero:");
    xprintf("0x%x ", divide_by_zero_exception);
    xprintf("0x%x\n", interrupt_handlers[0]);
    while(inputg().scan_code != ENTER);
    return XANIN_OK;
}