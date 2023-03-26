
#include <netapi/network_device.hpp>

#define NETDEVICES_HANDLERS 100

static NetworkDevice* XaninNetworkDevices [NETDEVICES_HANDLERS];


extern "C" {
    
    bool netapi_add_devicecpp(NetworkDevice* NetDev)
    {
        for(int i = 0; i < NETDEVICES_HANDLERS; i++)
        {
            if(!XaninNetworkDevices[i])
            {
                XaninNetworkDevices[i] = NetDev;
                XaninNetworkDevices[i]->name_set(std::string("XaninNetworkDevice" + ('0' + i )));
                return true;
            }
        }
        return false;
    }

    bool netapi_packet_sendcpp(uint8_t* buffer, uint16_t length)
    {
        for(int i = 0; i < NETDEVICES_HANDLERS; i++)
        {
            if(!XaninNetworkDevices[i])
            {
                XaninNetworkDevices[i]->packet_send(buffer, length);
                return true;
            }
        }
        return false;

    }

    uint8_t* netapi_packet_receivecpp(void)
    {
        for(int i = 0; i < NETDEVICES_HANDLERS; i++)
        {
            if(!XaninNetworkDevices[i])
                return XaninNetworkDevices[i]->packet_receive();
        }
        return NULL;
    }

    uint8_t* netapi_mac_getcpp(void)
    {
        for(int i = 0; i < NETDEVICES_HANDLERS; i++)
        {
            if(!XaninNetworkDevices[i])
                return XaninNetworkDevices[i]->mac_get();
        }
        return NULL;
    }

    void netapi_interrupt_handle(void)
    {
        for(int i = 0; i < NETDEVICES_HANDLERS; i++)
        {
            if(!XaninNetworkDevices[i])
                XaninNetworkDevices[i]->interrupt_handler();
        }
    }


}