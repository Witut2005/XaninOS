
#pragma once

#include <stdint.h>

#define ARP_ETHERNET 0x1
#define ARP_IP_PROTOCOL 0x800
#define ARP_REQUEST_REPLY 0x2
#define ARP_GET_MAC 0x1
#define ARP_ETHER_TYPE 0x0806

#define ARP_TABLE_ENTRIES 100

#ifdef __cplusplus
extern "C" {
#endif
struct ArpTableEntry
{
    uint8_t mac_address[6];
    uint8_t ip_address[4];
}__attribute__((packed));

struct AddressResolutionProtocol
{
    uint16_t hardware_type; // Hardware type
    uint16_t protocol_type; // Protocol type
    uint8_t hardware_address_length; // Hardware address length (Ethernet = 6)
    uint8_t protocol_address_length; // Protocol address length (IPv4 = 4)
    uint16_t opcode; // ARP Operation Code
    uint8_t source_hardware_address[6]; // Source hardware address - hlen bytes (see above)
    uint32_t source_protocol_address; // Source protocol address - plen bytes (see above). If IPv4 can just be a "u32" type.
    uint8_t destination_hardware_address[6]; // Destination hardware address - hlen bytes (see above)
    uint32_t destination_protocol_address; // Destination protocol address - plen bytes (see above). If IPv4 can just be a "u32" type.
}__attribute__((packed));

#ifdef __cplusplus
}
#endif

#ifndef __cplusplus
    typedef struct AddressResolutionProtocol AddressResolutionProtocol;
    typedef struct ArpTableEntry ArpTableEntry;
    extern void send_arp_request(AddressResolutionProtocol* arp);
    extern AddressResolutionProtocol* prepare_arp_request(AddressResolutionProtocol* arp, uint16_t hardware_type, uint16_t protocol_type, 
                                                    uint8_t hardware_address_length, uint8_t protocol_address_length, uint16_t opcode,
                                                        uint8_t* source_hardware_address, uint32_t source_protocol_address, uint8_t* destination_hardware_address,
                                                            uint32_t destination_protocol_address); 
    extern void arp_reply_handle(AddressResolutionProtocol* arp_header);
    extern uint8_t mac_get_from_ip(uint32_t ip);


#else
    extern "C" void send_arp_request(AddressResolutionProtocol* arp);
    extern "C" AddressResolutionProtocol* prepare_arp_request(AddressResolutionProtocol* arp, uint16_t hardware_type, uint16_t protocol_type, 
                                                    uint8_t hardware_address_length, uint8_t protocol_address_length, uint16_t opcode,
                                                        uint8_t* source_hardware_address, uint32_t source_protocol_address, uint8_t* destination_hardware_address,
                                                            uint32_t destination_protocol_address);
    extern "C" void arp_reply_handle(AddressResolutionProtocol* arp_header);
    extern "C" uint8_t mac_get_from_ip(uint32_t ip);

#endif

extern ArpTableEntry ArpTable[ARP_TABLE_ENTRIES];
extern ArpTableEntry LastArpReply;
extern uint8_t mac_broadcast[];


