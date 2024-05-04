
#include <lib/libc/string.h>
#include <sys/devices/acpi/madt/madt.h>
#include <sys/devices/acpi/rsdt/rsdt.h>
#include <sys/devices/acpi/sdt/sdt.h>
#include <sys/devices/com/com.h>
#include <sys/pmmngr/alloc.h>

uint8_t* madt_entries[0x10];
uint8_t acpi_madt0_counter = 0;
uint8_t acpi_madt1_counter = 0;
uint8_t acpi_madt2_counter = 0;
uint8_t acpi_madt3_counter = 0;
uint8_t acpi_madt4_counter = 0;
uint8_t acpi_madt5_counter = 0;
uint8_t acpi_madt9_counter = 0;

SystemAcpiMADT0 const** AcpiMADT0Pointers;
SystemAcpiMADT1 const** AcpiMADT1Pointers;
SystemAcpiMADT2 const** AcpiMADT2Pointers;
SystemAcpiMADT3 const** AcpiMADT3Pointers;
SystemAcpiMADT4 const** AcpiMADT4Pointers;
SystemAcpiMADT5 const** AcpiMADT5Pointers;
SystemAcpiMADT9 const** AcpiMADT9Pointers;

void madt_entries_get(SystemAcpiSDT const* const apic_entry)
{

    AcpiMADT0Pointers = kmalloc(sizeof(SystemAcpiMADT0) * 10);
    AcpiMADT1Pointers = kmalloc(sizeof(SystemAcpiMADT1) * 10);
    AcpiMADT2Pointers = kmalloc(sizeof(SystemAcpiMADT2) * 10);
    AcpiMADT3Pointers = kmalloc(sizeof(SystemAcpiMADT3) * 10);
    AcpiMADT4Pointers = kmalloc(sizeof(SystemAcpiMADT4) * 10);
    AcpiMADT5Pointers = kmalloc(sizeof(SystemAcpiMADT5) * 10);
    AcpiMADT9Pointers = kmalloc(sizeof(SystemAcpiMADT9) * 10);

    uint8_t* tmp = (uint8_t*)(apic_entry) + 0x2C;

    for (int i = 0; i < 0x10; tmp += *(tmp + 1), i++)
    {
        if (*tmp == 0)
            AcpiMADT0Pointers[acpi_madt0_counter++] = (SystemAcpiMADT0*)tmp;

        else if (*tmp == 1)
            AcpiMADT1Pointers[acpi_madt1_counter++] = (SystemAcpiMADT1*)tmp;

        else if (*tmp == 2)
            AcpiMADT2Pointers[acpi_madt2_counter++] = (SystemAcpiMADT2*)tmp;

        else if (*tmp == 3)
            AcpiMADT3Pointers[acpi_madt3_counter++] = (SystemAcpiMADT3*)tmp;

        else if (*tmp == 4)
            AcpiMADT4Pointers[acpi_madt4_counter++] = (SystemAcpiMADT4*)tmp;

        else if (*tmp == 5)
            AcpiMADT5Pointers[acpi_madt5_counter++] = (SystemAcpiMADT5*)tmp;

        else if (*tmp == 9)
            AcpiMADT9Pointers[acpi_madt9_counter++] = (SystemAcpiMADT9*)tmp;
    }
    dbg_info(DEBUG_LABEL_KERNEL, "ACPI madt entries get");
}

uint8_t madt_checksum_check(SystemAcpiSDT const* const entry)
{
    uint8_t* field = (uint8_t*)entry;
    uint8_t sum = 0;

    for (int i = 0; i < entry->length; i++)
        sum += field[i];

    return sum == 0;
}

SystemAcpiSDT const* const apic_sdt_find(void)
{
    uint32_t const* const sdt_pointers = (uint32_t const* const)(acpi_rsdt_get()->pointer_to_sdt);

    for (int i = 0; i < 10; i++)
    {
        if (bstrncmp((char*)sdt_pointers[i], "APIC", 4))
            return (SystemAcpiSDT const* const)(sdt_pointers[i]);
    }
    return NULL;
}
