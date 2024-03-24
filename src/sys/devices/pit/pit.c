

#include <stdint.h>
#include <lib/libc/hal.h>
#include <sys/devices/apic/apic_registers.h>
#include <stdbool.h>
#include <sys/interrupts/idt/idt.h>
#include <sys/devices/pit/pit.h>
#include <lib/libc/stdlibx.h>
#include <sys/devices/com/com.h>

#define PIT_CHANNEL0 0x40
#define PIT_CHANNEL1 0x41
#define PIT_CHANNEL2 0x42
#define PIT_MODE_COMMAND_REGISTER 0x43

#define PIT_BASE_FREQUENCY 1193182
#define PIT_XANIN_FREQUENCY 10000
#define PIC1_DATA_REG 0x21   

//https://www.youtube.com/watch?v=aK4paXV1XfM <-- USEFUL


void pit_divisor_set(uint16_t divisor_value)
{

    if (divisor_value < 250)
        divisor_value = 250;

    outbIO(PIT_CHANNEL0, divisor_value & 0x00ff);
    outbIO(PIT_CHANNEL0, (divisor_value & 0xff00) >> 8);
}

void pit_init(uint8_t vector)
{
    pit_time = 0;
    EFlags flags;
    eflags_get(&flags);

    interrupt_disable();
    outbIO(PIT_MODE_COMMAND_REGISTER, 0x36);
    pit_divisor_set(PIT_BASE_FREQUENCY / PIT_XANIN_FREQUENCY); // 10ms
    INTERRUPT_REGISTER(vector, pit_handler_init);
    dbg_info(DEBUG_LABEL_KERNEL_DEVICE, "PIT successufly initialized");

    if (flags.intf)
        interrupt_enable();
}

void pit_tick(void)
{
    pit_time = pit_time + 0.0001;

    for (int i = 0; i < INTERVALS_MAX; i++)
        do_interval(i);

}

void pit_handler(void)
{
    interrupt_disable();
    pit_tick();
    interrupt_enable();
    eoi_send();
}