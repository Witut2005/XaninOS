
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
    uint8_t ret;

    asm("in al,dx" :: "d" (port));
    
    asm("in al, %1;"
        "mov %0, al;"
        : "=r"(ret)
        : "r"(port)
        :"al"
        );

    return ret;
    

}

uint16_t inwIO(uint16_t port)
{

    uint16_t ret;
    
    asm("in ax, %1;"
        "mov %0, ax;"
        : "=r"(ret)
        : "r"(port)
        :"ax"
        );

    return ret;

}


uint32_t indIO(uint16_t port)
{
    uint32_t ret;
    
    asm("in eax, %1;"
        "mov %0, eax;"
        : "=r"(ret)
        : "r"(port)
        :"eax"
        );
    return ret;

}

void disable_cursor(void)
{
    outbIO(0x3D4, 0x0A);
	outbIO(0x3D5, 0x20);
}


void update_cursor(int x, int y)
{
	uint16_t pos = y * 90 + x;
 
	outbIO(0x3D4, 0x0F);
	outbIO(0x3D5, (uint8_t) (pos & 0xFF));
	outbIO(0x3D4, 0x0E);
	outbIO(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
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



