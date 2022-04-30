
#pragma once

#include <stdint.h>
//#include <chip8/chip8_opcodes.hpp>

#define VGA_SCREEN_RESOLUTION 4480
#define VGA_TEXT_MEMORY 0xb8000

class Chip8
{

    
    union ram
    {

        
        struct
        {
            uint8_t chip8_interpreter_space[0x200];
            
            uint8_t program_space[0xD00];
            


            uint8_t display_refresh_buffer[0x100];

        };

        uint8_t ram[0x1000];

    };
    
    uint8_t ram[0x1000] = {0x0};

  
    uint8_t v[0x10];
    uint16_t i;

    uint8_t screen[28][80];

    public:

    void execute();
    void execute_aa(); 
    void screen_clear();
    void ret();
    void jump();
    void execute_subroutine_aa();
    void instruction_skip_if_e_imm();
    void instruction_skip_if_ne_imm();
    void instruction_skip_if_e_reg();
    void mov_imm();
    void add_imm();
    void mov();
    void or_reg();
    void and_reg();
    void xor_reg();
    void add_reg_with_carry();
    void sub_reg_with_borrow();
    void shift_vy_in_vx();

};
    
   


