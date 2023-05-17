
#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <sys/devices/acpi/fadt/fadt.h>

struct SystemAcpiRSDT 
{
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oemid[6];
    char oem_table_id[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
    uint32_t pointer_to_sdt[0x10];
} __attribute__ ((packed));

typedef struct SystemAcpiRSDT SystemAcpiRSDT;

extern SystemAcpiRSDT* AcpiRSDT;
bool acpi_rsdt_checksum_check(SystemAcpiRSDT *header);

extern const SystemAcpiRSDT* const acpi_rsdt_get(void); 
void acpi_rsdt_set(SystemAcpiRSDT* RSDTAddress);