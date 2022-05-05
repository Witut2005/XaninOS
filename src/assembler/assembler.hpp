

#pragma once

#include <stdint.h>

typedef struct 
{
    char reg[10];
    uint8_t reg_code;
}asm_reg_t;

extern "C" uint32_t strcmp(char* a, char* b);

asm_reg_t registers[]
{
    {"ax", 0x0}, //0
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
    {"di", 0x7}, //15

    {"[bx+si]", 0x0},
    {"[bx+di]", 0x1},
    {"[bp+si]", 0x2},
    {"[bp+di]", 0x3},
    {"[si]", 0x4},
    {"[di]", 0x5},
    {"DONT_USE", 0x6},
    {"[bx]"}

};

typedef struct asm_intruction
{
    char instruction_name[4];
    uint8_t opcode;
    uint8_t operands;
};



static const asm_intruction opcodes[20] = {

    {"add", 0x2, 2},
    {"nop", 0x90, 2}


};

uint8_t machine_code[8];

static uint8_t operands_get(uint8_t opcode, char* dest, char* source)
{

    uint8_t ret = 0;

    for(int i = 0; i < sizeof(registers) / sizeof(registers[0]); i++)
    {
        if(strcmp(dest, registers[i].reg))
            if(i > 15)
                ret += (registers[i].reg_code << 3) + (1 << 6);
            else    
                ret += (registers[i].reg_code << 3);

    }

    for(int i = 0; i < sizeof(registers) / sizeof(registers[0]); i++)
    {
        if(strcmp(source, registers[i].reg))
            if(i > 15)
                ret += (registers[i].reg_code) + (1 << 6);
            else 
                ret += (registers[i].reg_code);

    }


    machine_code[0] = opcode;
    machine_code[1] = ret;
    


    return 2;

}

static void assemble()
{

}

