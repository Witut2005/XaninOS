
#pragma once

#include <stdint.h>

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

    public:
    static void add_device(uint8_t*(*receive_ptr)(void), void(*send_ptr)(uint32_t, uint16_t), NetworkDevice* device)
    {
        auto tmp = device->NetworkHandlers;

        while(!tmp->is_device_present)
            tmp++;
        
        tmp->is_device_present = true;
        tmp->receive_ptr = receive_ptr;
        tmp->send_ptr = send_ptr;
    }

    static void send_packet(uint32_t buffer, uint16_t length, NetworkDevice* device)
    {

        auto tmp = device->NetworkHandlers;

        while(!tmp->is_device_present)
            tmp++;

        tmp->send_ptr(buffer, length);

    }



    static uint8_t* packet_receive(NetworkDevice* device)
    {    
        auto tmp = device->NetworkHandlers;

        while(!tmp->is_device_present)
            tmp++;

        return tmp->receive_ptr();

    }


};

inline NetworkHandler NetworkDevice::NetworkHandlers[10];
inline NetworkDevice NetworkSubsystem;
