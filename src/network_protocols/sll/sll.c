
#include <network_protocols/sll/sll.h>
// #include <netapi/network_device.h>
#include <libc/memory.h>
#include <libc/stdlibx.h>


// static inline void sll_frame_send(uint16_t packet_type, uint16_t link_layer_address_type, uint16_t link_layer_address_length, 
//                         uint8_t* mac, uint16_t protocol, uint8_t* data_to_send, uint16_t packet_size)
// {
//     SllFrame* PacketFrame = (SllFrame*)calloc(packet_size);//chyba
//     PacketFrame->packet_type = packet_type;
//     PacketFrame->link_layer_address_type = link_layer_address_type;
//     PacketFrame->link_layer_address_length = link_layer_address_length;
//     memcpy(PacketFrame->mac, mac, 6);
//     PacketFrame->protocol = protocol;

//     uint8_t* data = (uint8_t*)PacketFrame;
//     data = data + 0x10;
//     memcpy(data, data_to_send, packet_size + 0x10);
//     netapi_packet_send((uint32_t)PacketFrame, packet_size + 0x10);

// }
