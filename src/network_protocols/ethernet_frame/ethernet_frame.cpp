
#include <network_protocols/ethernet_frame/ethernet_frame.hpp>
#include <libcpp/cstdlib.h>
#include <libcpp/cmemory.h>
#include <libcpp/endian.h>

uint32_t EthernetFrameInterface::checksum_set(EthernetFrame* EthernetHeader)
{
    EthernetHeader->checksum[0] = 0x0;
    EthernetHeader->checksum[1] = 0x20;
    EthernetHeader->checksum[2] = 0x20;
    EthernetHeader->checksum[3] = 0x3A;

    int x = *(uint32_t*) &EthernetHeader->checksum;
    return x;

}


void EthernetFrameInterface::send(NetworkDevice* device, uint8_t* mac_destination, uint8_t* mac_source, uint16_t protocol, uint8_t* buffer, uint16_t length)
{
    uint8_t* tmp = (uint8_t*)malloc(sizeof(uint8_t) * 1518);
    EthernetFrame* FrameHeader = (EthernetFrame*)tmp;

    memcpy(FrameHeader->mac_destination, mac_destination, 6);
    memcpy(FrameHeader->mac_destination, mac_destination, 6);
    FrameHeader->ethernet_type = endian_switch(protocol);

    FrameHeader->data = buffer;

    this->checksum_set(FrameHeader);


    device->send_packet((uint32_t)FrameHeader, length + ETHERNET_FRAME_MAC_HEADER_SIZE);
}

EthernetFrame* EthernetFrameInterface::receive(NetworkDevice* device)
{
    return (EthernetFrame*)device->receive_packet();
}

