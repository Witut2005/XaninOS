

#pragma once

#include <stddef.h>
#include <stdint.h>

extern void ioapic_init(uint32_t base);
extern void ioapic_write(uint8_t offset, uint32_t value);
extern uint32_t ioapic_read(uint8_t offset);
