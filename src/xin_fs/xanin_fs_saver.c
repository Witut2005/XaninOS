

#include <stdint.h>

uint16_t idt_16[3] = {0x3ff, 0x0, 0x0};

void xanin_saver_c(void)
{
    interrupt_disable();
    asm("lidt %0" :: "m"(idt_16));
}

