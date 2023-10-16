
#pragma once

#include <stdint.h>
#include <sys/net/netapi/objects/response.h>


enum ARP_ENUM
{
    ARP_ETHERNET = 0x1,
    ARP_IP_PROTOCOL = 0x800,
    ARP_REPLY = 0x2,
    ARP_GET_MAC = 0x1,
    ARP_ETHER_TYPE = 0x0806,
    ARP_MAC_LENGTH = 6,
    ARP_IP_LENGTH = 4,
    ARP_TABLE_NO_SUCH_ENTRY = 0xFF

};

#define ARP_TABLE_ENTRIES 0xFF 

#ifdef __cplusplus

#include <lib/libcpp/container/map.hpp>

class ArpModule
{
    public:
        static std::UnorderedMap<uint32_t, NetworkResponse*> PacketsInfo;
};

#endif

#ifdef __cplusplus
extern "C" {
#endif
struct ArpTableEntry
{
    uint8_t mac[6];
    uint8_t ip[4];
    int success;
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
#endif

#ifdef __cplusplus
extern "C" {
#endif
    void send_arp_request(AddressResolutionProtocol* arp, NetworkResponse* Response);
    AddressResolutionProtocol* prepare_arp_request(AddressResolutionProtocol* arp, uint16_t hardware_type, uint16_t protocol_type, 
                                                    uint8_t hardware_address_length, uint8_t protocol_address_length, uint16_t opcode,
                                                        uint8_t* source_hardware_address, uint32_t source_protocol_address, uint8_t* destination_hardware_address,
                                                            uint32_t destination_protocol_address);
    void arp_reply_handle(AddressResolutionProtocol* arp_header);
    uint8_t mac_get_from_ip(uint32_t ip);
    ArpTableEntry last_arp_reply_get(void);
    bool arp_table_add_entry(uint32_t ip, uint8_t* mac);

#ifdef __cplusplus
}
#endif

extern ArpTableEntry ArpTable[ARP_TABLE_ENTRIES];
// extern ArpTableEntry LastArpReply;
extern uint8_t current_arp_entry;
extern uint8_t mac_broadcast[];


