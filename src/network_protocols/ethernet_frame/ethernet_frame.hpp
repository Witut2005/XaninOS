
#include <stdint.h>
#include <netapi/network_device.h>
#include <network_protocols/ethernet_frame/ethernet_frame.h>
#include <libcpp/map.h>
#include <netapi/objects/mac.hpp>
#include <netapi/objects/response.h>

class EthernetFrameInterface
{
    private:

    public:
    static void send(const uint8_t* mac_destination, const uint8_t* mac_source, uint16_t protocol, const uint8_t* buffer, uint16_t lenght, NetworkResponse* Response);
    static void receive(uint8_t* buffer);
};