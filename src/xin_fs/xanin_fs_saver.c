

#include <stdint.h>

uint16_t idt_16[3] = {0x3ff, 0x0, 0x0};

void xanin_saver(void)
{

    asm("cli");
    asm("mov eax, 0x28");
    asm("mov ds, eax");
    asm("mov fs, eax");
    asm("mov gs, eax");
    asm("mov ss, eax");
    asm("mov es, eax");
    asm("lidt %0" : : "m"(idt_16));
    asm("sti");
    asm("mov eax, cr0");
    asm("xor eax, 0x1"); 
    asm("mov cr0,eax");
    //asm("jmp $");
    asm("jmp 0x0:0x7c02");

}

