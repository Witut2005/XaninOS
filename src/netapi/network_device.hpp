
#pragma once

#include <stdint.h>
#include <libcpp/cstdio.h>

struct NetworkHandler
{
    bool is_device_present;
    uint8_t*(*receive_ptr)(void);
    void(*send_ptr)(uint32_t, uint16_t);
};


class NetworkDevice 
{
    private:
    static NetworkHandler NetworkHandlers[10];
    static uint8_t* device_mac[10];

    public:
    static void add_device(uint8_t*(*receive_ptr)(void), void(*send_ptr)(uint32_t, uint16_t), uint8_t*(*mac_get)(void), NetworkDevice* device)
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

        device_mac[new_device_id] = mac_get();
        

    }

    static void packet_send(uint32_t buffer, uint16_t length, NetworkDevice* device)
    {

        // xprintf("z");

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

        return device_mac[device_id];
    }


};

inline NetworkHandler NetworkDevice::NetworkHandlers[10];
inline uint8_t* NetworkDevice::device_mac[10];
inline NetworkDevice NetworkSubsystem;
