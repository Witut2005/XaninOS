

#pragma once

#include <stdint.h>

char* register_names[]
{
    "eax",
    "ax",
    "ah",
    "al",

    "ecx",
    "cx",
    "ch",
    "cl",

    "edx",
    "dx",
    "dh",
    "dl",

    "ebx",
    "bx",
    "bh",
    "bl",

    "esp",
    "sp",
    "sph",
    "spl",

    "esi",
    "si",
    "sih",
    "sil",


    "edi",
    "di",
    "dih",
    "dil"

    "[eax]",
    "[ax]",
    
    "[ecx]",
    "[cx]",

    "[edx]",
    "[dx]",
    
    "[ebx]",
    "[bx]",

    "[esp]",
    "[sp]",

    "[esi]",
    "[si]",

    "[edi]",
    "[di]"

};

typedef struct asm_intruction
{
    char instruction_name[4];
    uint8_t opcode;
};


static const asm_intruction opcodes[20] = {

    {"nop", 0x90}


};


