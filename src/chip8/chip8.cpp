


#include <chip8/chip8.hpp>


void Chip8::execute()
{

}

void Chip8::execute_aa()
{

}

void Chip8::screen_clear()
{
    uint16_t* screen_cleaner = (uint16_t*) VGA_TEXT_MEMORY;

    for(int i = 0; i < VGA_TEXT_MEMORY + VGA_SCREEN_RESOLUTION; i++)
        screen_cleaner = 0x0;

}

void Chip8::ret()
{

}

void Chip8::jump()
{

}

void Chip8::execute_subroutine_aa()
{

}

void Chip8::instruction_skip_if_e_imm()
{

}

void Chip8::instruction_skip_if_ne_imm()
{

}

void Chip8::instruction_skip_if_e_reg()
{

}

void Chip8::mov_imm()
{

}

void Chip8::add_imm()
{

}

void Chip8::mov()
{

}


void Chip8::or_reg()
{

}

void Chip8::and_reg()
{

}

void Chip8::xor_reg()
{

}

void Chip8::add_reg_with_carry()
{

}

void Chip8::sub_reg_with_borrow()
{

}

void Chip8::shift_vy_in_vx()
{

}
