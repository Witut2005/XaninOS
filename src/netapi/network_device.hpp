
#include <stdint.h>

class NetworkDevice 
{
    public:
    virtual void send_packet(uint32_t buffer, uint16_t length) = 0;
    virtual uint8_t* receive_packet(void) = 0;
};