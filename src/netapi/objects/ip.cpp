
#include <netapi/network_device.h>
#include <netapi/objects/ip.h>
#include <netapi/objects/mac.hpp>
#include <netapi/loopback/loopback.h>
#include <libc/stdlibx.h>

namespace net
{ 

bool is_system_ip(uint32_t ip) // ALL IN LITTLE ENDIAN
{
    if((ip == xanin_ip_get()) || (ip == XaninNetworkLoopback.ip_get()))
        return true;
    return false;
}

}