
#include <stdint.h>
#include <lib/stdiox.h>


void stack_frame_test(uint32_t x, uint32_t y)
{


    uint32_t  p1, p2;


    asm (
        "mov eax, [ebp + 8]\n\t"
        "mov %0, eax" :: "m" (p1)
        );

    asm (
        "mov eax, [ebp + 12]\n\t"
        "mov %0, eax" :: "m" (p2)
        );

    
    xprintf("\n\r");

    xprintf("0x%x\n", p1);
    xprintf("0x%x\n", p2);

    while(getscan() != ENTER);

}