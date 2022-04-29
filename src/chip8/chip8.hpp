
#pragma once

#include <stdint.h>
//#include <chip8/chip8_opcodes.hpp>

class Chip8
{

    uint8_t ram[0xFFF];
  
    uint8_t v[0x10];
    uint16_t i;

    uint8_t screen[28][80];

    void execute(void);


};
    
   


void execute_aa()
{

}


void screen_clear()
{

}


void ret()
{

}

void jump()
{

}

void execute_subroutine_aa()
{

}

void instruction_skip_if_e_imm()
{

}

void instruction_skip_if_ne_imm()
{

}

void instruction_skip_if_e_reg()
{

}

void mov_imm()
{

}




void add_imm()
{

}

void mov()
{

}

void or_reg()
{

}

void and_reg()
{

}


void xor_reg()
{

}


void add_reg_with_carry()
{

}


void sub_reg_with_borrow()
{

}



void shift_vy_in_vx()
{

}
