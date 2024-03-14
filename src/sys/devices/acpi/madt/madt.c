
#include <lib/libc/string.h>
#include <sys/pmmngr/alloc.h>
#include <sys/devices/acpi/sdt/sdt.h>
#include <sys/devices/acpi/rsdt/rsdt.h>
#include <sys/devices/acpi/madt/madt.h>
#include <sys/devices/com/com.h>

uint8_t* madt_entries[0x10];
uint8_t acpi_madt0_counter = 0;
uint8_t acpi_madt1_counter = 0;
uint8_t acpi_madt2_counter = 0;
uint8_t acpi_madt3_counter = 0;
uint8_t acpi_madt4_counter = 0;
uint8_t acpi_madt5_counter = 0;
uint8_t acpi_madt9_counter = 0;

const SystemAcpiMADT0** AcpiMADT0Pointers;
const SystemAcpiMADT1** AcpiMADT1Pointers;
const SystemAcpiMADT2** AcpiMADT2Pointers;
const SystemAcpiMADT3** AcpiMADT3Pointers;
const SystemAcpiMADT4** AcpiMADT4Pointers;
const SystemAcpiMADT5** AcpiMADT5Pointers;
const SystemAcpiMADT9** AcpiMADT9Pointers;

void madt_entries_get(SystemAcpiSDT* apic_entry)
{

    AcpiMADT0Pointers = kmalloc(SIZE_OF(SystemAcpiMADT0) * 10);
    AcpiMADT1Pointers = kmalloc(SIZE_OF(SystemAcpiMADT1) * 10);
    AcpiMADT2Pointers = kmalloc(SIZE_OF(SystemAcpiMADT2) * 10);
    AcpiMADT3Pointers = kmalloc(SIZE_OF(SystemAcpiMADT3) * 10);
    AcpiMADT4Pointers = kmalloc(SIZE_OF(SystemAcpiMADT4) * 10);
    AcpiMADT5Pointers = kmalloc(SIZE_OF(SystemAcpiMADT5) * 10);
    AcpiMADT9Pointers = kmalloc(SIZE_OF(SystemAcpiMADT9) * 10);

    uint8_t *tmp = (uint8_t*)(apic_entry) + 0x2C;

    for (int i = 0; i < 0x10; tmp += *(tmp + 1), i++)
    {
        if (*tmp == 0)
            AcpiMADT0Pointers[acpi_madt0_counter++] = (SystemAcpiMADT0 *)tmp;

        else if (*tmp == 1)
            AcpiMADT1Pointers[acpi_madt1_counter++] = (SystemAcpiMADT1 *)tmp;

        else if (*tmp == 2)
            AcpiMADT2Pointers[acpi_madt2_counter++] = (SystemAcpiMADT2 *)tmp;

        else if (*tmp == 3)
            AcpiMADT3Pointers[acpi_madt3_counter++] = (SystemAcpiMADT3 *)tmp;

        else if (*tmp == 4)
            AcpiMADT4Pointers[acpi_madt4_counter++] = (SystemAcpiMADT4 *)tmp;

        else if (*tmp == 5)
            AcpiMADT5Pointers[acpi_madt5_counter++] = (SystemAcpiMADT5 *)tmp;

        else if (*tmp == 9)
            AcpiMADT9Pointers[acpi_madt9_counter++] = (SystemAcpiMADT9 *)tmp;
    }
    dbg_info(DEBUG_LABEL_KERNEL, "ACPI madt entries get");
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
        if(bstrncmp((char*)sdt_pointers[i], "APIC", 4))
            return (const SystemAcpiSDT* const)(sdt_pointers[i]);
    }
    return NULL;
}