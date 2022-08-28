
#pragma once

#include <stdint.h>
#include <libcpp/cstdio.h>

struct NetworkHandler
{
    bool is_device_present;
    uint8_t*(*receive_ptr)(void);
    void(*send_ptr)(uint32_t, uint16_t);
    uint8_t* device_mac;
};


class NetworkDevice 
{
    private:
    static NetworkHandler NetworkHandlers[10];

    public:
    static void add_device(uint8_t*(*receive_ptr)(void), void(*send_ptr)(uint32_t, uint16_t), uint8_t* mac_ptr, NetworkDevice* device)
    {
        NetworkHandler* tmp = device->NetworkHandlers;

        auto new_device_id = 0x0;

        while(!tmp->is_device_present)
        {
            tmp++;
            new_device_id++;
        }

        tmp->is_device_present = true;
        tmp->receive_ptr = receive_ptr;
        tmp->send_ptr = send_ptr;

        tmp->device_mac = mac_ptr;
        

    }

    static void packet_send(uint32_t buffer, uint16_t length, NetworkDevice* device)
    {

        NetworkHandler* tmp = device->NetworkHandlers;

        while(!tmp->is_device_present)
            tmp++;

        tmp->send_ptr(buffer, length);

    }



    static uint8_t* packet_receive(NetworkDevice* device)
    {    
        NetworkHandler* tmp = device->NetworkHandlers;

        while(!tmp->is_device_present)
            tmp++;

        return tmp->receive_ptr();

    }

    static uint8_t* mac_get(NetworkDevice* device)
    {

        NetworkHandler* tmp = device->NetworkHandlers;
        auto device_id = 0x0;

        while(!tmp->is_device_present)
        {
            tmp++;
            device_id++;
        }

        return tmp->device_mac;
    }


};

inline NetworkHandler NetworkDevice::NetworkHandlers[10];
inline NetworkDevice NetworkSubsystem;
