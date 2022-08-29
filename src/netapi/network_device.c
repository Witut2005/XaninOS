
#include <stdint.h>
#include <netapi/network_device.h>
#include <libc/stdiox.h>
    
NetworkHandler NetworkHandlers[10];

void netapi_add_device(uint8_t*(*receive_ptr)(void), void(*send_ptr)(uint32_t, uint16_t), uint8_t* mac_ptr)
{
    NetworkHandler* tmp = NetworkHandlers;
    int new_device_id = 0x0;

    while(!tmp->is_device_present)
    {
        tmp++;
        new_device_id++;
    }

    tmp->is_device_present = 1;
    tmp->receive_ptr = receive_ptr;
    tmp->send_ptr = send_ptr;

    tmp->device_mac = mac_ptr;
    

}

void netapi_packet_send(uint32_t buffer, uint16_t length)
{

    NetworkHandler* tmp = NetworkHandlers;
    int device_id = 0x0;

    while(!tmp->is_device_present)
    {
        tmp++;
        device_id++;
    }

    // if(device_id == 0 and MainNetworkHandler != nullptr)
    //     MainNetworkHandler->send_ptr(buffer, length);
    // else
    // xprintf(" 0x%x", tmp->send_ptr);
    tmp->send_ptr(buffer, length);


}


uint8_t* netapi_packet_receive(void)
{    
    NetworkHandler* tmp = NetworkHandlers;

    while(!tmp->is_device_present)
        tmp++;

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

    return tmp->device_mac;
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

