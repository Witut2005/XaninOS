
#include <lib/libc/hal.h>
#include <lib/libc/time.h>

void beep(int hz)
{
    
 	uint32_t Div;
 	uint8_t tmp;
 
        //Set the PIT to the desired frequency
 	Div = 1193180 / hz;
 	outbIO(0x43, 0xb6);
 	outbIO(0x42, (uint8_t) (Div) );
 	outbIO(0x42, (uint8_t) (Div >> 8));

    sleep(10);

    tmp = inbIO(0x61);
    if(tmp != (tmp | 3))
        outbIO(0x61, tmp | 3);
}