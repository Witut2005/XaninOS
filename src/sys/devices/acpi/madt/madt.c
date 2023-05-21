
#include <lib/libc/string.h>
#include <sys/pmmngr/alloc.h>
#include <sys/devices/acpi/sdt/sdt.h>
#include <sys/devices/acpi/rsdt/rsdt.h>
#include <sys/devices/acpi/madt/madt.h>

uint8_t* madt_entries[0x10];
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

void madt_entries_get(SystemAcpiSDT* apic_entry)
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


uint8_t madt_checksum_check(SystemAcpiSDT* entry)
{
    uint8_t* field = (uint8_t*)entry;
    uint8_t sum = 0;

    for(int i = 0; i < entry->length; i++)
        sum += field[i];
    
    return sum == 0;
}

const SystemAcpiSDT* const apic_sdt_find(void)
{
    const uint32_t* const sdt_pointers = (const uint32_t* const)(acpi_rsdt_get()->pointer_to_sdt);

    for(int i = 0; i < 10; i++)
    {
        if(strncmp((char*)sdt_pointers[i], "APIC", 4))
            return (const SystemAcpiFADT* const)(sdt_pointers[i]);
    }
    return NULL;
}