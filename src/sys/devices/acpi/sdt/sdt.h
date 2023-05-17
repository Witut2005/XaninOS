
#pragma once
#include <stdint.h>
#include <sys/devices/acpi/fadt/fadt.h>

struct SystemAcpiSDT
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

} __attribute__ ((packed));

typedef struct SystemAcpiSDT SystemAcpiSDT;

extern SystemAcpiSDT* ApicSDT;

void acpi_print_sdt(SystemAcpiSDT *x);
SystemAcpiSDT* acpi_apic_sdt_get(void);
void acpi_apic_sdt_set(SystemAcpiSDT *ApicSDTAddress);
SystemAcpiSDT* apic_sdt_find(void);
SystemAcpiFADT* acpi_fadt_find(void);