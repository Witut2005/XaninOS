
#include <stdint.h>
#include <netapi/network_device.h>
#include <libc/stdiox.h>
#include <libc/stdlibx.h>
#include <libc/string.h>
#include <xin_fs/xin.h>
#include <network_protocols/internet_protocol/ipv4/ip.h>
    

NetworkHandler NetworkHandlers[NETWORK_CARDS_HANDLERS] = {0,0,0,0,0,0,0};

bool netapi_add_device(uint8_t*(*receive_ptr)(void), void(*send_ptr)(uint8_t*, uint16_t), uint8_t* mac_ptr, void(*interrupt_handler)(void), pci_device* pci_info)
{
    NetworkHandler* tmp = NetworkHandlers;
    int new_device_id = 0x0;

    while(tmp->is_device_present)
    {
        tmp++;
        new_device_id++;

        if(new_device_id == NETWORK_CARDS_HANDLERS)
            return false;

    }

    tmp->is_device_present = 1;
    tmp->receive_ptr = receive_ptr;
    tmp->send_ptr = send_ptr;
    tmp->device_mac = mac_ptr;
    tmp->pci_info = pci_info;
    tmp->device_name = (uint8_t*)calloc(sizeof(uint8_t) * 50);
   
    char* new_device_name = (char*)calloc(50);
    strcpy(new_device_name, "XaninNetworkDevice");
    new_device_name[strlen(new_device_name)] = (char)('0' + new_device_id);
    
    strcpy(tmp->device_name, new_device_name);

    // NetworkHandlersBackup[new_device_id].is_device_present = 1;
    // NetworkHandlersBackup[new_device_id].receive_ptr = receive_ptr;
    // NetworkHandlersBackup[new_device_id].send_ptr = send_ptr;
    // NetworkHandlersBackup[new_device_id].device_mac = mac_ptr;

    free(new_device_name);
    return true;
    

}

bool network_device_available_check(void)
{
    NetworkHandler* tmp = NetworkHandlers;
    uint8_t device_id = 0;

    while(!tmp->is_device_present)
    {
        tmp++;
        device_id++;
        if(device_id == NETWORK_CARDS_HANDLERS)
            return false;
    }

    return true;
}

void netapi_packet_send(uint8_t* buffer, uint16_t length)
{

    NetworkHandler* tmp = NetworkHandlers;
    int device_id = 0x0;

    while(tmp->is_device_present != true)
    {
        tmp++;
        device_id++;
    }


    // if(NetworkHandlers[device_id].send_ptr != NetworkHandlersBackup[device_id].send_ptr)
    //     NetworkHandlers[device_id].send_ptr = NetworkHandlersBackup[device_id].send_ptr;
    // xprintf("\n0x%x\n", tmp->is_device_present);
    // xprintf("0x%x\n", tmp->receive_ptr);
    // xprintf("0x%x\n", tmp->send_ptr);
    // xprintf("0x%x\n", tmp->device_mac);
    
    tmp->send_ptr(buffer, length);


}


uint8_t* netapi_packet_receive(void)
{    
    NetworkHandler* tmp = NetworkHandlers;

    while(!tmp->is_device_present)
        tmp++;

    // if(NetworkHandlers[device_id].device_mac != NetworkHandlersBackup[device_id].send_ptr)
    //     NetworkHandlers[device_id].send_ptr = NetworkHandlersBackup[device_id].send_ptr;

    return tmp->receive_ptr();

}

uint8_t* netapi_mac_get(void)
{

    NetworkHandler* tmp = NetworkHandlers;
    int device_id = 0x0;

    while(!tmp->is_device_present)
    {
        tmp++;
        device_id++;
    }

    // if(NetworkHandlers[device_id].device_mac != NetworkHandlersBackup[device_id].send_ptr)
    //     NetworkHandlers[device_id].send_ptr = NetworkHandlersBackup[device_id].send_ptr;

    return tmp->device_mac;
}                                                                        

void network_handler(void)
{
    NetworkHandler* tmp = NetworkHandlers;

    while(!tmp->is_device_present)
    {
        tmp++;
    }

    return tmp->interrupt_handler();

}

NetworkHandler* device_info_get_by_name(char* device_name)
{
    if(!strlen(device_name))
        return NULL;

    NetworkHandler* tmp = NetworkHandlers;
    while(!strcmp(tmp->device_name, device_name) )
    {
        if(tmp == &NetworkHandlers[NETWORK_CARDS_HANDLERS])
            return NULL;
        tmp++;
    }

    return tmp;

}


uint32_t xanin_ip_get(void)
{
    XinEntry* nic_ip = fopen("/config/nic.conf", "rw");

    char* ip_str = (char*)calloc(64);
    fread(nic_ip, ip_str, 15);

    uint32_t ip = str2ipv4(ip_str);
    free(ip_str);

    if(!ip)
        return ((192 << 24) | (168 << 16) | (19 << 8) | (12));

    return ip;
}
