#pragma once

#include <stdint.h>

struct 
{

    uint32_t int31_0;
    uint32_t int32_63;

    void(*appHandler)(void);

}signal;

#define int31_0 signal.int31_0
#define int32_63 signal.int32_63

#define nullptr 0x0


#define IRQ1_ON(x) {signal.appHandler = &x;int32_63 |= 0x2;}
#define IRQ1_OFF() {int32_63 ^= 0x2;signal.appHandler = nullptr;}

#define KEYBOARD_SIG_ON(x) {signal.appHandler = &x;int32_63 |= 0x2;}
#define KEYBOARD_SIG_OFF() {int32_63 ^= 0x2;signal.appHandler = nullptr;}