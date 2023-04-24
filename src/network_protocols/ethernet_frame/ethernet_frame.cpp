
#include <network_protocols/ethernet_frame/ethernet_frame.hpp>
#include <libc/stdlibx.h>
#include <libcpp/endian.h>
#include <libc/stdiox.h>
#include <devices/NIC/8254x.hpp>
#include <network_protocols/arp/arp.h>
#include <libc/time.h>
#include <network_protocols/ethernet_type.h>
#include <network_protocols/internet_protocol/ipv4/ip.h>

extern std::UnorderedMap<net::MacAddress, NetworkResponse*> ArpPacketsInfo;

void EthernetFrameInterface::send(const uint8_t* mac_destination, const uint8_t* mac_source, uint16_t protocol, const uint8_t* buffer, uint16_t length, NetworkResponse* Response)
{
    
    uint8_t* tmp = (uint8_t*)malloc(sizeof(uint8_t) * 2000);
    EthernetFrame* FrameHeader = (EthernetFrame*)tmp;

    memcpy(FrameHeader->mac_destination, const_cast<uint8_t*>(mac_destination), 6);
    memcpy(FrameHeader->mac_source, const_cast<uint8_t*>(mac_source), 6);
    FrameHeader->ethernet_type = endian_switch(protocol);

    tmp = tmp + ETHERNET_FRAME_MAC_HEADER_SIZE;

    int i = 0;
    for(; i < length; i++)
        tmp[i] = buffer[i];

    netapi_packet_send((uint8_t*)FrameHeader, length + ETHERNET_FRAME_MAC_HEADER_SIZE);

    if(memcmp(const_cast<uint8_t*>(mac_source), (uint8_t*)"\0\0\0\0\0", 6))
        EthernetFrameInterface::receive((uint8_t*)FrameHeader);

    free(FrameHeader);

}

void EthernetFrameInterface::receive(uint8_t* buffer)
{

    EthernetFrame* Frame = (EthernetFrame*)malloc(sizeof(EthernetFrame));
    uint8_t* tmp = buffer;
    

    memcpy(Frame->mac_destination, tmp, 6);
    tmp = tmp + 6;
    
    memcpy(Frame->mac_source, tmp, 6);
    tmp = tmp + 6;

    Frame->ethernet_type = static_cast<uint16_t>(((tmp[0] << 8) | tmp[1]));
    tmp = tmp + 2;

    Frame->data = tmp;

    switch(Frame->ethernet_type)
    {
        case ARP_ETHER_TYPE: 
        {
            // ArpModule::PacketsInfo[Frame->mac_source]->success = true;
            // memcpy(ArpModule::PacketsInfo[Frame->mac_source]->data, Frame->data, sizeof(AddressResolutionProtocol));

            arp_reply_handle((AddressResolutionProtocol*)Frame->data);
            break;
        }
            
        case ETHERNET_TYPE_IPV4:
        {
            uint8_t* data =  (uint8_t*)Frame->data;
            InternetProtocolInterface* InternetProtocolSubsystem = (InternetProtocolInterface*)malloc(sizeof(InternetProtocolInterface));
            InternetProtocolSubsystem->ipv4_packet_receive((Ipv4Header*)data);
            break;
        }

    }

    free(Frame);

}

extern "C"
{
    void ethernet_frame_send(uint8_t* mac_destination, uint8_t* mac_source, uint16_t protocol, uint8_t* buffer, uint16_t length, NetworkResponse* Response)
    {
        EthernetFrameInterface* EthernetFrameSubsystem  = (EthernetFrameInterface*)calloc(sizeof(EthernetFrameSubsystem));
        EthernetFrameSubsystem->send(mac_destination, mac_source, protocol, buffer, length, Response);
    }
}

