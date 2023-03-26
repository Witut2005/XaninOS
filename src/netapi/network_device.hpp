
#pragma once

#include <libcpp/string.h>
#include <devices/PCI/pci.h>

class NetworkDevice
{
    public:
    virtual uint8_t* mac_get(void) = 0;
    virtual void interrupt_handler(void) = 0;
    virtual pci_device* pci_info_get(void)= 0;

    virtual void name_set(std::string const& name) = 0;
    virtual std::string name_get(void) const = 0;

    virtual uint8_t* packet_receive(void) = 0;
    virtual void packet_send(uint8_t*, uint16_t) = 0;
};

extern "C"
{
    bool netapi_add_devicecpp(NetworkDevice* NetDev);
    bool netapi_packet_sendcpp(uint8_t* buffer, uint16_t length);
    uint8_t* netapi_packet_receivecpp(void);
    uint8_t* netapi_mac_getcpp(void);
    uint8_t* netapi_mac_getcpp(void);
    void netapi_interrupt_handle(void);
}