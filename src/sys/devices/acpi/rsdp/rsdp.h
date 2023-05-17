
#pragma once
#include <stdint.h>
#include <stdbool.h>

/*
    RSDP has pointer to RSDT structure, so first we need to find 
    RSDP to parse RSDT (pretty meaningless)
*/

struct SystemAcpiRSDP
{

    uint8_t signature[8];
    uint8_t checksum;
    uint8_t oemid[6];
    
    uint8_t revision;
    uint32_t rsdt_address;

} __attribute__ ((packed));

typedef struct SystemAcpiRSDP SystemAcpiRSDP;
// extern SystemAcpiRSDP* rsdp;

bool acpi_rsdp_checksum_check(SystemAcpiRSDP* header);
SystemAcpiRSDP* acpi_rsdp_address_base_get(void);
void acpi_print_rsdp(void);

SystemAcpiRSDP acpi_rsdp_copy_get(void);
SystemAcpiRSDP* acpi_rsdp_reference_get(void);

void acpi_rsdp_set(SystemAcpiRSDP* RSDPAddress);
