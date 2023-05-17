
#include <sys/devices/acpi/rsdt/rsdt.h>

SystemAcpiRSDT* AcpiRSDT;

void acpi_rsdt_set(SystemAcpiRSDT* RSDTAddress)
{
    AcpiRSDT = RSDTAddress;
}

const SystemAcpiRSDT* const acpi_rsdt_get(void)
{
    return AcpiRSDT;
}

bool acpi_rsdt_checksum_check(SystemAcpiRSDT *header)
{
    uint8_t* tmp = (uint8_t *)header;
    uint8_t sum = 0;

    for(int i = 0; i < header->length; i++)
        sum += tmp[i];

    return sum == 0;
}