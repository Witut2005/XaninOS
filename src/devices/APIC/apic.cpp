#include <devices/APIC/apic.hpp>


bool AdvancedProgrammableInterruptContoller::enable()
{
    uint32_t* tmp = (uint32_t*)APIC_SPURIOUS_INTERRUPT_VECTOR_REGISTER;
    *tmp |= 1 << 8;

    return true;
}

bool AdvancedProgrammableInterruptContoller::disable()
{
    uint32_t* tmp = (uint32_t*)APIC_SPURIOUS_INTERRUPT_VECTOR_REGISTER;
    if(*tmp & (1 << 8))
        *tmp ^= 1 << 8;

    return false;        
}


uint32_t AdvancedProgrammableInterruptContoller::read(uint32_t address)
{
    uint32_t* tmp = (uint32_t*)address;
    return *tmp;
}

void AdvancedProgrammableInterruptContoller::write(uint32_t address, uint32_t value)
{
    uint32_t* tmp = (uint32_t*)address;
    *tmp = value;
}