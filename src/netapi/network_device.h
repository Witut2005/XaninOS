#pragma once

#include <stdint.h>


uint8_t* netapi_mac_get(void);
void netapi_send_packet(uint32_t address, uint16_t lenght);
uint8_t* netapi_receive_packet(void);
void netapi_init(void);
void netapi_add_device(uint8_t*(*receive_ptr)(void), void(*send_ptr)(uint32_t, uint16_t), uint8_t* mac_ptr, void(*interrupt_handler)(void));


struct NetworkHandler
{
    uint8_t is_device_present;
    uint8_t*(*receive_ptr)(void);
    void(*send_ptr)(uint32_t, uint16_t);
    uint8_t* device_mac;
    void(*interrupt_handler)(void);
};

typedef struct NetworkHandler NetworkHandler;