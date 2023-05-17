
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

SystemAcpiSDT* apic_sdt_find(void)
{
    uint32_t* sdt_pointers = acpi_rsdt_get()->pointer_to_sdt;

    for(int i = 0; i < 10; i++)
    {
        if(strncmp(sdt_pointers[i], "APIC", 4))
            return sdt_pointers[i];
    }
    return NULL;
}

SystemAcpiFADT* acpi_fadt_find(void)
{
    uint32_t* sdt_pointers = acpi_rsdt_get()->pointer_to_sdt;

    for(int i = 0; i < 10; i++)
    {
        if(strncmp(sdt_pointers[i], "FACP", 4))
            return sdt_pointers[i];
    }
    return NULL;
}

SystemAcpiSDT* acpi_apic_sdt_get(void)
{
    return ApicSDT;
}

void acpi_apic_sdt_set(SystemAcpiSDT* ApicSDTAddress)
{
    ApicSDT = ApicSDTAddress;
}