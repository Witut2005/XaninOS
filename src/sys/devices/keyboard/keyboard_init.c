
#pragma once

#include <lib/libc/hal.h>
#include <sys/interrupts/handlers/handlers.h>
#include <sys/interrupts/idt/idt.h>
#include <sys/devices/com/com.h>

#define KEYBOARD_TEST_FAILURE 0xfc
#define KEYBOARD_TEST_SUCCESS 0x55

#define KEYBOARD_DISABLE 0xAD
#define KEYBOARD_ENABLE 0xAE

extern void keyboard_handler_init(void);

uint8_t keyboard_self_test()
{
    outbIO(0x64, 0xaa);
    return inbIO(0x60);
}

void keyboard_reset(void)
{
    outbIO(0x64, KEYBOARD_DISABLE); // KEYBOARD OFF

    for (int i = 0; i < 10; i++)
        io_wait();

    outbIO(0x64, KEYBOARD_ENABLE); // KEYBOARD ON
}

uint8_t keyboard_init(uint8_t vector)
{

    uint8_t keyboard_status = keyboard_self_test();

    if (keyboard_status == KEYBOARD_TEST_FAILURE)
    {
        dbg_error(DEBUG_LABEL_KERNEL_DEVICE, "keyboard self test failed. Halting execution");
        cpu_halt();
    }

    INTERRUPT_REGISTER(vector, keyboard_handler_init);
    dbg_info(DEBUG_LABEL_IRQ, "Keyboard device installed");
    return keyboard_status;
}
