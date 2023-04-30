
#include <network_protocols/arp/arp.h>
#include <network_protocols/ethernet_frame/ethernet_frame.hpp>
#include <libc/stdlibx.h>
#include <libcpp/endian.h>
#include <libc/stdiox.h>
#include <libc/string.h>
#include <libc/time.h>
#include <netapi/loopback/loopback.h>
#include <netapi/objects/ip.h>
#include <netapi/objects/mac.hpp>
ArpTableEntry ArpTable[ARP_TABLE_ENTRIES] = {0,0,false};
ArpTableEntry LastArpReply = {0,0,false};

uint8_t mac_broadcast[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t current_arp_entry = 0x0;

std::UnorderedMap<uint32_t, NetworkResponse*> ArpModule::PacketsInfo;

#define send_arp send_arp_request

extern "C" void arp_module_init(void)
{
    ArpModule::PacketsInfo.init();
}

extern "C" bool arp_module_status(void)
{
    return ArpModule::PacketsInfo.is_initialized();
}

extern "C"
{

    void send_arp_request(AddressResolutionProtocol* Arp, NetworkResponse* Response)
    {

        if(Response != NULL)
            ArpModule::PacketsInfo.insert(endian_switch(Arp->destination_protocol_address), Response);

        if(net::is_system_ip(endian_switch(Arp->destination_protocol_address)))
        {
            memset(Arp->source_hardware_address, 0, ARP_MAC_LENGTH);
            memset(Arp->destination_hardware_address, 0, ARP_MAC_LENGTH);
            EthernetFrameInterface::send(Arp->destination_hardware_address, Arp->source_hardware_address,  ARP_ETHER_TYPE, (uint8_t*)Arp, sizeof(AddressResolutionProtocol), Response);
            arp_reply_handle(Arp);
        }

        else
            EthernetFrameInterface::send(Arp->destination_hardware_address, Arp->source_hardware_address,  ARP_ETHER_TYPE, (uint8_t*)Arp, sizeof(AddressResolutionProtocol), Response);

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

    void arp_loopback_reply(AddressResolutionProtocol* ArpHeader)
    {
        if(endian_switch(ArpHeader->opcode) != ARP_GET_MAC)
            return;

        if(ArpModule::PacketsInfo.exists(endian_switch(ArpHeader->destination_protocol_address)))
        {
            ArpModule::PacketsInfo[endian_switch(ArpHeader->destination_protocol_address)]->success = true;
            memcpy((uint8_t*)ArpModule::PacketsInfo[endian_switch(ArpHeader->destination_protocol_address)]->data, (uint8_t*)ArpHeader, sizeof(AddressResolutionProtocol));
        }
    }


    void arp_reply_handle(AddressResolutionProtocol* ArpHeader)
    {


        if(net::is_system_ip(endian_switch(ArpHeader->destination_protocol_address)))
        {
            
            if(net::is_system_mac(ArpHeader->source_hardware_address))
                arp_loopback_reply(ArpHeader);
            
            else 
            {
                if(endian_switch(ArpHeader->opcode) == ARP_REPLY)
                {
                    if(ArpModule::PacketsInfo.exists(endian_switch(ArpHeader->source_protocol_address)))
                    {
                        xprintf("nicho");
                        ArpModule::PacketsInfo[endian_switch(ArpHeader->source_protocol_address)]->success = true;
                        xprintf(" 0x%x\n", ArpModule::PacketsInfo[endian_switch(ArpHeader->source_protocol_address)]->data);
                        memcpy((uint8_t*)ArpModule::PacketsInfo[endian_switch(ArpHeader->source_protocol_address)]->data, (uint8_t*)ArpHeader, sizeof(AddressResolutionProtocol));
                    }
                }

                else if(endian_switch(ArpHeader->opcode) == ARP_GET_MAC)
                {
                    AddressResolutionProtocol XaninArpReply;
                    prepare_arp_request(&XaninArpReply, ARP_ETHERNET, ARP_IP_PROTOCOL, 0x6, 0x4, ARP_REPLY, netapi_mac_get(xanin_ip_get()), xanin_ip_get(), ArpHeader->source_hardware_address, ArpHeader->source_protocol_address);
                    send_arp(&XaninArpReply, NULL);
                }

            }

        }

        else if(endian_switch(ArpHeader->opcode) != ARP_REPLY)//HANDLE ONLY REPLIES (NOT ARP PROBE ETC)
            return;

        uint32_t ip_addr = endian_switch(ArpHeader->source_protocol_address);
        uint8_t arp_entry_used = current_arp_entry;

        for(int i = 0; i < ARP_TABLE_ENTRIES; i++)
        {
            if(memcmp(ArpTable[i].ip, (uint8_t*)&ip_addr, 4))
            {
                arp_entry_used = i;
                current_arp_entry--;
            }
        }

        memcpy(ArpTable[arp_entry_used].mac, ArpHeader->source_hardware_address, 6);
        memcpy(ArpTable[arp_entry_used].ip, (uint8_t*)&ip_addr, 4);

        current_arp_entry++;
    }

    uint8_t mac_get_from_ip(uint32_t ip)
    {

        const ArpTableEntry* const table = (ArpTableEntry*)ArpTable;

        for(int i = 0; i < ARP_TABLE_ENTRIES; i++)
        {
            const uint8_t* tmp = (uint8_t*)&table[i].ip;
            for(int j = 0; j < 4; j++)
                if(memcmp((uint8_t*)&ip, ArpTable[i].ip, 4))
                    return i;
        }
        
        return 0xFF;

    }

    ArpTableEntry last_arp_reply_get(void)
    {
        return LastArpReply;
    }

    bool arp_table_add_entry(uint32_t ip, uint8_t* mac)
    {
        for(int i = 0; i < ARP_TABLE_ENTRIES; i++)
        {
            if(ArpTable[i].ip) 
            {
                memcpy(ArpTable[i].ip, (uint8_t*)&ip, 4);
                memcpy(ArpTable[i].mac, mac, 6);
                ArpTable[i].success = true;
                current_arp_entry++;
                return true;
            }
        }
        return false;
    }

}