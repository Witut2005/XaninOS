
#include <stdint.h>
#include <lib/libcpp/container/map.hpp>
#include <sys/net/netapi/network_device.h>
#include <sys/net/netapi/objects/mac.hpp>
#include <sys/net/netapi/objects/response.h>
#include <sys/net/network_protocols/ethernet_frame/ethernet_frame.h>

class EthernetFrameInterface
{
    private:

    public:
    static void send(const uint8_t* mac_destination, const uint8_t* mac_source, uint16_t protocol, const uint8_t* buffer, uint16_t lenght, NetworkResponse* Response);
    static void receive(uint8_t* buffer);
};