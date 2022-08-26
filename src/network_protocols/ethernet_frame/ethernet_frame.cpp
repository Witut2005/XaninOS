
#include <network_protocols/ethernet_frame/ethernet_frame.hpp>
#include <libcpp/cstdlib.h>
#include <libcpp/cmemory.h>
#include <libcpp/endian.h>
#include <libcpp/cstdio.h>
#include <devices/NIC/8254x.hpp>

extern "C" void i8254x_packet_send(uint32_t address, uint16_t length);
extern "C" uint32_t* i8254x_class_return(void);

void EthernetFrameInterface::send(uint8_t* mac_destination, uint8_t* mac_source, uint16_t protocol, uint8_t* buffer, uint16_t length)
{
    

    uint8_t* tmp = (uint8_t*)malloc(sizeof(uint8_t) * 1518);
    EthernetFrame* FrameHeader = (EthernetFrame*)tmp;

    memcpy(FrameHeader->mac_destination, mac_destination, 6);
    memcpy(FrameHeader->mac_source, mac_source, 6);
    FrameHeader->ethernet_type = 0x806;//endian_switch(protocol);

    tmp = tmp + ETHERNET_FRAME_MAC_HEADER_SIZE;

    int i = 0;
    for(; i < length; i++)
    {
        tmp[i] = buffer[i];
    }

    tmp[i] = 0x0;
    tmp[i + 1] = 0x20;
    tmp[i + 2] = 0x20;
    tmp[i + 3] = 0x3A;


    NetworkDevice::send_packet((uint32_t)FrameHeader, length + ETHERNET_FRAME_MAC_HEADER_SIZE, &NetworkSubsystem);

    free(FrameHeader);
    


}

EthernetFrame* EthernetFrameInterface::receive(void)
{
    // return (EthernetFrame*)NetworkDevice::MainNetworkDevice->receive_packet();
}

EthernetFrameInterface EthernetFrameSubsystem;

extern "C"
{
    void ethernet_frame_send(uint8_t* mac_destination, uint8_t* mac_source, uint16_t protocol, uint8_t* buffer, uint16_t length)
    {
        EthernetFrameSubsystem.send(mac_destination, mac_source, protocol, buffer, length);
    }
}

