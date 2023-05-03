

#include <stdint.h>
#include <libc/hal.h>
#include <devices/APIC/apic_registers.h>
#include <stdbool.h>
#include <IDT/idt.h>
#include <pit/pit.h>
#include <libc/stdlibx.h>

#define PIT_CHANNEL0 0x40
#define PIT_CHANNEL1 0x41
#define PIT_CHANNEL2 0x42
#define PIT_MODE_COMMAND_REGISTER 0x43

#define PIT_BASE_FREQUENCY 1193182
#define PIT_XANIN_FREQUENCY 10000
#define PIC1_DATA_REG 0x21   

//https://www.youtube.com/watch?v=aK4paXV1XfM <-- USEFUL

void set_pit_divisor(uint16_t divisor_value)
{

    if(divisor_value < 250)
        divisor_value = 250;

    outbIO(PIT_CHANNEL0, divisor_value & 0x00ff);
    outbIO(PIT_CHANNEL0, (divisor_value & 0xff00) >> 8);

}


void set_pit(uint8_t vector)
{

    interrupt_disable();
    outbIO(PIT_MODE_COMMAND_REGISTER, 0x36); //
    set_pit_divisor(PIT_BASE_FREQUENCY / PIT_XANIN_FREQUENCY); // 10ms
    INTERRUPT_REGISTER(vector, pit_handler_init);
    interrupt_enable();
}


void pit_tick(void)
{
    // pit_time = pit_time + (1 / (float)(PIT_BASE_FREQUENCY / frequency));
    pit_time = pit_time + 0.0001;

    for(int i = 0; i < INTERVALS_MAX; i++)
        do_interval(i);
    
}

void pit_handler(void)
{
    interrupt_disable();
    pit_tick();
    interrupt_enable();
    eoi_send();
}