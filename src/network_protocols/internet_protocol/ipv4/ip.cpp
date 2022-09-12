
#include <network_protocols/internet_protocol/ipv4/ip.hpp>
#include <network_protocols/icmp/icmp.h>
#include <libcpp/cstdlib.h>
#include <network_protocols/ethernet_frame/ethernet_frame.hpp>
#include <network_protocols/arp/arp.h>
#include <libcpp/endian.h>
#include <libcpp/cmemory.h>
#include <libcpp/cstdio.h>

#define ETHERNET_TYPE_IPV4 0x800
#define IPV4_HEADER_SIZE 20


uint32_t InternetProtocolInterface::create_ip_address(uint8_t ip_address[4])
{

    uint32_t tmp = *(uint32_t*)ip_address;
    return tmp;

}


void InternetProtocolInterface::ip4_packet_send(uint32_t dest_ip, uint32_t src_ip, uint8_t protocol, uint16_t packet_size, uint8_t* data)
{
    uint16_t final_packet_size = packet_size + IPV4_HEADER_SIZE;

    Ipv4Header* IpHeader = (Ipv4Header*)malloc(sizeof(1518));
    IpHeader->version_ihl = 0x5;
    IpHeader->version_ihl |= IPV4_HEADER_VERSION << 4;
    IpHeader->packet_size = endian_switch(static_cast<uint16_t>(packet_size + IPV4_HEADER_SIZE));
    IpHeader->fragment_offset_and_flags = 0x0;
    IpHeader->time_to_live = 0xFF;
    IpHeader->protocol = protocol;
    IpHeader->identification = 0x0;
    IpHeader->checksum = endian_switch(static_cast<uint16_t>(packet_size + IpHeader->identification)); // oj nie wiem czy dobrze
    IpHeader->source_ip_address = src_ip;
    IpHeader->destination_ip_address= dest_ip;


    // xprintf("%d ", *final_packet_size);


    switch (protocol)
    {
        case USER_DATAGRAM_PROTOCOL: 
        {
            uint8_t* protocol_header = (uint8_t*)IpHeader;
            protocol_header = protocol_header + IPV4_HEADER_SIZE;
            UdpHeader* UdpPacket = (UdpHeader*)protocol_header;

            memcpy((uint8_t*)UdpPacket, data, 8);
            uint8_t* packet_data = (uint8_t*)UdpPacket;
            packet_data = packet_data + sizeof(UdpHeader);
            memcpy(packet_data, data, 1518 - sizeof(Ipv4Header) - sizeof(UdpHeader));

            EthernetFrameInterface* NewEthernetFrame = (EthernetFrameInterface*)malloc(sizeof(EthernetFrameInterface));
            
            int arp_table_index = mac_get_from_ip(dest_ip);
            NewEthernetFrame->send(arp_table_index != 0xFF ? ArpTable[arp_table_index].mac_address : mac_broadcast, netapi_mac_get(), ETHERNET_TYPE_IPV4, (uint8_t*)IpHeader, final_packet_size);

            free(NewEthernetFrame);
            break;
        }

        case INTERNET_CONTROL_MESSAGE_PROTOCOL:
        {
            uint8_t* protocol_header = (uint8_t*)IpHeader;
            protocol_header = protocol_header + IPV4_HEADER_SIZE;
            IcmpPacket* Packet = (IcmpPacket*)protocol_header;

            memcpy((uint8_t*)Packet, data, packet_size);

            EthernetFrameInterface* NewEthernetFrame = (EthernetFrameInterface*)malloc(sizeof(EthernetFrameInterface));
            int arp_table_index = mac_get_from_ip(dest_ip);
            NewEthernetFrame->send(arp_table_index != 0xFF ? ArpTable[arp_table_index].mac_address : mac_broadcast, netapi_mac_get(), ETHERNET_TYPE_IPV4, (uint8_t*)IpHeader, final_packet_size);
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
        InternetProtocolInterface InternetProtocolPacket;
        InternetProtocolPacket.ip4_packet_send(dest_ip, src_ip, protocol, (uint16_t)packet_size, data);
    }

}