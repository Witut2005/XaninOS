

#include <stdint.h>
#include <libc/hal.h>
#include <devices/APIC/apic_registers.h>
#include <stdbool.h>

#define PIT_CHANNEL0 0x40
#define PIT_CHANNEL1 0x41
#define PIT_CHANNEL2 0x42
#define PIT_MODE_COMMAND_REGISTER 0x43

#define PIT_BASE_FREQUENCY 1193182

#define PIC1_DATA_REG 0x21   


//https://www.youtube.com/watch?v=aK4paXV1XfM <-- USEFUL
