
#include <sys/interrupts/idt/idt.h>
#include <lib/libc/stdlibx.h>

extern void divide_by_zero_exception(void);
extern void keyboard_handler_init(void);
extern irq_handler interrupt_handlers[0x100];



int idt_examine(void)
{

    uint8_t bytes[] = {0xba, 0x0, 0x0, 0x0, 0x0, 0xF7, 0xF2};
    uint8_t* tmp = (uint8_t*)NULL;

    memcpy(tmp, bytes, 10);

    interrupt_disable();
    
    uint16_t nicholatte[3] = {0x0, 0x0, 0x0};
    asm("lidt %0" :: "m"(nicholatte));

    // interrupt_handlers[0] = divide_by_zero_exception;
    for(int i = 0; i < 32; i++)
        INTERRUPT_UNREGISTER(0);

    xprintf("device by zero:");
    // xprintf("0x%x ", divide_by_zero_exception);
    xprintf("0x%x\n", interrupt_handlers[0]);
    int x = 0;
    int a = 1 / x;

    xprintf("0x%x ", keyboard_handler_init);
    xprintf("0x%x\n", interrupt_handlers[0x21]);

    while(inputg().scan_code != ENTER);
    return XANIN_OK;
}