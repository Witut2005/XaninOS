#pragma once

#define PCI_ADDRESS_PORT 0xCF8
#define PCI_DATA_PORT 0xCFC


#define ENABLE_CONFIGURATION_SPACE_MAPPING 0x80000000


#include <stdint.h>

struct pci_device_structure0
{

    
    uint16_t vendor_id;
    uint16_t device_id;

    uint16_t command;
    uint16_t status;
    
    uint8_t revision_id;
    uint8_t p_interface;
    uint8_t subclass;
    uint8_t _class;
    
    uint8_t cache_line_size;
    uint8_t latency_timer;
    uint8_t header_type;
    uint8_t bist;
    
    uint32_t base0; 
    uint32_t base1;
    uint32_t base2;
    uint32_t base3;
    uint32_t base4;
    uint32_t base5;
    
    uint32_t card_bus;

    uint16_t subsystem_vendor_id;
    uint16_t subsystem_id;
    
    uint32_t rom_base;
    
    uint8_t capabilities_pointer;
    uint8_t reserved0[3];
    
    uint8_t reserved1[4];
    
    uint8_t interrupt_line;
    uint8_t interrupt_pin;
    uint8_t minimal_grant;
    uint8_t max_latency;

    uint8_t bus;
    uint8_t slot;
    uint8_t function;
    uint8_t offset;

}__attribute__((packed));
typedef struct pci_device_structure0 pci_device;
 
struct pci_device_structure1
{

    uint16_t vendor_id;
    uint16_t device_id;
    uint16_t command;
    uint16_t status;
    uint8_t revision_id;
    uint8_t p_interface;
    uint8_t subclass;
    uint8_t _class;
    uint8_t cache_line_size;
    uint8_t latency_timer;
    uint8_t header_type;
    uint8_t bist;
    uint32_t base0;
    uint32_t base1;
    uint8_t primary_bus_number;
    uint8_t secondary_bus_number;
    uint8_t subordinate_bus_number;
    uint8_t secondary_latency_timer;
    uint8_t io_base;
    uint8_t io_limit;
    uint16_t secondary_status;
    uint16_t memory_base;
    uint16_t memory_limit;
    uint16_t prefetchable_memory_base;
    uint16_t prefetchable_memory_limit;
    uint32_t prefetchable_base_upper_32_bits;
    uint32_t prefetchable_limit_upper_32_bits;
    uint16_t io_base_upper_16_bits;
    uint16_t io_limit_upper_16_bits;
    uint8_t capabilities_pointer;
    uint8_t reserved0[3];
    uint32_t rom_base; 
    uint8_t interrupt_line;
    uint8_t interrupt_pin;
    uint16_t bridge_control; 

}__attribute__((packed));

struct pci_power_management_register
{
    uint8_t power_state : 2;
    uint8_t reserved1 : 6;
    uint8_t pme_enable : 1;
    uint8_t data_select : 4;
    uint8_t data_scale : 2;
    uint8_t pme_status : 1;
    uint16_t reserved2;
}__attribute__((packed));

typedef struct pci_power_management_register pci_power_management_register;

struct pci_config_register
{

    uint8_t pci_bus_number;
    uint8_t pci_device_number;
    uint8_t pci_function_number;
    uint8_t pci_register_offset;

};

#ifndef __cplusplus
    typedef struct pci_config_register pci_config_register;
#endif
    
static pci_config_register pci_config_address;


enum PCI_DEVICES
{
   USB_CLASS_CONTROLLER_CODE = 0x0C,
   USB_SUBCLASS_CONTROLLER_CODE = 0x03,
 
   NETWORK_CLASS_CONTROLLER_CODE = 0x2,
   ETHERNET_SUBCLASS_CONTROLLER_CODE = 0x0
};


static char* pci_device_name[15] = 
{
    "unclassified",
    "mass storage controller",
    "network controller",
    "display controller",
    "multimedia controller",
    "memory controller",
    "bridge",
    "simple communication controller",
    "base system peripheral",
    "input decide controller",
    "docking station",
    "processor",
    "serial bus controller"

};


struct PciDevicePack 
{
    const char* name;
    const pci_device** const PciInfo;
    uint8_t length;
};

#ifndef __cplusplus
typedef struct PciDevicePack PciDevicePack;
#endif


#ifdef __cplusplus
extern "C" {
#endif

uint32_t pci_find_device(uint16_t, uint16_t, pci_device*);
//PciDevicePack pci_find_devices(uint16_t, uint16_t);
uint8_t pci_get_data8(const uint8_t bus, const uint8_t slot, const uint8_t function, const uint8_t offset);
uint16_t pci_get_data16(const uint8_t bus, const uint8_t slot, const uint8_t function, const uint8_t offset);

#ifdef __cplusplus
}
#endif

