
#include <network_protocols/arp/arp.h>
#include <network_protocols/ethernet_frame/ethernet_frame.hpp>
#include <libcpp/cstdlib.h>
#include <libcpp/cmemory.h>
#include <libcpp/endian.h>
#include <libcpp/cstdio.h>
#include <libcpp/cstring.h>
#include <libcpp/ctime.h>


ArpTableEntry ArpTable[ARP_TABLE_ENTRIES] = {0,0};
ArpTableEntry LastArpReply = {0,0};

uint8_t mac_broadcast[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

extern "C"
{

    void send_arp_request(AddressResolutionProtocol* arp)
    {
        EthernetFrameInterface EthernetFrameSubsystem;// = (EthernetFrameInterface*)malloc(sizeof(EthernetFrameInterface));    
        EthernetFrameSubsystem.send(arp->destination_hardware_address, arp->source_hardware_address,  ARP_ETHER_TYPE, (uint8_t*)arp, sizeof(AddressResolutionProtocol));
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
        if(strncmp((char*)arp_header->destination_hardware_address, "\0\0\0\0\0\0", 6))
            return;

        memcpy(LastArpReply.mac_address, arp_header->source_hardware_address, 6);
        memcpy(LastArpReply.ip_address, (uint8_t*)&arp_header->source_protocol_address, 4);

        auto* i = ArpTable;
        while(i->ip_address[0] == 0 and *(uint32_t*)i->mac_address == 0)
            i++;

        memcpy(i->mac_address, arp_header->source_hardware_address, 6);
        memcpy(i->ip_address, (uint8_t*)&arp_header->source_protocol_address, 4);
    }

    uint8_t mac_get_from_ip(uint32_t ip)
    {

        uint8_t* tmp = (uint8_t*)&ip;

        for(int i = 0; i < ARP_TABLE_ENTRIES; i++)
        {
            for(int j = 0; j < 4; j++)
                xprintf("%d.", tmp[j]);
            xprintf("\n");
            tmp = ArpTable[i].ip_address;
            for(int j = 0; j < 4; j++)
                xprintf("%d.", tmp[j]);
            xprintf("\n");
            // xprintf("%d", *(uint8_t*)&ip);
            if(memcmp((uint8_t*)&ip, ArpTable[i].ip_address, 4))
            {

                return i;
            
            }
        }
        
        return 0xFF;

    }


}