
#include "./keyboard.hpp"

using namespace Device;

Keyboard& Keyboard::the(void)
{
    return s_instance;
}

bool Keyboard::init(interrupt_vector_t vector)
{

}
void Keyboard::reset(void)
{

}

bool Keyboard::test(void)
{

}

void Keyboard::handle(void)
{

}

void Keyboard::write(Registers) const
{

}

uint8_t Keyboard::read(Registers) const
{

}

Keyboard Keyboard::s_instance;