
#pragma once

#include <chip8/chip8.hpp>


typedef void(Chip8::*chip8_opcode_t)(void);

//n = not
//aa = at_address
//e = equal


chip8_opcode_t opcode[20] = {


    &Chip8::execute_aa,
    &Chip8::screen_clear,
    &Chip8::ret,
    &Chip8::jump,
    
    &Chip8::execute_subroutine_aa,
    
    &Chip8::instruction_skip_if_e_imm,
    &Chip8::instruction_skip_if_ne_imm,
    
    &Chip8::instruction_skip_if_e_reg,
   
    &Chip8::mov_imm,
    &Chip8::add_imm,

    &Chip8::mov,
    
    &Chip8::or_reg,
    &Chip8::and_reg,
    &Chip8::xor_reg,

    &Chip8::add_reg_with_carry,
    &Chip8::sub_reg_with_borrow,

    &Chip8::shift_vy_in_vx //8XY6



};


