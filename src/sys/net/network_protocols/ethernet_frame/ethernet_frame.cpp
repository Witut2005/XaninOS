
#include <lib/libc/stdlibx.h>
#include <lib/libcpp/endian.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/time.h>

#include <sys/devices/nic/8254x.hpp>
#include <sys/net/network_protocols/arp/arp.h>
#include <sys/net/network_protocols/ethernet_type.h>
#include <sys/net/network_protocols/internet_protocol/ipv4/ip.h>
#include <sys/net/network_protocols/ethernet_frame/ethernet_frame.hpp>

extern std::UnorderedMap<net::MacAddress, NetworkResponse*> ArpPacketsInfo;

void EthernetFrameInterface::send(const uint8_t* mac_destination, const uint8_t* mac_source, uint16_t protocol, const uint8_t* buffer, uint16_t length, NetworkResponse* Response)
{
    if(length > ETHERNET_FRAME_PAYLOAD_SIZE)
        length = ETHERNET_FRAME_PAYLOAD_SIZE;
    
    uint8_t* FrameBytes = (uint8_t*)calloc(SIZE_OF(uint8_t) * 2000);
    EthernetFrame* FrameHeader = (EthernetFrame*)FrameBytes;

    memcpy(FrameHeader->mac_destination, const_cast<uint8_t*>(mac_destination), 6);
    memcpy(FrameHeader->mac_source, const_cast<uint8_t*>(mac_source), 6);
    FrameHeader->ethernet_type = BIG_ENDIAN(protocol);

    for(int i = 0; i < length; i++)
        FrameHeader->data[i] = buffer[i];

    netapi_packet_send((uint8_t*)FrameHeader, length + ETHERNET_FRAME_MAC_HEADER_SIZE);

    if(net::is_system_mac(const_cast<uint8_t*>(mac_source)))
        EthernetFrameInterface::receive((uint8_t*)FrameHeader);

    free(FrameHeader);

}

void EthernetFrameInterface::receive(uint8_t* buffer)
{

    EthernetFrame Frame; 
    uint8_t* tmp = buffer;
    

    memcpy(Frame.mac_destination, tmp, 6);
    tmp = tmp + 6;
    
    memcpy(Frame.mac_source, tmp, 6);
    tmp = tmp + 6;

    Frame.ethernet_type = static_cast<uint16_t>(((tmp[0] << 8) | tmp[1]));
    tmp = tmp + 2;

    memcpy(Frame.data, tmp, ETHERNET_FRAME_PAYLOAD_SIZE);

    switch(Frame.ethernet_type)
    {
        case ARP_ETHER_TYPE: 
        {
            // ArpModule::PacketsInfo[Frame->mac_source]->success = true;
            // memcpy(ArpModule::PacketsInfo[Frame->mac_source]->data, Frame->data, SIZE_OF(AddressResolutionProtocol));

            arp_reply_handle((AddressResolutionProtocol*)&Frame.data);
            break;
        }
            
        case ETHERNET_TYPE_IPV4:
        {
            InternetProtocolInterface* InternetProtocolSubsystem = (InternetProtocolInterface*)malloc(SIZE_OF(InternetProtocolInterface));
            InternetProtocolSubsystem->ipv4_packet_receive((Ipv4Header*)&Frame.data);
            break;
        }

    }


}

extern "C"
{
    void ethernet_frame_send(uint8_t* mac_destination, uint8_t* mac_source, uint16_t protocol, uint8_t* buffer, uint16_t length, NetworkResponse* Response)
    {
        EthernetFrameInterface::send(mac_destination, mac_source, protocol, buffer, length, Response);
    }
}

