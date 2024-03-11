
#include <sys/net/netapi/network_device.h>
#include <sys/devices/nic/8254x.hpp>
#include <lib/libcpp/initializer_list.hpp>
#include <lib/libcpp/utility.h>
#include <lib/libcpp/container/vector.hpp>

struct NetworkDeviceInitializer 
{
    bool(*probe)(const pci_device&) = nullptr;
    NetworkDevice*(*create)(const pci_device&) = nullptr;
};

class NetworkDevicesManager 
{
    public:
    static void initialize(void);

    NetworkDevicesManager(const NetworkDevicesManager&) = delete;
    NetworkDevicesManager& operator = (const NetworkDevicesManager&) = delete;

    private:

    static constexpr NetworkDeviceInitializer m_initializers[] = {
        {Intel8254xDriver::probe, Intel8254xDriver::create}
    };

    NetworkDevice* devices[0x10] = {nullptr};
};