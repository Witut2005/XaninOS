
#include <stddef.h>
#include <stdint.h>


namespace ne2000
{

    namespace page0 
    {

        enum Ne2000Register
        {
            command           = 0x0,
            cldma0            = 0x1,
            cldma1            = 0x2,
            boundary          = 0x3,
            transmit_status   = 0x4,
            collsiion_counter = 0x5,
            fifo              = 0x6,
            isr               = 0x7,
            crda0             = 0x8,
            crda1             = 0x9,
            id0               = 0xa,
            id1               = 0xb,
            receive_status    = 0xc,
            cntr0             = 0xd,
            cntr1             = 0xe,
            cntr2             = 0xf
        };
        
    };


}
