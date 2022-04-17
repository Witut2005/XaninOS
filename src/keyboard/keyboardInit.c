

#pragma once

#define KEYBOARD_TEST_FAILURE 0xfc
#define KEYBOARD_TEST_SUCCESS 0x55

#include <lib/hal.h>


uint8_t keyboard_self_test()
{
    outbIO(0x64,0xaa);
    return inbIO(0x60);
}


uint8_t keyboard_init()
{

    uint8_t KEYBOARD_TEST_STATUS = keyboard_self_test();

	outbIO(0x64, 0xAE);             // KEYBOARD ON
    outbIO(PIC1_DATA_REG, 0xFD);    // 11111101 <-- irq1 ON

    if(KEYBOARD_TEST_STATUS == KEYBOARD_TEST_FAILURE)
    {
        xprintf("keyboard self test failed. Halting execution\n");
        asm("cli");
        asm("hlt");
    }

    

    return KEYBOARD_TEST_STATUS;

}
