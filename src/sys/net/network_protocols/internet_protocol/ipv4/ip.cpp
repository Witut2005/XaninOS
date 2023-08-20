
#include <sys/net/network_protocols/internet_protocol/ipv4/ip.h>
#include <sys/net/network_protocols/icmp/icmp.h>
#include <sys/net/network_protocols/dhcp/dhcp.h>
#include <sys/net/network_protocols/arp/arp.h>
#include <sys/net/netapi/network_device.h>
#include <lib/libc/stdlibx.h>
#include <sys/net/network_protocols/ethernet_frame/ethernet_frame.hpp>
#include <sys/net/network_protocols/arp/arp.h>
#include <lib/libcpp/endian.h>
#include <lib/libc/stdiox.h>
#include <sys/log/syslog.h>

#include <sys/net/network_protocols/udp/udp.hpp>
#include <sys/net/netapi/loopback/loopback.h>
#include <sys/net/netapi/objects/ip.h>

#define ETHERNET_TYPE_IPV4 0x800
#define IPV4_HEADER_SIZE 20

bool loopback_packet = false;

NetworkResponse* InternetProtocolInterface::Response;
address_t InternetProtocolInterface::PacketSent;

extern "C" bool is_loopback_packet(void)
{
    return loopback_packet;
}


uint32_t InternetProtocolInterface::create_ip_address(uint8_t ip_address[4])
{
    uint32_t tmp = *(uint32_t*)ip_address;
    return tmp;
}

extern "C" uint16_t ipv4_checksum_get(uint8_t* packet_data, uint32_t data_size)
{
    uint32_t temp = 0;
    uint16_t* data = (uint16_t*)packet_data;

    for(int i = 0; i < data_size /2; i++)
        temp += ((data[i] & 0xFF00) >> 8) | ((data[i] & 0x00FF) << 8);

    if(data_size % 2)
        temp += ((uint16_t)((char*)data)[data_size-1]) << 8;
    
    while(temp & 0xFFFF0000)
        temp = (temp & 0xFFFF) + (temp >> 16);
    
    return ((~temp & 0xFF00) >> 8) | ((~temp & 0x00FF) << 8);
}

