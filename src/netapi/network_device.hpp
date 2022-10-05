
#pragma once

// #include <stdint.h>
// #include <libcpp/cstdio.h>
#include <devices/PCI/pci.h>



extern "C" void netapi_add_device(uint8_t*(*receive_ptr)(void), void(*send_ptr)(uint32_t, uint16_t), uint8_t* mac_ptr, void(*interrupt_handler)(void), pci_device* pci_info);
extern "C" void netapi_packet_send(uint32_t buffer, uint16_t length);
extern "C" uint8_t* netapi_packet_receive(void);
extern "C" uint8_t* netapi_mac_get(void);
extern "C" void netapi_init(void);
