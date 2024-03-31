#pragma once

#include <stdint.h>
#include <sys/devices/pci/pci.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
class NetworkDevice
{
private:
    bool(*interrupt_handler)(void);

public:

    static inline bool probe(const pci_device&) { return false; }
    static inline NetworkDevice* create(const pci_device&) { return NULL; }

    inline bool handle_interrupt(void)
    {
        return this->interrupt_handler();
    }

    inline void interrupt_handler_set(bool(*handler)(void))
    {
        this->interrupt_handler = handler;
    }

    virtual bool is_device_present(void) const = 0;
    virtual uint8_t* mac_get(void) = 0;
    virtual pci_device* pci_info_get(void) = 0;

    virtual void name_set(const char* name) = 0;
    virtual const char* name_get(void) const = 0;

    virtual uint8_t* packet_receive(void) = 0;
    virtual void packet_send(uint8_t*, uint16_t) = 0;
};

extern "C" {
#endif
    bool netapi_packet_send(uint8_t* buffer, uint16_t length);
    uint8_t* netapi_packet_receive(void);
    uint8_t* netapi_mac_get(uint32_t nic_ip);
    uint8_t* netapi_device_mac_get(const char* device_name);
    void netapi_interrupt_handle(void);
    pci_device* netapi_device_info_get(const char* device_name);
    PciDevicePack* netapi_all_cards_info_get(PciDevicePack* Pack);
    int netapi_rename_device(char* old_name, char* new_name);
    uint32_t xanin_ip_get(void);
    bool network_device_available_check(void);
    void netapi_interrupt_handle_entry(void);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
NetworkDevice* netapi_find_available_device(void);
bool netapi_add_device(NetworkDevice* NetDev, const char* name, bool(*interrupt_handler_entry)(void));
#endif
