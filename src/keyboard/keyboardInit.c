
#define KEYBOARD_TEST_FAILURE 0xfc
#define KEYBOARD_TEST_SUCCESS 0x55


#include "../lib/hal.h" 


uint8_t keyboardSelfTest()
{
    outbIO(0x64,0xaa);
    return inbIO(0x60);
}


uint8_t kbInit() 
{

    uint8_t KEYBOARD_TEST_STATUS = keyboardSelfTest();

	outbIO(0x64, 0xAE);             // KEYBOARD ON
    outbIO(PIC1_DATA_REG, 0xFD);    // 11111101 <-- irq1 ON 

    if(KEYBOARD_TEST_STATUS == KEYBOARD_TEST_FAILURE) 
    {
        sprint(red,white,"keyboard self test failed. Halting execution\n");
        asm("cli");
        asm("hlt");
    }

    else if(KEYBOARD_TEST_STATUS == KEYBOARD_TEST_SUCCESS)
        sprint(black,white,"keyboard self test passed :))\n");

    
    return KEYBOARD_TEST_STATUS;

}
