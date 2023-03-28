#pragma once

#include <stdint.h>
#include <devices/PCI/pci.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <libcpp/string.h>
class NetworkDevice
{
    public:
    virtual bool is_device_present(void) const = 0;
    virtual uint8_t* mac_get(void) = 0;
    virtual void interrupt_handler(void) = 0;
    virtual pci_device* pci_info_get(void)= 0;

    virtual void name_set(std::string const& name) = 0;
    virtual std::string name_get(void) const = 0;

    virtual uint8_t* packet_receive(void) = 0;
    virtual void packet_send(uint8_t*, uint16_t) = 0;
};


extern "C"{
#endif
    bool netapi_packet_send(uint8_t* buffer, uint16_t length);
    uint8_t* netapi_packet_receive(void);
    uint8_t* netapi_mac_get(void);
    void netapi_interrupt_handle(void);
    uint32_t xanin_ip_get(void);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
bool netapi_add_device(NetworkDevice* NetDev);
#endif