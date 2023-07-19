
#pragma once

#include <stdint.h>

struct Registers
{
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t eip;
    uint16_t cs; 
    uint16_t ds;
    uint16_t es;
    uint16_t fs;
    uint16_t gs;
    uint16_t ss;
};

typedef struct Registers Registers;

#ifdef __cplusplus
extern "C" { 
#endif

extern void __asm_registers_values_get(Registers* RegistersStruct);

#ifdef __cplusplus
}
#endif