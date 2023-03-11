

class NetworkDevice
{
    public:
    virtual void packet_send(uint8_t*, uint16_t) = 0; 
    virtual uint8_t* packet_receive(void) = 0;
};