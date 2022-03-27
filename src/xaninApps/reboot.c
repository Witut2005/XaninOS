
#pragma once

#include <stdint.h>

void reboot(void)
{

    uint16_t tmp[3] = {0,0,0};

    asm("lidt %0" :: "m"(tmp));
    asm("mov ebx, 0x0");
    asm("div ebx");

}