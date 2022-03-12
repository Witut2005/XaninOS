
#pragma once

#include <stdint.h>
#include <lib/hal.h>

#define PIT_CHANNEL0 0x40
#define PIT_CHANNEL1 0x41
#define PIT_CHANNEL2 0x42
#define PIT_MODE_COMMAND_REGISTER 0x43

#define PIT_FREQUENCY 1193182

#define PIC1_DATA_REG 0x21   


uint16_t divisor = 0xFFFF;
bool pitActive = false; 
volatile uint32_t pit_current_time = 0x0;


void set_pit_divisor(uint16_t divisor_value)
{
    if(divisor_value < 250)
        divisor_value = 250;

    divisor = divisor_value;

    outbIO(PIT_CHANNEL0,(uint8_t)(divisor & 0x00ff));
    
    for(int i = 0; i < 0xFFFF; i++)
        asm("nop");

    outbIO(PIT_CHANNEL0,(uint8_t)((divisor & 0x00ff) >> 8));

}




void set_pit()
{
    set_pit_divisor(0xFFFF);
    outbIO(PIC1_DATA_REG, 0xFC); // pit irq on
    outbIO(PIT_MODE_COMMAND_REGISTER,0x30);
}


void sleep(uint32_t miliseconds)
{

    volatile uint32_t start_time = pit_current_time + miliseconds;

    while(pit_current_time < start_time)
        asm("nop");

}