void InternetProtocolInterface::ip4_packet_send(uint32_t dest_ip, uint32_t src_ip, uint8_t protocol, uint8_t ttl, uint8_t* data, uint16_t packet_size, NetworkResponse* Response)
{


    if(dest_ip == XaninNetworkLoopback.ip_get())
        src_ip = dest_ip;

    uint16_t final_packet_size = packet_size + IPV4_HEADER_SIZE;

    Ipv4Header* IpHeader = (Ipv4Header*)malloc(SIZE_OF(1518));
    IpHeader->version_ihl = 0x5;
    IpHeader->version_ihl |= IPV4_HEADER_VERSION << 4;
    IpHeader->tos =(uint8_t)NULL;
    IpHeader->packet_size = BIG_ENDIAN(static_cast<uint16_t>(packet_size + IPV4_HEADER_SIZE));
    IpHeader->fragment_offset_and_flags = (uint8_t)NULL;
    IpHeader->time_to_live = ttl;
    IpHeader->protocol = protocol;
    IpHeader->identification = (uint16_t)NULL;
    IpHeader->checksum = BIG_ENDIAN(static_cast<uint16_t>(packet_size + IpHeader->identification)); // oj nie wiem czy dobrze
    IpHeader->source_ip_address = BIG_ENDIAN(src_ip);
    IpHeader->destination_ip_address= BIG_ENDIAN(dest_ip);

    switch (protocol)
    {
        case USER_DATAGRAM_PROTOCOL: 
        {
            uint8_t* protocol_header = (uint8_t*)IpHeader;
            protocol_header = protocol_header + IPV4_HEADER_SIZE;
            UdpHeader* UdpPacket = (UdpHeader*)protocol_header;

            memcpy((uint8_t*)UdpPacket, data , 1518 - SIZE_OF(Ipv4Header) - SIZE_OF(UdpHeader));

            int arp_table_index = mac_get_from_ip(dest_ip);
            
            EthernetFrameInterface::send(arp_table_index != ARP_TABLE_NO_SUCH_ENTRY ? ArpTable[arp_table_index].mac : mac_broadcast, netapi_mac_get(xanin_ip_get()), ETHERNET_TYPE_IPV4, (uint8_t*)IpHeader, final_packet_size, Response);

            break;
        }

        case INTERNET_CONTROL_MESSAGE_PROTOCOL:
        {


            uint8_t* protocol_header = (uint8_t*)IpHeader;
            protocol_header = protocol_header + IPV4_HEADER_SIZE;
            IcmpPacket* Packet = (IcmpPacket*)protocol_header;

            memcpy((uint8_t*)Packet, data, packet_size);

            if(Packet->type == ICMP_ECHO_REQUEST)
            {
                if(Response)
                    IcmpModule::PacketsInfo.insert_or_assign(std::make_pair(LITTLE_ENDIAN(Packet->echo_id), LITTLE_ENDIAN(Packet->echo_sequence)), Response);
            }

            int arp_table_index = mac_get_from_ip(dest_ip);
            uint8_t* macd = IpHeader->destination_ip_address != xanin_ip_get() ? ArpTable[arp_table_index].mac : XaninNetworkLoopback.mac_get();

            // if mac is not in ARP table
            if(arp_table_index == ARP_TABLE_NO_SUCH_ENTRY)
            {
                AddressResolutionProtocol* ArpPacket = (AddressResolutionProtocol*)calloc(SIZE_OF(AddressResolutionProtocol));
                prepare_arp_request(ArpPacket, ARP_ETHERNET, ARP_IP_PROTOCOL, ARP_MAC_LENGTH, ARP_IP_LENGTH, ARP_GET_MAC, netapi_mac_get(xanin_ip_get()), xanin_ip_get(), mac_broadcast, dest_ip);
                
                for(int i = 0; i < 20; i++)
                    send_arp_request(ArpPacket, NULL);

                free(ArpPacket);

                arp_table_index = mac_get_from_ip(dest_ip);
                macd = ArpTable[arp_table_index].mac;
                if(arp_table_index == ARP_TABLE_NO_SUCH_ENTRY)
                {
                    printk("NO SUCH MAC !!! (icmp module)");
                    free(IpHeader);
                    return;
                }
            }

            // send icmp request
            EthernetFrameInterface::send(macd, netapi_mac_get(src_ip), ETHERNET_TYPE_IPV4, (uint8_t*)IpHeader, final_packet_size, NULL);

            break;
        }


    }

    free(IpHeader);

}

void InternetProtocolInterface::ipv4_packet_receive(Ipv4Header* PacketData)
{

    // if((endian_switch(PacketData->destination_ip_address) == LOOPBACK_IP_ADDRESS) && (LITTLE_ENDIAN(PacketData->source_ip_address) == LOOPBACK_IP_ADDRESS))

    if(net::is_system_ip(LITTLE_ENDIAN(PacketData->destination_ip_address)))
        loopback_packet = true;
    else
        loopback_packet = false;

    switch(PacketData->protocol)
    {
        case USER_DATAGRAM_PROTOCOL:
        {

            break;
        }

        case INTERNET_CONTROL_MESSAGE_PROTOCOL:
        {
            IcmpPacket* IcmpPacketReceived = (IcmpPacket*)((uint8_t*)(PacketData) + SIZE_OF(Ipv4Header));
            IcmpModule::receive(IcmpPacketReceived, LITTLE_ENDIAN(PacketData->source_ip_address));
            break;
        }

    }
}


extern "C"
{

    void ipv4_packet_receive(Ipv4Header* PacketData)
    {
        InternetProtocolInterface* InternetProtocolSubsystem = (InternetProtocolInterface*)malloc(SIZE_OF(InternetProtocolInterface));
        InternetProtocolSubsystem->ipv4_packet_receive(PacketData);
    }

    uint32_t create_ip_address(uint8_t ip_address[4])
    {
        InternetProtocolInterface* InternetProtocolSubsystem = (InternetProtocolInterface*)malloc(SIZE_OF(InternetProtocolInterface));
        uint32_t tmp = InternetProtocolSubsystem->create_ip_address(ip_address);
        free(InternetProtocolSubsystem);
        return tmp;
    }

    void ipv4_packet_send(uint32_t dest_ip, uint32_t src_ip, uint8_t protocol, uint8_t ttl,  uint8_t* data, uint16_t packet_size, NetworkResponse* Response)
    {
        InternetProtocolInterface InternetProtocolPacket;
        InternetProtocolPacket.ip4_packet_send(dest_ip, src_ip, protocol, ttl, data, packet_size, Response);
    }

}

