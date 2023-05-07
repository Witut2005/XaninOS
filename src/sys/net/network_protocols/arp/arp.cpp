
#include <sys/net/network_protocols/arp/arp.h>
#include <sys/net/network_protocols/ethernet_frame/ethernet_frame.hpp>
#include <lib/libc/stdlibx.h>
#include <lib/libcpp/endian.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/string.h>
#include <lib/libc/time.h>
#include <sys/net/netapi/loopback/loopback.h>
#include <sys/net/netapi/objects/ip.h>
#include <sys/net/netapi/objects/mac.hpp>
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
            ArpModule::PacketsInfo.insert_or_assign(LITTLE_ENDIAN(Arp->destination_protocol_address), Response);

        if(net::is_system_ip(LITTLE_ENDIAN(Arp->destination_protocol_address)))
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
        arp->hardware_type = BIG_ENDIAN(hardware_type);
        arp->protocol_type = BIG_ENDIAN(protocol_type);
        arp->hardware_address_length = hardware_address_length;
        arp->protocol_address_length = protocol_address_length;
        arp->opcode = BIG_ENDIAN(opcode);
        memcpy(arp->source_hardware_address, source_hardware_address, 6);
        arp->source_protocol_address = BIG_ENDIAN(source_protocol_address);
        memcpy(arp->destination_hardware_address, destination_hardware_address, 6);
        arp->destination_protocol_address = BIG_ENDIAN(destination_protocol_address);
        return arp;

    }

    void arp_loopback_reply(AddressResolutionProtocol* ArpHeader)
    {
        if(LITTLE_ENDIAN(ArpHeader->opcode) != ARP_GET_MAC)
            return;

        if(ArpModule::PacketsInfo.exists(LITTLE_ENDIAN(ArpHeader->destination_protocol_address)))
        {
            (*ArpModule::PacketsInfo.find(LITTLE_ENDIAN(ArpHeader->destination_protocol_address)))->success = true;
            memcpy((uint8_t*)(*ArpModule::PacketsInfo.find(LITTLE_ENDIAN(ArpHeader->destination_protocol_address)))->data, (uint8_t*)ArpHeader, sizeof(AddressResolutionProtocol));
        }
    }


    void arp_reply_handle(AddressResolutionProtocol* ArpHeader)
    {


        if(net::is_system_ip(LITTLE_ENDIAN(ArpHeader->destination_protocol_address)))
        {
            
            if(net::is_system_mac(ArpHeader->source_hardware_address))
                arp_loopback_reply(ArpHeader);
            
            else 
            {
                if(LITTLE_ENDIAN(ArpHeader->opcode) == ARP_REPLY)
                {
                    if(ArpModule::PacketsInfo.exists(LITTLE_ENDIAN(ArpHeader->source_protocol_address)))
                    {
                        ArpModule::PacketsInfo[LITTLE_ENDIAN(ArpHeader->source_protocol_address)]->success = true;
                        memcpy((uint8_t*)ArpModule::PacketsInfo[LITTLE_ENDIAN(ArpHeader->source_protocol_address)]->data, (uint8_t*)ArpHeader, sizeof(AddressResolutionProtocol));
                    }
                }

                else if(LITTLE_ENDIAN(ArpHeader->opcode) == ARP_GET_MAC)
                {
                    AddressResolutionProtocol XaninArpReply;
                    prepare_arp_request(&XaninArpReply, ARP_ETHERNET, ARP_IP_PROTOCOL, 0x6, 0x4, ARP_REPLY, netapi_mac_get(xanin_ip_get()), xanin_ip_get(), ArpHeader->source_hardware_address, ArpHeader->source_protocol_address);
                    send_arp(&XaninArpReply, NULL);
                }

            }

        }

        else if(LITTLE_ENDIAN(ArpHeader->opcode) != ARP_REPLY)//HANDLE ONLY REPLIES (NOT ARP PROBE ETC)
            return;

        uint32_t ip_addr = LITTLE_ENDIAN(ArpHeader->source_protocol_address);
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