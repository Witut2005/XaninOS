
#include <sys/devices/acpi/sdt/sdt.h>
#include <sys/devices/acpi/rsdt/rsdt.h>
#include <lib/libc/stdiox.h>
#include <stddef.h>

SystemAcpiSDT* ApicSDT;

void acpi_print_sdt(SystemAcpiSDT* x)
{
    xprintf("singature:       : ");

    for (int i = 0; i < 4; i++)
        xprintf("%c", x->signature[i]);

    xprintf("\n");

    xprintf("length           : 0x%x\n", x->length);
    xprintf("revision         : 0x%x\n", x->revision);
    xprintf("checksum         : 0x%x\n", x->checksum);

    xprintf("oemid       : ");
    for (int i = 0; i < 6; i++)
        xprintf("%c", x->oemid[i]);
    xprintf("\n");

    xprintf("oem table id: ");
    for (int i = 0; i < 8; i++)
        xprintf("%c", x->oem_table_id[i]);
    xprintf("\n");

    xprintf("oem_revision     : 0x%x\n", x->oem_revision);
    xprintf("creator_id       : 0x%x\n", x->creator_id);
    xprintf("creator_revision : 0x%x\n", x->creator_revision);
}


bool acpi_sdt_checksum_check(uint8_t* header, uint32_t length)
{
    uint8_t sum = 0;

    for (int i = 0; i < length; i++)
        sum += header[i];

    return sum == 0;
}