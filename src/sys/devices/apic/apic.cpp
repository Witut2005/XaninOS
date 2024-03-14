#include <sys/devices/apic/apic.hpp>


bool AdvancedProgrammableInterruptContoller::enable()
{
    uint32_t* tmp = (uint32_t*)APIC_SPURIOUS_INTERRUPT_VECTOR_REGISTER;
    *tmp |= 1 << 8;

    dbg_info(DEBUG_LABEL_APIC, "Apic enabled");
    return true;
}

bool AdvancedProgrammableInterruptContoller::disable()
{
    uint32_t* tmp = (uint32_t*)APIC_SPURIOUS_INTERRUPT_VECTOR_REGISTER;
    if(*tmp & (1 << 8))
        *tmp ^= 1 << 8;

    dbg_info(DEBUG_LABEL_APIC, "Apic disabled");
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

void AdvancedProgrammableInterruptContoller::set_spurious_vector_number(uint32_t vector)
{
    uint32_t* tmp = (uint32_t*)APIC_SPURIOUS_INTERRUPT_VECTOR_REGISTER;
    *tmp = (*tmp & ~0xFF) | vector; 
}

void AdvancedProgrammableInterruptContoller::lvt_timer_set()
{
    *(uint32_t*)APIC_LVT_REGISTER |= LVT_UNMASK;
}
