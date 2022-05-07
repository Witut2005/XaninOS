
#pragma once

#include <stdint.h>
#include <headers/macros.h>

void outbIO(uint16_t port,uint8_t al)
{
    asm("outb dx,al" :: "d" (port), "a"(al));
}

void outwIO(uint16_t port,uint16_t ax)
{
    asm("out dx,ax" :: "d" (port), "a"(ax));
}

void outdIO(uint16_t port,uint32_t eax)
{
    asm("out dx,eax" :: "d" (port), "a"(eax));
}


uint8_t inbIO(uint16_t port)
{
    asm("in al,dx" :: "d" (port));

    register uint8_t retVal asm("al");
    return retVal;
}

uint16_t inwIO(uint16_t port)
{
    asm("in ax,dx" :: "d" (port));

    register uint16_t retVal asm("ax");
    return retVal;
}


uint32_t indIO(uint16_t port)
{
    asm("in eax,dx" :: "d" (port));

    register uint32_t retVal asm("eax");
    return retVal;

}

void disable_cursor(void)
{
     outbIO(0x3D4, 0x0A);
	   outbIO(0x3D5, 0x20);
}

void eoi_send(void)
{
    asm("out 0x20, al" :: "a"(0x20));
}

void io_wait(void)
{
    asm("out 0x80, al" :: "a"(0x0));
}

void real_mode_enter(uint16_t segment, uint16_t offset)
{

    /* get segment */
    asm (
        "mov eax, [ebp + 8]\n\t"
        "mov ebx, eax\n\t"
        "and ebx, 0xFFFF"
        );

    /* get offset */
    asm (
        "mov eax, [ebp + 12]\n\t"
        "mov edx, eax\n\t"
        "and edx, 0xFFFF"
        );


    void (*enter16)(void) = (void(*)(void))0x600;
    enter16();
}

void rdmsr(uint32_t msr_id, uint32_t low, uint32_t high)
{
    asm("rdmsr" : "=a"(low), "=d"(high) : "c"(msr_id));
}


void wrmsr(uint32_t msr_id, uint32_t low, uint32_t high)
{
    asm("wrmsr" : "=a"(low), "=d"(high) : "c"(msr_id));
}



