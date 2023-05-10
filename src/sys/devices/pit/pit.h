
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <sys/devices/apic/apic_registers.h>
#include <lib/libc/hal.h>
#include <sys/interrupts/idt/idt.h>

#define PIT_CHANNEL0 0x40
#define PIT_CHANNEL1 0x41
#define PIT_CHANNEL2 0x42
#define PIT_MODE_COMMAND_REGISTER 0x43

#define PIT_BASE_FREQUENCY 1193182

#define PIC1_DATA_REG 0x21   


//https://www.youtube.com/watch?v=aK4paXV1XfM <-- USEFUL

extern float pit_time;
extern float pit_interval_time;

extern void pit_handler_init(void);
extern void set_pit_divisor(uint16_t divisor_value);
extern void pit_tick(void);
extern void pit_handler(void);
extern void set_pit(uint8_t vector);


