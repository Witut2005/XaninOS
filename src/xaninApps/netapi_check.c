
#include <network_protocols/icmp/icmp.h>
#include <network_protocols/arp/arp.h>
#include <network_protocols/internet_protocol/ipv4/ip.h>
#include <netapi/objects/response.h>
#include <netapi/network_device.h>
#include <libc/stdlibx.h>

int netapi_check(char* protocol)
{
    
    NetworkResponse* Response;

    if(strcmp(protocol, "ARP"))
    {
        response_object_create(&Response, sizeof(AddressResolutionProtocol));
        AddressResolutionProtocol* ArpPacket = (AddressResolutionProtocol*)calloc(sizeof(AddressResolutionProtocol));
        prepare_arp_request(ArpPacket, ARP_ETHERNET, ARP_IP_PROTOCOL, ARP_MAC_LENGTH, ARP_IP_LENGTH, ARP_GET_MAC, netapi_mac_get(xanin_ip_get()), xanin_ip_get(), mac_broadcast, (192 << 24) | (168 << 16) | (0 << 8) | (160));
        // prepare_arp_request(ArpPacket, ARP_ETHERNET, ARP_IP_PROTOCOL, ARP_MAC_LENGTH, ARP_IP_LENGTH, ARP_GET_MAC, netapi_mac_get(xanin_ip_get()), xanin_ip_get(), mac_broadcast, LOOPBACK_IP_ADDRESS);
        
        for(int i = 0; i < 5; i++)
        {
            if(Response->success)
                break;
            xprintf("0x%x\n", Response->data);
            send_arp_request(ArpPacket, Response); 
            NETWORK_RESPONSE_WAIT(500);
        }
    }

    else if(strcmp(protocol, "ICMP"))
    {
        response_object_create(&Response, sizeof(IcmpPacket));
        icmp_ping((127 << 24) | (1), Response);
        NETWORK_RESPONSE_WAIT(2000);
    }

    else
    {
        xprintf("Unknown protocol: %s\n", protocol);
        while(inputg().scan_code != ENTER);
        return XANIN_OK;
    }


    if(Response->success)
        xprintf("%zprotocol works!\n", OUTPUT_COLOR_SET(black, green));
    else
        xprintf("%zprotocol doesnt works!\n", OUTPUT_COLOR_SET(black, red));

    response_object_destroy(&Response);
    
    while(inputg().scan_code != ENTER);
    return XANIN_OK;

}
