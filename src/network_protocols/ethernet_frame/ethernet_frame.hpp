
#include <stdint.h>
#include <netapi/network_device.hpp>
#include <network_protocols/ethernet_frame/ethernet_frame.h>

class EthernetFrameInterface
{
    public:
        void send(uint8_t* mac_destination, uint8_t* mac_source, uint16_t protocol, uint8_t* buffer, uint16_t lenght);
        EthernetFrame* receive(uint8_t* buffer);
};