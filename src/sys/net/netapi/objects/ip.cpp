
#include <sys/net/netapi/network_device.h>
#include <sys/net/netapi/objects/ip.h>
#include <sys/net/netapi/objects/mac.hpp>
#include <sys/net/netapi/loopback/loopback.h>
#include <lib/libc/stdlibx.h>

namespace net
{ 

bool is_system_ip(uint32_t ip) // ALL IN LITTLE ENDIAN
{
    if((ip == xanin_ip_get()) || (ip == XaninNetworkLoopback.ip_get()))
        return true;
    return false;
}

}