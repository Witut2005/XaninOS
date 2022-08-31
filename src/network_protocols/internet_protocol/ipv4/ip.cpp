
#include <network_protocols/internet_protocol/ipv4/ip.hpp>
#include <libcpp/cstdlib.h>

uint32_t InternetProtocolInterface::create_ip_address(uint8_t ip_address[4])
{

    uint32_t tmp = *(uint32_t*)ip_address;
    return tmp;

}

void InternetProtocolInterface::ip4_packet_send(uint32_t dest_ip, uint32_t src_ip, uint8_t protocol, uint16_t packet_size, uint8_t* protocol_header)
{

    Ipv4Header* IpHeader = (Ipv4Header*)malloc(sizeof(Ipv4Header));
    IpHeader->version = IPV4_HEADER_VERSION;
    IpHeader->ihl = 0x5;
    IpHeader->packet_size = packet_size;
    IpHeader->flags = 0x0;
    IpHeader->time_to_live = 0xFF;
    IpHeader->protocol = protocol;
    IpHeader->checksum = 0x0; // oj nie wiem czy dobrze
    IpHeader->source_ip_address = src_ip;
    IpHeader->destination_ip_address= dest_ip;




    switch (protocol)
    {
        case USER_DATAGRAM_PROTOCOL: 
        {
            UdpHeader* Udp = (UdpHeader*)protocol_header;

            break;
        }
    }
}

extern "C"
{
    uint32_t create_ip_address(uint8_t ip_address[4])
    {
        InternetProtocolInterface* InternetProtocolSubsystem = (InternetProtocolInterface*)malloc(sizeof(InternetProtocolInterface));
        uint32_t tmp = InternetProtocolSubsystem->create_ip_address(ip_address);
        free(InternetProtocolSubsystem);
        return tmp;
    }
}