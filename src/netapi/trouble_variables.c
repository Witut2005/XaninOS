
#include <stdint.h>
struct NetworkHandler
{
    uint8_t is_device_present;
    uint8_t*(*receive_ptr)(void);
    void(*send_ptr)(uint32_t, uint16_t);
    uint8_t* device_mac;
};

struct NetworkHandler NetworkHandlers[10];
struct NetworkHandler* MainNetworkHandler;

