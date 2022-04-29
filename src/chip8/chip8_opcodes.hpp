
#pragma once

#include <chip8/chip8.hpp>


typedef void(*chip8_opcode_t)(void);

//n = not
//aa = at_address
//e = equal


namespace chip8
{

chip8_opcode_t opcode[20] = {

    execute_aa,
    screen_clear,
    ret,
    jump,
    
    execute_subroutine_aa,
    
    instruction_skip_if_e_imm,
    instruction_skip_if_ne_imm,
    
    instruction_skip_if_e_reg,
   
    mov_imm,
    add_imm,

    mov,
    
    or_reg,
    and_reg,
    xor_reg,

    add_reg_with_carry,
    sub_reg_with_borrow,

    shift_vy_in_vx //8XY6



};

};
