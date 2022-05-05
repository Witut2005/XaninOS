

#pragma once

#include <stdint.h>

typedef struct 
{
    char reg[5];
    uint8_t reg_code;
}asm_reg_t;

asm_reg_t registers[]
{
    {"ax", 0x0},
    {"al", 0x0},
    {"ah", 0x4},

    {"cx", 0x1},
    {"cl", 0x1},
    {"ch", 0x5},

    {"dx", 0x2},
    {"dl", 0x2},
    {"dh", 0x6},

    {"bx", 0x3},
    {"bl", 0x3},
    {"bh", 0x7},
    
    {"sp", 0x4},
    {"bp", 0x5},
    {"si", 0x6},
    {"di", 0x7} 
    
};

typedef struct asm_intruction
{
    char instruction_name[4];
    uint8_t opcode;
    uint8_t operands;
};



static const asm_intruction opcodes[20] = {

    {"nop", 0x90, 2},
    {"add", 0x2, 2}


};

static uint8_t operands_get()
{

    

}



