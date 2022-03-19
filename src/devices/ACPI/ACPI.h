
#include <stdint.h>


struct rsdp_descriptor1
{

    uint8_t revision;
    uint8_t checksum;
    
    uint8_t signature[8];
    uint8_t OEMID[6];
    
    uint32_t rsdt_address;

} __attribute__ ((packed))rsdp;

