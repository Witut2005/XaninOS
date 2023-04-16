
#pragma once

#include <stdint.h>
#include <devices/PCI/pci.h>
#include <stdbool.h>
#include <stddef.h>
#include <netapi/network_device.h>

class LoopbackNetworkDevice 
{
    public:

    bool is_device_present(void) const; 
    uint8_t* mac_get(void);
    const char* name_get(void) const;
    uint32_t ip_get(void) const;
    void packet_send(uint8_t*, uint16_t);
};

extern LoopbackNetworkDevice XaninNetworkLoopback;