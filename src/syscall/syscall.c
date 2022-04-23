
#include <stdint.h>
#include <lib/stdiox.h>

void syscall()
{
    uint32_t call_selector;


    asm("push eax");
    asm("pop [ecx]" :: "ecx"(&call_selector));

    //screen_clear();
    xprintf("eax: 0x%x\n", call_selector);


    asm("cli");
    asm("hlt");

    switch(call_selector)
    {
        case 1:

        screen_clear();
        xprintf("piwko");
        asm("cli");
        asm("hlt");
    }

}