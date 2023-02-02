
#include <stdint.h>
#include <netapi/network_device.h>
#include <network_protocols/ethernet_frame/ethernet_frame.h>

class EthernetFrameInterface
{
    public:
        void send(const uint8_t* mac_destination, const uint8_t* mac_source, uint16_t protocol, const uint8_t* buffer, uint16_t lenght);
        void receive(uint8_t* buffer);
};