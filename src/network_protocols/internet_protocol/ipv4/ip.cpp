
#include <network_protocols/internet_protocol/ipv4/ip.hpp>
#include <network_protocols/icmp/icmp.h>
#include <network_protocols/dhcp/dhcp.h>
#include <libc/stdlibx.h>
#include <network_protocols/ethernet_frame/ethernet_frame.hpp>
#include <network_protocols/arp/arp.h>
#include <libcpp/endian.h>
#include <libcpp/cmemory.h>
#include <libc/stdiox.h>
#include <network_protocols/sll/sll.h>

#define ETHERNET_TYPE_IPV4 0x800
#define IPV4_HEADER_SIZE 20


uint32_t InternetProtocolInterface::create_ip_address(uint8_t ip_address[4])
{

    uint32_t tmp = *(uint32_t*)ip_address;
    return tmp;

}

extern "C" uint16_t ipv4_checksum_get(uint16_t* data, uint32_t data_size)
{
    uint32_t temp = 0;

    for(int i = 0; i < data_size /2; i++)
        temp += ((data[i] & 0xFF00) >> 8) | ((data[i] & 0x00FF) << 8);

    if(data_size % 2)
        temp += ((uint16_t)((char*)data)[data_size-1]) << 8;
    
    while(temp & 0xFFFF0000)
        temp = (temp & 0xFFFF) + (temp >> 16);
    
    return ((~temp & 0xFF00) >> 8) | ((~temp & 0x00FF) << 8);
}

void InternetProtocolInterface::ip4_packet_send(uint32_t dest_ip, uint32_t src_ip, uint8_t protocol, uint8_t ttl, uint8_t* data, uint16_t packet_size)
{
    uint16_t final_packet_size = packet_size + IPV4_HEADER_SIZE;

    Ipv4Header* IpHeader = (Ipv4Header*)malloc(sizeof(1518));
    IpHeader->version_ihl = 0x5;
    IpHeader->version_ihl |= IPV4_HEADER_VERSION << 4;
    IpHeader->packet_size = endian_switch(static_cast<uint16_t>(packet_size + IPV4_HEADER_SIZE));
    IpHeader->fragment_offset_and_flags = 0x0;
    IpHeader->time_to_live = ttl;
    IpHeader->protocol = protocol;
    IpHeader->identification = 0x0;
    IpHeader->checksum = endian_switch(static_cast<uint16_t>(packet_size + IpHeader->identification)); // oj nie wiem czy dobrze
    IpHeader->source_ip_address = endian_switch(src_ip);
    IpHeader->destination_ip_address= endian_switch(dest_ip);


    // xprintf("%d ", *final_packet_size);


    switch (protocol)
    {
        case USER_DATAGRAM_PROTOCOL: 
        {
            uint8_t* protocol_header = (uint8_t*)IpHeader;
            protocol_header = protocol_header + IPV4_HEADER_SIZE;
            UdpHeader* UdpPacket = (UdpHeader*)protocol_header;

            memcpy((uint8_t*)UdpPacket, data , 1518 - sizeof(Ipv4Header) - sizeof(UdpHeader));

            EthernetFrameInterface* NewEthernetFrame = (EthernetFrameInterface*)malloc(sizeof(EthernetFrameInterface));
            
            int arp_table_index = mac_get_from_ip(dest_ip);
            NewEthernetFrame->send(arp_table_index != 0xFF ? ArpTable[arp_table_index].mac_address : mac_broadcast, netapi_mac_get(), ETHERNET_TYPE_IPV4, (uint8_t*)IpHeader, final_packet_size);

            free(NewEthernetFrame);
            break;
        }

        case INTERNET_CONTROL_MESSAGE_PROTOCOL:
        {
            // uint8_t* protocol_header = (uint8_t*)IpHeader;
            // protocol_header = protocol_header + IPV4_HEADER_SIZE;
            // IcmpPacket* Packet = (IcmpPacket*)protocol_header;

            // memcpy((uint8_t*)Packet, data, packet_size);
            // uint32_t os_ip_address = 192 << 24 |  168 << 16 | 19 << 8 |12;


            // sll_frame_send(0x4, 0x1, 0x6, netapi_mac_get(), ETHERNET_TYPE_IPV4, (uint8_t*)IpHeader, packet_size);

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

void InternetProtocolInterface::ipv4_packet_receive(Ipv4Header* PacketData)
{
    // screen_clear();
    // xprintf("0x%x", PacketData->protocol);
    switch(PacketData->protocol)
    {
        case USER_DATAGRAM_PROTOCOL:
        {

            break;
        }

        case INTERNET_CONTROL_MESSAGE_PROTOCOL:
        {
            asm("int 0x0");
            break;
        }

    }
}


extern "C"
{

    void ipv4_packet_receive(Ipv4Header* PacketData)
    {
        InternetProtocolInterface* InternetProtocolSubsystem = (InternetProtocolInterface*)malloc(sizeof(InternetProtocolInterface));
        InternetProtocolSubsystem->ipv4_packet_receive(PacketData);
    }

    uint32_t create_ip_address(uint8_t ip_address[4])
    {
        InternetProtocolInterface* InternetProtocolSubsystem = (InternetProtocolInterface*)malloc(sizeof(InternetProtocolInterface));
        uint32_t tmp = InternetProtocolSubsystem->create_ip_address(ip_address);
        free(InternetProtocolSubsystem);
        return tmp;
    }

    void ipv4_packet_send(uint32_t dest_ip, uint32_t src_ip, uint8_t protocol, uint8_t ttl,  uint8_t* data, uint16_t packet_size)
    {
        InternetProtocolInterface InternetProtocolPacket;
        InternetProtocolPacket.ip4_packet_send(dest_ip, src_ip, protocol, ttl, data, packet_size);
    }

}

