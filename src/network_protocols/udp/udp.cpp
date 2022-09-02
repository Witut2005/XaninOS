
#include <network_protocols/udp/udp.hpp>
#include <libcpp/cstdlib.h>
#include <libcpp/cmemory.h>
#include <network_protocols/internet_protocol/ipv4/ip.hpp>


void UserDatagramProtocolInterface::ipv4_send(uint16_t dest_port, uint16_t src_port, uint16_t packet_size, uint8_t* data)
{
    UdpHeader* UdpPacket = (UdpHeader*)malloc(sizeof(1518));
    memcpy((uint8_t*)(UdpPacket + 1), data, packet_size);
    InternetProtocolInterface* IpPacket = (InternetProtocolInterface*)malloc(sizeof(InternetProtocolInterface));
    // IpPacket->ip4_packet_send()
}