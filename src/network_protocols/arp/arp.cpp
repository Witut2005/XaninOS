
#include <network_protocols/arp/arp.h>
#include <network_protocols/ethernet_frame/ethernet_frame.hpp>
#include <libc/stdlibx.h>
#include <libcpp/cmemory.h>
#include <libcpp/endian.h>
#include <libc/stdiox.h>
#include <libc/string.h>
#include <libcpp/ctime.h>


ArpTableEntry ArpTable[ARP_TABLE_ENTRIES] = {0,0,false};
ArpTableEntry LastArpReply = {0,0,false};

uint8_t mac_broadcast[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t current_arp_entry = 0x0;

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
        arp->source_protocol_address = endian_switch(source_protocol_address);
        memcpy(arp->destination_hardware_address, destination_hardware_address, 6);
        arp->destination_protocol_address = endian_switch(destination_protocol_address);
        return arp;

    }


    void arp_reply_handle(AddressResolutionProtocol* arp_header)
    {

        uint32_t xanin_ip_address = xanin_ip_get();
        if(endian_switch(arp_header->destination_protocol_address) == xanin_ip_address && endian_switch(arp_header->opcode) == ARP_GET_MAC)
        {
            AddressResolutionProtocol* XaninArpReply = (AddressResolutionProtocol*)calloc(sizeof(AddressResolutionProtocol));
            prepare_arp_request(XaninArpReply, ARP_ETHERNET, ARP_IP_PROTOCOL, 0x6, 0x4, ARP_REPLY, netapi_mac_get(), 192 << 24 | 168 << 16 | 19 << 8 | 12, arp_header->destination_hardware_address, arp_header->destination_protocol_address);
            send_arp_request(XaninArpReply);
            free(XaninArpReply);
        }

        if(endian_switch(arp_header->opcode) != ARP_REPLY)//HANDLE ONLY REPLIES (NOT ARP PROBE ETC)
        {
            memset((uint8_t*)&LastArpReply, 0, sizeof(LastArpReply));
            return;
        }

        if(memcmp((uint8_t*)arp_header->destination_hardware_address, 0x0, 6))
        {
            memset((uint8_t*)&LastArpReply, 0, sizeof(LastArpReply));
            return;
        }

        uint32_t ip_addr = endian_switch(arp_header->source_protocol_address);

        uint8_t arp_entry_used = current_arp_entry;

        // if(!ip_addr && !arp_header->source_hardware_address[0])
        //     return;

        for(int i = 0; i < ARP_TABLE_ENTRIES; i++)
        {
            if(memcmp(ArpTable[i].ip_address, (uint8_t*)&ip_addr, 4))
            {
                arp_entry_used = i;
                current_arp_entry--;
            }
        }

        memcpy(ArpTable[arp_entry_used].mac_address, LastArpReply.mac_address, 6);
        memcpy(ArpTable[arp_entry_used].ip_address, LastArpReply.ip_address, 4);

        memcpy(LastArpReply.mac_address, arp_header->source_hardware_address, 6);
        memcpy(LastArpReply.ip_address, (uint8_t*)&ip_addr, 4);
        LastArpReply.success = true;

        current_arp_entry++;
    }

    uint8_t mac_get_from_ip(uint32_t ip)
    {

        const ArpTableEntry* const table = (ArpTableEntry*)ArpTable;

        for(int i = 0; i < ARP_TABLE_ENTRIES; i++)
        {
            const uint8_t* tmp = (uint8_t*)&table[i].ip_address;
            for(int j = 0; j < 4; j++)
                if(memcmp((uint8_t*)&ip, ArpTable[i].ip_address, 4))
                    return i;
        }
        
        return 0xFF;

    }

    ArpTableEntry last_arp_reply_get(void)
    {
        return LastArpReply;
    }

}
