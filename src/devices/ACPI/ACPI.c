
#include <stdbool.h>
#include <devices/ACPI/ACPI.h>
#include <libc/string.h>
#include <libc/stdiox.h>
#include <libc/stdlibx.h>
#include <kernel/pmmngr/alloc.h>

uint32_t acpi_rdsp_base;
acpi_rsdt* rsdt;
uint8_t* madt_entries[0x10];
rsdp_descriptor* rsdp;
apic_sdt_entry* apic_sdt;

uint8_t madt_entry_type0_counter = 0;
uint8_t madt_entry_type1_counter = 0;
uint8_t madt_entry_type2_counter = 0;
uint8_t madt_entry_type3_counter = 0;
uint8_t madt_entry_type4_counter = 0;
uint8_t madt_entry_type5_counter = 0;
uint8_t madt_entry_type9_counter = 0;


const madt_entry_type0** madt_entry_type0_ptr;
const madt_entry_type1** madt_entry_type1_ptr;
const madt_entry_type2** madt_entry_type2_ptr;
const madt_entry_type3** madt_entry_type3_ptr;
const madt_entry_type4** madt_entry_type4_ptr;
const madt_entry_type5** madt_entry_type5_ptr;
const madt_entry_type9** madt_entry_type9_ptr;

bool acpi_rsdp_checksum_check(rsdp_descriptor *header)
{
    uint8_t sum = 0;
    uint8_t *field = (uint8_t *)header;

    for (int i = 0; i < 20; i++)
    {
        sum += field[i];
    }

    return sum == 0;
}

bool acpi_rsdt_checksum_check(acpi_rsdt *header)
{
    uint8_t* tmp = (uint8_t *)header;
    uint8_t sum = 0;

    for(int i = 0; i < header->length; i++)
    {
        sum += tmp[i];
    }

    return sum == 0;

}


rsdp_descriptor *get_acpi_rsdp_address_base(void)
{
    char *acpi_string = (char *)0xE000;

    while (acpi_string < (char *)0xFFFFF)
    {
        if (strncmp(acpi_string, "RSD PTR ", 8))
        {
            return (rsdp_descriptor *)acpi_string;
        }

        acpi_string += 0x10;
    }

    return (rsdp_descriptor*)NULL;

}

void acpi_print_rsdp(void)
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



void acpi_print_sdt(sdt *x)
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

apic_sdt_entry *apic_sdt_find(void)
{

    char *tmp = (uint8_t *)rsdt->pointer_to_sdt[0];

    while (1)
        if (strncmp(tmp, "APIC", 4))// && !strncmp(tmp, "APICx", 5))
            return (apic_sdt_entry *)tmp;
        else
            tmp++;
}

void madt_entries_get(apic_sdt_entry *apic_entry)
{

    madt_entry_type0_ptr = kmalloc(sizeof(madt_entry_type0) * 10);
    madt_entry_type1_ptr = kmalloc(sizeof(madt_entry_type1) * 10);
    madt_entry_type2_ptr = kmalloc(sizeof(madt_entry_type2) * 10);
    madt_entry_type3_ptr = kmalloc(sizeof(madt_entry_type3) * 10);
    madt_entry_type4_ptr = kmalloc(sizeof(madt_entry_type4) * 10);
    madt_entry_type5_ptr = kmalloc(sizeof(madt_entry_type5) * 10);
    madt_entry_type9_ptr = kmalloc(sizeof(madt_entry_type9) * 10);

    uint8_t *tmp = (uint8_t*)(apic_entry) + 0x2C;

    for (int i = 0; i < 0x10; tmp += *(tmp + 1), i++)
    {
        if (*tmp == 0)
            madt_entry_type0_ptr[madt_entry_type0_counter++] = (madt_entry_type0 *)tmp;

        else if (*tmp == 1)
            madt_entry_type1_ptr[madt_entry_type1_counter++] = (madt_entry_type1 *)tmp;

        else if (*tmp == 2)
            madt_entry_type2_ptr[madt_entry_type2_counter++] = (madt_entry_type2 *)tmp;

        else if (*tmp == 3)
            madt_entry_type3_ptr[madt_entry_type3_counter++] = (madt_entry_type3 *)tmp;

        else if (*tmp == 4)
            madt_entry_type4_ptr[madt_entry_type4_counter++] = (madt_entry_type4 *)tmp;

        else if (*tmp == 5)
            madt_entry_type5_ptr[madt_entry_type5_counter++] = (madt_entry_type5 *)tmp;

        else if (*tmp == 9)
            madt_entry_type9_ptr[madt_entry_type9_counter++] = (madt_entry_type9 *)tmp;
    }
}


uint8_t madt_checksum_check(apic_sdt_entry* entry)
{
    uint8_t* field = (uint8_t*)entry;
    uint8_t sum = 0;

    for(int i = 0; i < entry->length; i++)
        sum += field[i];
    

    return sum == 0;

}