
#include <network_protocols/ethernet_frame/ethernet_frame.hpp>
#include <libc/stdlibx.h>
#include <libcpp/cmemory.h>
#include <libcpp/endian.h>
#include <libc/stdiox.h>
#include <devices/NIC/8254x.hpp>
#include <network_protocols/arp/arp.h>
#include <libcpp/ctime.h>
#include <network_protocols/ethernet_type.h>
#include <network_protocols/internet_protocol/ipv4/ip.h>

extern "C" void i8254x_packet_send(uint32_t address, uint16_t length);
extern "C" uint32_t* i8254x_class_return(void);


void EthernetFrameInterface::send(const uint8_t* mac_destination, const uint8_t* mac_source, uint16_t protocol, const uint8_t* buffer, uint16_t length)
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

    if(memcmp(const_cast<uint8_t*>(mac_destination), (uint8_t*)"\0\0\0\0\0", 6) && memcmp(const_cast<uint8_t*>(mac_source), (uint8_t*)"\0\0\0\0\0", 6))
        this->receive((uint8_t*)FrameHeader);

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

EthernetFrameInterface EthernetFrameSubsystem;

extern "C"
{
    void ethernet_frame_send(uint8_t* mac_destination, uint8_t* mac_source, uint16_t protocol, uint8_t* buffer, uint16_t length)
    {
        EthernetFrameSubsystem.send(mac_destination, mac_source, protocol, buffer, length);
    }
}

