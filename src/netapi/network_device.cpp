
#include <stdint.h>
#include <netapi/network_device.hpp>

extern "C"
{
    uint8_t* mac_get(void)
    {
        return NetworkDevice::mac_get(&NetworkSubsystem);
    }

    void send_packet(uint32_t address, uint16_t length)
    {
        return NetworkDevice::packet_send(address, length, &NetworkSubsystem);
    }

    uint8_t* receive_packet(void)
    {
        return NetworkDevice::packet_receive(&NetworkSubsystem);
    }

}
