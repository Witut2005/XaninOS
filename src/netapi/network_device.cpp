
#include <netapi/network_device.h>
#include <xin_fs/xin.h>
#include <libc/string.h>

#define NETDEVICES_HANDLERS 100

static NetworkDevice* XaninNetworkDevices [NETDEVICES_HANDLERS];


    
bool netapi_add_device(NetworkDevice* NetDev)
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

extern "C" {

    bool netapi_packet_send(uint8_t* buffer, uint16_t length)
    {
        for(int i = 0; i < NETDEVICES_HANDLERS; i++)
        {
            if(XaninNetworkDevices[i])
            {
                XaninNetworkDevices[i]->packet_send(buffer, length);
                return true;
            }
        }
        return false;

    }

    uint8_t* netapi_packet_receive(void)
    {
        for(int i = 0; i < NETDEVICES_HANDLERS; i++)
        {
            if(XaninNetworkDevices[i])
                return XaninNetworkDevices[i]->packet_receive();
        }
        return NULL;
    }

    uint8_t* netapi_mac_get(void)
    {
        for(int i = 0; i < NETDEVICES_HANDLERS; i++)
        {
            if(XaninNetworkDevices[i])
                return XaninNetworkDevices[i]->mac_get();
        }
        return NULL;
    }

    void netapi_interrupt_handle(void)
    {
        for(int i = 0; i < NETDEVICES_HANDLERS; i++)
        {
            if(XaninNetworkDevices[i])
                XaninNetworkDevices[i]->interrupt_handler();
        }
    }

    uint32_t xanin_ip_get(void)
    {
        XinEntry* nic_ip = xin::fopen("/config/nic.conf", "rw");

        char* ip_str     = (char*)calloc(64);
        xin::fread(nic_ip, ip_str, 15);

        xin::fclose(&nic_ip);

        uint32_t ip      = str2ipv4(ip_str);
        free(ip_str);

        if(!ip)
            return ((192 << 24) | (168 << 16) | (19 << 8) | (12));

        return ip;
    }

    bool network_device_available_check(void)
    {
    }


}