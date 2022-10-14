
#include <stdint.h>
#include <netapi/network_device.h>
#include <libc/stdiox.h>
#include <libc/stdlibx.h>
#include <libc/string.h>
    
NetworkHandler NetworkHandlersBackup[10];
NetworkHandler NetworkHandlers[10];

void netapi_init(void)
{
    for(int i = 0; i < 10; i++)
    {
        NetworkHandlers[i].is_device_present = 0x0;
        NetworkHandlers[i].device_mac = 0x0;
        NetworkHandlers[i].send_ptr = 0x0;
        NetworkHandlers[i].receive_ptr = 0x0;
    }
}

void netapi_add_device(uint8_t*(*receive_ptr)(void), void(*send_ptr)(uint32_t, uint16_t), uint8_t* mac_ptr, void(*interrupt_handler)(void), pci_device* pci_info)
{
    NetworkHandler* tmp = NetworkHandlers;
    int new_device_id = 0x0;

    while(tmp->is_device_present)
    {
        tmp++;
        new_device_id++;
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

    NetworkHandlersBackup[new_device_id].is_device_present = 1;
    NetworkHandlersBackup[new_device_id].receive_ptr = receive_ptr;
    NetworkHandlersBackup[new_device_id].send_ptr = send_ptr;
    NetworkHandlersBackup[new_device_id].device_mac = mac_ptr;

    free(new_device_name);
    

}

void netapi_packet_send(uint32_t buffer, uint16_t length)
{

    NetworkHandler* tmp = NetworkHandlers;
    int device_id = 0x0;

    while(tmp->is_device_present != 1)
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
    NetworkHandler* tmp = NetworkHandlers;
    while(!strcmp(tmp->device_name, device_name) && tmp < &NetworkHandlers[10])
    {
        tmp++;
    }

    if(tmp == &NetworkHandlers[10])
        return nullptr;
    else
        return tmp;

}
