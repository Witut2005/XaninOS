
#pragma once

#include <stddef.h>
#include <stdint.h>

extern bool apic_enable(void);
extern bool apic_disable(void);
extern void apic_write(uint32_t address, uint32_t value);
extern uint32_t apic_read(uint32_t address);
extern void apic_set_spurious_vector_number(uint32_t vector_number);
extern void apic_lvt_set(void);