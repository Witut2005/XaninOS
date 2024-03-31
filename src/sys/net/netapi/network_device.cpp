
#include <sys/net/netapi/network_device.h>
#include <sys/net/netapi/loopback/loopback.h>
#include <lib/libc/file.h>
#include <fs/xin.h>
#include <lib/libcpp/ostream.h>
#include <lib/libc/string.h>
#include <lib/libc/hal.h>

#define NETDEVICES_HANDLERS 100

static NetworkDevice* XaninNetworkDevices[NETDEVICES_HANDLERS];

NetworkDevice* netapi_find_available_device(void)
{
    for (int i = 0; i < NETDEVICES_HANDLERS; i++)
    {
        if (XaninNetworkDevices[i])
        {
            if (XaninNetworkDevices[i]->is_device_present())
                return XaninNetworkDevices[i];
        }
    }
    return (NetworkDevice*)NULL;
}

bool netapi_add_device(NetworkDevice* NetDev, const char* name, bool(*interrupt_handler_entry)(void)) // add new to device to system :0
{
    for (int i = 0; i < NETDEVICES_HANDLERS; i++)
    {
        if (!XaninNetworkDevices[i])
        {
            XaninNetworkDevices[i] = NetDev;
            XaninNetworkDevices[i]->name_set(name);
            XaninNetworkDevices[i]->interrupt_handler_set(interrupt_handler_entry);
            return true;
        }
    }
    return false;
}

extern "C" {

    bool netapi_packet_send(uint8_t* buffer, uint16_t length) // send data to the network :))
    {
        for (int i = 0; i < NETDEVICES_HANDLERS; i++)
        {
            if (XaninNetworkDevices[i])
            {
                XaninNetworkDevices[i]->packet_send(buffer, length);
                return true;
            }
        }
        return false;

    }

    uint8_t* netapi_packet_receive(void) // receive incoming packet
    {
        for (int i = 0; i < NETDEVICES_HANDLERS; i++)
        {
            if (XaninNetworkDevices[i])
                return XaninNetworkDevices[i]->packet_receive();
        }
        return NULL;
    }

    uint8_t* netapi_mac_get(uint32_t nic_ip) // get XaninOS main network card MAC
    {

        if (nic_ip == XaninNetworkLoopback.ip_get())
            return XaninNetworkLoopback.mac_get();

        for (int i = 0; i < NETDEVICES_HANDLERS; i++)
        {
            if (XaninNetworkDevices[i])
                return XaninNetworkDevices[i]->mac_get();
        }
        return NULL;
    }

    void netapi_interrupt_handle(void) // netapi interrupt handler
    {
        interrupt_disable();
        for (int i = 0; i < NETDEVICES_HANDLERS; i++)
        {
            if (XaninNetworkDevices[i])
            {
                bool status = XaninNetworkDevices[i]->handle_interrupt();
            }
        }
        interrupt_enable();
    }

    pci_device* netapi_device_info_get(const char* device_name) // get network card pci info
    {
        for (int i = 0; i < NETDEVICES_HANDLERS; i++)
        {
            if (XaninNetworkDevices[i])
            {
                if (bstrcmp(const_cast<char*>(XaninNetworkDevices[i]->name_get()), device_name))
                    return XaninNetworkDevices[i]->pci_info_get();
            }
        }

        return NULL;

    }

    PciDevicePack* netapi_all_cards_info_get(PciDevicePack* Pack) // get all network cards pci information
    {
        Pack->length = 0;

        for (int i = 0; i < NETDEVICES_HANDLERS; i++)
        {
            if (XaninNetworkDevices[i])
            {
                Pack->name = XaninNetworkDevices[i]->name_get();
                Pack->PciInfo[Pack->length++] = XaninNetworkDevices[i]->pci_info_get();
            }
        }

        return Pack;

    }

    uint32_t xanin_ip_get(void) // get XaninOS IP from file
    {
        XinEntry* nic_ip = fopen("/config/nic.conf", "r");

        char* ip_str = (char*)calloc(64);

        fread(nic_ip, (uint8_t*)ip_str, 15);
        fclose(&nic_ip);

        uint32_t ip = str2ipv4(ip_str);
        free(ip_str);

        if (!ip)
            return ((192 << 24) | (168 << 16) | (19 << 8) | (12));

        return ip;
    }

    bool network_device_available_check(void) // check if any network device is available
    {
        for (int i = 0; i < NETDEVICES_HANDLERS; i++)
        {
            if (XaninNetworkDevices[i])
            {
                if (XaninNetworkDevices[i]->is_device_present())
                    return true;
            }
        }
        return false;
    }

    uint8_t* netapi_device_mac_get(const char* device_name)
    {
        for (int i = 0; i < NETDEVICES_HANDLERS; i++)
        {
            if (XaninNetworkDevices[i])
            {
                if (bstrcmp(const_cast<char*>(XaninNetworkDevices[i]->name_get()), device_name))
                    return XaninNetworkDevices[i]->mac_get();
            }
        }

        return NULL;
    }

    int netapi_rename_device(char* old_name, char* new_name)
    {
        for (int i = 0; i < NETDEVICES_HANDLERS; i++)
        {
            if (XaninNetworkDevices[i])
            {
                if (bstrcmp(const_cast<char*>(XaninNetworkDevices[i]->name_get()), old_name))
                {
                    XaninNetworkDevices[i]->name_set(new_name);
                    return XANIN_OK;
                }
            }
        }

        return XANIN_ERROR;
    }
}