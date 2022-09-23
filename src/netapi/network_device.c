
#include <stdint.h>
#include <netapi/network_device.h>
#include <libc/stdiox.h>
#include <libc/stdlibx.h>
    
static NetworkHandler NetworkHandlersBackup[10];
static NetworkHandler NetworkHandlers[10];

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

void netapi_add_device(uint8_t*(*receive_ptr)(void), void(*send_ptr)(uint32_t, uint16_t), uint8_t* mac_ptr, void(*interrupt_handler)(void))
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

    NetworkHandlersBackup[new_device_id].is_device_present = 1;
    NetworkHandlersBackup[new_device_id].receive_ptr = receive_ptr;
    NetworkHandlersBackup[new_device_id].send_ptr = send_ptr;
    NetworkHandlersBackup[new_device_id].device_mac = mac_ptr;

    

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

    // uint8_t* mac_get(void)
    // {
    //     return NetworkDevice::mac_get(&NetworkSubsystem);
    // }

    // void send_packet(uint32_t address, uint16_t length)
    // {
    //     return NetworkDevice::packet_send(address, length, &NetworkSubsystem);
    // }

    // uint8_t* receive_packet(void)
    // {
    //     return NetworkDevice::packet_receive(&NetworkSubsystem);
    // }

