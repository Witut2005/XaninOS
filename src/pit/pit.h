
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <devices/APIC/apic_registers.h>
#include <libc/hal.h>
#include <IDT/idt.h>

#define PIT_CHANNEL0 0x40
#define PIT_CHANNEL1 0x41
#define PIT_CHANNEL2 0x42
#define PIT_MODE_COMMAND_REGISTER 0x43

#define PIT_BASE_FREQUENCY 1193182

#define PIC1_DATA_REG 0x21   


//https://www.youtube.com/watch?v=aK4paXV1XfM <-- USEFUL

extern float pit_time;

extern void pit_handler_init(void);
extern void set_pit_divisor(uint16_t divisor_value);
extern void pit_tick(uint32_t frequency);
extern void pit_handler(void);
extern void set_pit(uint8_t vector);


