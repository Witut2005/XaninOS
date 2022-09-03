#pragma once

#include <stdint.h>

extern uint8_t* netapi_mac_get(void);
extern void netapi_send_packet(uint32_t address, uint16_t lenght);
extern uint8_t* netapi_receive_packet(void);
extern void netapi_init(void);

struct NetworkHandler
{
    uint8_t is_device_present;
    uint8_t*(*receive_ptr)(void);
    void(*send_ptr)(uint32_t, uint16_t);
    uint8_t* device_mac;
};

typedef struct NetworkHandler NetworkHandler;