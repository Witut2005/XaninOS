
#include <network_protocols/arp/arp.h>
#include <network_protocols/ethernet_frame/ethernet_frame.hpp>
#include <libcpp/cstdlib.h>
#include <libcpp/cmemory.h>
#include <libcpp/endian.h>
#include <libcpp/cstdio.h>


ArpTableEntry ArpTable[10] = {0,0};

extern "C"
{

    void send_arp_request(AddressResolutionProtocol* arp)
    {
        // xprintf("x");
        EthernetFrameInterface* EthernetFrameSubsystem = (EthernetFrameInterface*)malloc(sizeof(EthernetFrameInterface));    
        EthernetFrameSubsystem->send(arp->destination_hardware_address, arp->source_hardware_address,  ARP_ETHER_TYPE, (uint8_t*)arp, sizeof(AddressResolutionProtocol));
        free(EthernetFrameSubsystem);
    }

    AddressResolutionProtocol* prepare_arp_request(AddressResolutionProtocol* arp, uint16_t hardware_type, uint16_t protocol_type, 
                                                    uint8_t hardware_address_length, uint8_t protocol_address_length, uint16_t opcode,
                                                        uint8_t* source_hardware_address, uint32_t source_protocol_address, uint8_t* destination_hardware_address,
                                                            uint32_t destination_protocol_address)
    {
        arp->hardware_type = endian_switch(hardware_type);
        arp->protocol_type = endian_switch(protocol_type);
        arp->hardware_address_length = hardware_address_length;
        arp->protocol_address_length = protocol_address_length;
        arp->opcode = endian_switch(opcode);
        memcpy(arp->source_hardware_address, source_hardware_address, 6);
        arp->source_protocol_address = source_protocol_address;
        memcpy(arp->destination_hardware_address, destination_hardware_address, 6);
        arp->destination_protocol_address = destination_protocol_address;
        return arp;

    }


    void arp_reply_handle(AddressResolutionProtocol* arp_header)
    {

        auto* i = ArpTable;
        while(i->ip_address[0] == 0)
            i++;

        memcpy(i->mac_address, arp_header->source_hardware_address, 6);
        memcpy(i->ip_address, arp_header->source_hardware_address, 4);
    }

}