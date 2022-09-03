
#include <network_protocols/internet_protocol/ipv4/ip.hpp>
#include <libcpp/cstdlib.h>
#include <network_protocols/ethernet_frame/ethernet_frame.hpp>
#include <network_protocols/arp/arp.h>
#include <libcpp/endian.h>
#include <libcpp/cmemory.h>

#define ETHERNET_TYPE_IPV4 0x800
#define IPV4_HEADER_SIZE 20

uint32_t InternetProtocolInterface::create_ip_address(uint8_t ip_address[4])
{

    uint32_t tmp = *(uint32_t*)ip_address;
    return tmp;

}

void InternetProtocolInterface::ip4_packet_send(uint32_t dest_ip, uint32_t src_ip, uint8_t protocol, uint16_t packet_size, uint8_t* data)
{

    Ipv4Header* IpHeader = (Ipv4Header*)malloc(sizeof(1518));
    IpHeader->version = IPV4_HEADER_VERSION;
    IpHeader->ihl = 0x5;
    IpHeader->packet_size = endian_switch(static_cast<uint16_t>(packet_size + IPV4_HEADER_SIZE));
    IpHeader->flags = 0x0;
    IpHeader->time_to_live = 0xFF;
    IpHeader->protocol = protocol;
    IpHeader->identification = 0x0;
    IpHeader->checksum = packet_size + IpHeader->identification; // oj nie wiem czy dobrze
    IpHeader->source_ip_address = src_ip;
    IpHeader->destination_ip_address= dest_ip;




    switch (protocol)
    {
        case USER_DATAGRAM_PROTOCOL: 
        {
            uint8_t* protocol_header = (uint8_t*)IpHeader;
            protocol_header = protocol_header + IPV4_HEADER_SIZE;
            UdpHeader* UdpPacket = (UdpHeader*)protocol_header;

            memcpy((uint8_t*)UdpPacket, data, 8);
            uint8_t* packet_data = (uint8_t*)UdpPacket + sizeof(UdpHeader);
            memcpy(packet_data, data, 1518 - sizeof(Ipv4Header) - sizeof(UdpHeader));

            EthernetFrameInterface* NewEthernetFrame = (EthernetFrameInterface*)malloc(sizeof(EthernetFrameInterface));
            NewEthernetFrame->send(mac_get_from_ip((uint8_t*)&dest_ip) == nullptr ? mac_get_from_ip((uint8_t*)&dest_ip) : mac_broadcast, netapi_mac_get(), ETHERNET_TYPE_IPV4, (uint8_t*)IpHeader, packet_size);

            free(NewEthernetFrame);
            break;
        }
    }

    free(IpHeader);

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

    void ipv4_packet_send(uint32_t dest_ip, uint32_t src_ip, uint8_t protocol, uint16_t packet_size, uint8_t* data)
    {
        InternetProtocolInterface* InternetProtocolPacket = (InternetProtocolInterface*)malloc(sizeof(InternetProtocolInterface));
        InternetProtocolPacket->ip4_packet_send(dest_ip, src_ip, protocol, packet_size, data);
        free(InternetProtocolPacket);
    }

}