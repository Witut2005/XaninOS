
#pragma once

// #include <stdint.h>
// #include <libcpp/cstdio.h>

struct NetworkHandler
{
    uint8_t is_device_present;
    uint8_t*(*receive_ptr)(void);
    void(*send_ptr)(uint32_t, uint16_t);
    uint8_t* device_mac;
    void(*interrupt_handler)(void);
};


extern "C" void netapi_add_device(uint8_t*(*receive_ptr)(void), void(*send_ptr)(uint32_t, uint16_t), uint8_t* mac_ptr, void(*interrupt_handler)(void));
extern "C" void netapi_packet_send(uint32_t buffer, uint16_t length);
extern "C" uint8_t* netapi_packet_receive(void);
extern "C" uint8_t* netapi_mac_get(void);
extern "C" void netapi_init(void);


// static NetworkHandler NetworkHandlers[10];
// // extern "C" NetworkHandler* MainNetworkHandler;


// class NetworkDevice 
// {
//     private:

//     public:
//     static void add_device(uint8_t*(*receive_ptr)(void), void(*send_ptr)(uint32_t, uint16_t), uint8_t* mac_ptr, NetworkDevice* device)
//     {
//         NetworkHandler* tmp = NetworkHandlers;

//         auto new_device_id = 0x0;

//         while(!tmp->is_device_present)
//         {
//             tmp++;
//             new_device_id++;
//         }

//         // if(new_device_id == 0)
//         //     MainNetworkHandler = NetworkHandlers;

//         tmp->is_device_present = true;
//         tmp->receive_ptr = receive_ptr;
//         tmp->send_ptr = send_ptr;

//         tmp->device_mac = mac_ptr;
        

//     }

//     static void packet_send(uint32_t buffer, uint16_t length, NetworkDevice* device)
//     {
//         xprintf("d");

//         NetworkHandler* tmp = NetworkHandlers;
//         auto device_id = 0x0;

//         while(!tmp->is_device_present)
//         {
//             tmp++;
//             device_id++;
//         }

//         // if(device_id == 0 and MainNetworkHandler != nullptr)
//         //     MainNetworkHandler->send_ptr(buffer, length);
//         // else
//         xprintf(" 0x%x", tmp->send_ptr);
//         tmp->send_ptr(buffer, length);


//     }



//     static uint8_t* packet_receive(NetworkDevice* device)
//     {    
//         NetworkHandler* tmp = NetworkHandlers;

//         while(!tmp->is_device_present)
//             tmp++;

//         return tmp->receive_ptr();

//     }

//     static uint8_t* mac_get(NetworkDevice* device)
//     {

//         NetworkHandler* tmp = NetworkHandlers;
//         auto device_id = 0x0;

//         while(!tmp->is_device_present)
//         {
//             tmp++;
//             device_id++;
//         }

//         return tmp->device_mac;
//     }


// };

// inline NetworkDevice NetworkSubsystem;
