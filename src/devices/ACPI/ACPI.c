
#include <devices/ACPI/ACPI.h>

uint32_t acpi_rdsp_base;

uint32_t acpi_rsdp_checksum_check(rsdp_descriptor *header)
{
    uint8_t sum = 0;
    uint8_t *field = (uint8_t *)header;

    for (int i = 0; i < 20; i++)
    {
        sum += field[i];
        i++;
    }

    return sum;
}

rsdp_descriptor *get_acpi_rsdp_address_base(void)
{
    char *acpi_string = (char *)0xE000;

    while (acpi_string < (char *)0xFFFFF)
    {
        if (strncmp(acpi_string, "RSD PTR ", 8) && acpi_rsdp_checksum_check((rsdp_descriptor*)acpi_string) == 0)
        {
            return (rsdp_descriptor *)acpi_string;
        }

        acpi_string += 0x10;
    }

    return 0xFFFFFFFF;

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

    xprintf("rsdt oemid       : ");
    for (int i = 0; i < 6; i++)
        xprintf("%c", x->oemid[i]);
    xprintf("\n");

    xprintf("rsdt oem table id: ");
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
        if (strncmp(tmp, "APIC", 4) && !strncmp(tmp, "APICx", 5))
            return (apic_sdt_entry *)tmp;
        else
            tmp++;
}

void madt_entries_get(apic_sdt_entry *apic_entry)
{

    madt_entry_type0_ptr = malloc(sizeof(madt_entry_type0) * 10);
    madt_entry_type1_ptr = malloc(sizeof(madt_entry_type1) * 10);
    madt_entry_type2_ptr = malloc(sizeof(madt_entry_type2) * 10);
    madt_entry_type3_ptr = malloc(sizeof(madt_entry_type3) * 10);
    madt_entry_type4_ptr = malloc(sizeof(madt_entry_type4) * 10);
    madt_entry_type5_ptr = malloc(sizeof(madt_entry_type5) * 10);
    madt_entry_type9_ptr = malloc(sizeof(madt_entry_type9) * 10);

    uint8_t *tmp = nullptr; //(uint8_t*)&apic_entry->entry_type;

    xprintf("lolek: 0x%x\n", tmp);

    for (int i = 0; i < 0x10; tmp += *(tmp + 1), i++)
    {

        if (*tmp == 0)
        {
            madt_entry_type0_ptr[madt_entry_type0_counter] = (madt_entry_type0 *)tmp;
            madt_entry_type0_counter++;
        }

        else if (*tmp == 1)
        {
            madt_entry_type1_ptr[madt_entry_type1_counter] = (madt_entry_type1 *)tmp;
            madt_entry_type1_counter++;
        }

        else if (*tmp == 2)
        {
            madt_entry_type2_ptr[madt_entry_type2_counter] = (madt_entry_type2 *)tmp;
            madt_entry_type2_counter++;
        }

        else if (*tmp == 3)
        {
            madt_entry_type3_ptr[madt_entry_type3_counter] = (madt_entry_type3 *)tmp;
            madt_entry_type3_counter++;
        }

        else if (*tmp == 4)
        {
            madt_entry_type4_ptr[madt_entry_type4_counter] = (madt_entry_type4 *)tmp;
            madt_entry_type4_counter++;
        }

        else if (*tmp == 5)
        {
            madt_entry_type5_ptr[madt_entry_type5_counter] = (madt_entry_type5 *)tmp;
            madt_entry_type5_counter++;
        }

        else if (*tmp == 9)
        {
            madt_entry_type9_ptr[madt_entry_type9_counter] = (madt_entry_type9 *)tmp;
            madt_entry_type9_counter++;
        }
    }
}

/*


uint8_t lapic_ids[256]={0}; // CPU core Local APIC IDs
uint8_t numcore=0;          // number of cores detected
uint64_t lapic_ptr=0;       // pointer to the Local APIC MMIO registers
uint64_t ioapic_ptr=0;      // pointer to the IO APIC MMIO registers

void detect_cores(uint8_t *rsdt)
{
  uint8_t *ptr, *ptr2;
  uint32_t len;

  // iterate on ACPI table pointers
  for(len = *((uint32_t*)(rsdt + 4)), ptr2 = rsdt + 36; ptr2 < rsdt + len; ptr2 += rsdt[0]=='X' ? 8 : 4) {
    ptr = (uint8_t*)(uintptr_t)(rsdt[0]=='X' ? *((uint64_t*)ptr2) : *((uint32_t*)ptr2));
    if(!memcmp(ptr, "APIC", 4)) {
      // found MADT
      lapic_ptr = (uint64_t)(*((uint32_t*)(ptr+0x24)));
      ptr2 = ptr + *((uint32_t*)(ptr + 4));
      // iterate on variable length records
      for(ptr += 44; ptr < ptr2; ptr += ptr[1]) {
        switch(ptr[0]) {
          case 0: if(ptr[4] & 1) lapic_ids[numcore++] = ptr[3]; break; // found Processor Local APIC
          case 1: ioapic_ptr = (uint64_t)*((uint32_t*)(ptr+4)); break;  // found IOAPIC
          case 5: lapic_ptr = *((uint64_t*)(ptr+4)); break;             // found 64 bit LAPIC
        }
      }
      break;
    }
  }
}


*/