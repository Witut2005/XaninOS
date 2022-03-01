#pragma once

#ifndef HAL_H
#define HAL_H

#include <stdint.h>
#include <headers/macros.h>

void outbIO(uint16_t port,uint8_t al)
{
    asm("outb dx,al" :: "d" (port), "a"(al));
}

void outdwIO(uint16_t port,uint16_t ax)
{
    asm("out dx,ax" :: "d" (port), "a"(ax));
}

void outddIO(uint16_t port,uint32_t eax)
{
    asm("out dx,eax" :: "d" (port), "a"(eax));
}


uint8_t inbIO(uint16_t port)
{
    asm("in al,dx" :: "d" (port));

    register uint8_t retVal asm("al");
    return retVal;
}

uint16_t indwIO(uint16_t port)
{
    asm("in ax,dx" :: "d" (port));

    register uint16_t retVal asm("ax");
    return retVal;
}


uint32_t inddIO(uint16_t port)
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



#endif
