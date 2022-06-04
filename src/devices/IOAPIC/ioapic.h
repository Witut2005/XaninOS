

#pragma once

#include <stddef.h>
#include <stdint.h>

#define APIC_IRQ_BASE 0x20

enum APIC_IOREDTBL_OPTIONS 
{


    PIC_PIT_VECTOR        = 0x20,
    PIC_KEYBOARD_VECTOR   = 0x21,
    
    APIC_VECTOR           = 0,
    APIC_DELIVERY_MODE    = 8,
    APIC_DESTINATION_MODE = 11,
    APIC_INT_PIN_POLARITY = 13,
    APIC_TRIGGER_MODE     = 15,
    APIC_INT_MASK         = 16

};

extern void ioapic_init(uint32_t base);
extern void ioapic_write(uint8_t offset, uint32_t value);
extern uint32_t ioapic_read(uint8_t offset);
extern uint32_t ioapic_id_get(void);
extern void ioapic_ioredtbl_configure(uint32_t lower, uint32_t upper);
