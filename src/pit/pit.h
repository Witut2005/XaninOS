
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

static inline void set_pit_divisor(uint16_t divisor_value)
{

    if(divisor_value < 250)
        divisor_value = 250;

    outbIO(PIT_CHANNEL0,(uint8_t)(divisor_value & 0x00ff));
    outbIO(PIT_CHANNEL0,(uint8_t)((divisor_value & 0xff00) >> 8));

}


static inline void set_pit()
{

    interrupt_disable();
    // outbIO(PIC1_DATA_REG, 0xFC); // pit irq on
    // outbIO(PIT_MODE_COMMAND_REGISTER,0x30);
    set_pit_divisor(0x8000);
    interrupt_enable();
    INTERRUPT_REGISTER(0x22, pit_handler_init);
}


static inline void pit_tick(uint32_t frequency)
{
    pit_time += 1 / (float)(PIT_BASE_FREQUENCY / frequency);
    eoi_send();
    // return pit_time;
}




