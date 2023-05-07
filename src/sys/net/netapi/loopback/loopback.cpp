
#include <sys/net/netapi/loopback/loopback.h>

bool LoopbackNetworkDevice::is_device_present(void) const
{
    return true;
}

uint8_t* LoopbackNetworkDevice::mac_get(void) 
{
    return (uint8_t*)"\0\0\0\0\0";
}

const char* LoopbackNetworkDevice::name_get(void) const
{
    return "loopback";
}

uint32_t LoopbackNetworkDevice::ip_get(void) const
{
    return (127 << 24) | 1;
}

void LoopbackNetworkDevice::packet_send(uint8_t* buffer, uint16_t length)
{
    NetworkDevice* NetworkCard = netapi_find_available_device();
    NetworkCard->packet_send(buffer, length);
}

LoopbackNetworkDevice XaninNetworkLoopback;
