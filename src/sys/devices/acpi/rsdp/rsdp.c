
#include <sys/devices/acpi/rsdp/rsdp.h>
#include <stddef.h>
#include <lib/libc/string.h>



SystemAcpiRSDP* acpi_rsdp_find(void)
{
    char *acpi_string = (char *)0xE000;

    while (acpi_string < (char *)0xFFFFF)
    {
        if (strncmp(acpi_string, "RSD PTR ", 8))
        {
            return (SystemAcpiRSDP*)acpi_string;
        }

        acpi_string += 0x10;
    }

    return (SystemAcpiRSDP*)NULL;

}

bool acpi_rsdp_checksum_check(SystemAcpiRSDP* header)
{
    uint8_t sum = 0;
    uint8_t *field = (uint8_t *)header;

    for (int i = 0; i < 20; i++)
    {
        sum += field[i];
    }

    return sum == 0;
}

void acpi_print_rsdp(SystemAcpiRSDP* rsdp)
{
    xprintf("RDSP data: %x\n", rsdp);

    xprintf("signature: ");
    for (int i = 0; i < 8; i++)
        xprintf("%c", rsdp->signature[i]);

    xprintf("\n");

    xprintf("checksum: 0x%x\n", rsdp->checksum);
    xprintf("OMID: %s\n", rsdp->oemid);
    xprintf("revision: 0x%x\n", rsdp->revision);
    xprintf("rsdt base: 0x%x\n", rsdp->rsdt_address);
}


