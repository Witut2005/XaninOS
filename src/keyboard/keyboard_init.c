

#pragma once

#define KEYBOARD_TEST_FAILURE 0xfc
#define KEYBOARD_TEST_SUCCESS 0x55

#define KEYBOARD_DISABLE 0xAD
#define KEYBOARD_ENABLE 0xAE

#include <libc/hal.h>
#include <handlers/handlers.h>


uint8_t keyboard_self_test()
{
    outbIO(0x64,0xaa);
    return inbIO(0x60);
}

void keyboard_reset(void)
{
    outbIO(0x64, KEYBOARD_DISABLE);             // KEYBOARD OFF
    for(int i = 0; i < 10; i++)
        io_wait();
	outbIO(0x64, KEYBOARD_ENABLE);             // KEYBOARD ON
}

uint8_t keyboard_init(void)
{

    uint8_t KEYBOARD_TEST_STATUS = keyboard_self_test();

	outbIO(0x64, KEYBOARD_DISABLE);             // KEYBOARD OFF
    for(int i = 0; i < 10; i++)
        io_wait();
	outbIO(0x64, KEYBOARD_ENABLE);             // KEYBOARD ON
    outbIO(PIC1_DATA_REG, 0xFD);    // 11111101 <-- irq1 ON

    if(KEYBOARD_TEST_STATUS == KEYBOARD_TEST_FAILURE)
    {
        xprintf("keyboard self test failed. Halting execution\n");
        interrupt_disable();
        asm("hlt");
    }

    

    return KEYBOARD_TEST_STATUS;

}
