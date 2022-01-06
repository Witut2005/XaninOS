
#pragma once

#include "../lib/hal.h"
#include <stdint.h>

#define PIT_CHANNEL0 0x40
#define PIT_CHANNEL1 0x41
#define PIT_CHANNEL2 0x42
#define PIT_MODE_COMMAND_REGISTER 0x43

#define PIT_FREQUENCY 1193182

#define PIC1_DATA_REG 0x21   

uint16_t divisor;

void setDivisor(uint16_t newDivisorVal)
{
    if(newDivisorVal < 250)
        newDivisorVal = 250;

    divisor = newDivisorVal;

    outbIO(PIT_CHANNEL0,(uint8_t)(divisor & 0x00ff));
    outbIO(PIT_CHANNEL0,(uint8_t)((divisor & 0x00ff) >> 8));

}




void setPit()
{
    outbIO(PIC1_DATA_REG, 0xFC); // pit irq on
    outbIO(PIT_MODE_COMMAND_REGISTER,0x30);
}
