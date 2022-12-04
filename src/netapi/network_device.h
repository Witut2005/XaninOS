#pragma once

#include <stdint.h>
#include <devices/PCI/pci.h>

struct NetworkHandler
{
    uint8_t is_device_present;
    uint8_t*(*receive_ptr)(void);
    void(*send_ptr)(uint32_t, uint16_t);
    uint8_t* device_mac;
    void(*interrupt_handler)(void);
    pci_device* pci_info;
    char* device_name;
};

#ifndef __cplusplus
typedef struct NetworkHandler NetworkHandler;
#endif

#ifdef __cplusplus 
extern "C" {
#endif

void netapi_init(void);
void netapi_add_device(uint8_t*(*receive_ptr)(void), void(*send_ptr)(uint32_t, uint16_t), uint8_t* mac_ptr, void(*interrupt_handler)(void), pci_device* pci_info);
void netapi_packet_send(uint32_t buffer, uint16_t length);
uint8_t* netapi_packet_receive(void);
uint8_t* netapi_mac_get(void);
NetworkHandler* device_info_get_by_name(char* device_name);
uint32_t xanin_ip_get(void);


#ifdef __cplusplus 
}
#endif

extern NetworkHandler NetworkHandlers[10];
