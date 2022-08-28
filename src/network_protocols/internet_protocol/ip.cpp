
#include <network_protocols/internet_protocol/ip.hpp>
#include <libcpp/cstdlib.h>

uint32_t InternetProtocolInterface::create_ip_address(uint8_t ip_address[4])
{

    uint32_t tmp = *(uint32_t*)ip_address;
    return tmp;

}

extern "C"
{
    uint32_t create_ip_address(uint8_t ip_address[4])
    {
        InternetProtocolInterface* InternetProtocolSubsystem = (InternetProtocolInterface*)malloc(sizeof(InternetProtocolInterface));
        return InternetProtocolSubsystem->create_ip_address(ip_address);
    }
}