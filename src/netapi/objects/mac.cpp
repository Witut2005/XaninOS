

#include <netapi/network_device.h>
#include <netapi/objects/mac.hpp>
#include <netapi/loopback/loopback.h>
#include <libc/stdlibx.h>

namespace net
{

bool is_system_mac(uint8_t* mac)
{
    net::MacAddress LoopbackMacAddress(XaninNetworkLoopback.mac_get());
    net::MacAddress XaninMacAddress(netapi_mac_get(xanin_ip_get()));
    net::MacAddress GivenMac(mac);

    return ((GivenMac == LoopbackMacAddress) | (GivenMac == XaninMacAddress));
}


bool is_loopback_mac(uint8_t* mac)
{
    return net::MacAddress(mac) == net::MacAddress(XaninNetworkLoopback.mac_get());
}
}