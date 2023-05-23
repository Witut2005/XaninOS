
#pragma once

#include <stdint.h>

#include <sys/devices/acpi/sdt/sdt.h>

struct SystemAcpiMADT0
{
    uint8_t entry_type;
    uint8_t length;

    uint8_t acpi_processor_id;
    uint8_t apic_id;
    uint32_t flags;

}__attribute__((packed));
typedef struct SystemAcpiMADT0 SystemAcpiMADT0;

struct SystemAcpiMADT1
{
    uint8_t entry_type;
    uint8_t length;

    uint8_t io_apic_id;
    uint8_t reserved;
    uint32_t io_apic_base;
    uint32_t global_system_int_table;
}__attribute__ ((packed));
typedef struct SystemAcpiMADT1 SystemAcpiMADT1;


struct SystemAcpiMADT2
{
    uint8_t entry_type;
    uint8_t length;

    uint8_t bus_source;
    uint8_t irq_source;
    uint32_t global_system_int_table;
    uint16_t flags;
}__attribute__ ((packed));
typedef struct SystemAcpiMADT2 SystemAcpiMADT2;


struct SystemAcpiMADT3
{
    uint8_t entry_type;
    uint8_t length;

    uint8_t nmi_source;
    uint8_t reserved;
    uint16_t flags;
    uint32_t global_system_int;
}__attribute__ ((packed));
typedef struct SystemAcpiMADT3 SystemAcpiMADT3;


struct SystemAcpiMADT4
{
    uint8_t entry_type;
    uint8_t length;

    uint8_t acpi_processor_id;
    uint16_t flags;
    uint8_t lint;
}__attribute__ ((packed));
typedef struct SystemAcpiMADT4 SystemAcpiMADT4;

struct SystemAcpiMADT5
{
    uint8_t entry_type;
    uint8_t length;

    uint8_t reserved;

    uint32_t lapic_address_low;
    uint32_t lapic_address_high; //64bit physical address

}__attribute__ ((packed));
typedef struct SystemAcpiMADT5 SystemAcpiMADT5;

struct SystemAcpiMADT9
{
    uint8_t entry_type;
    uint8_t length;

    uint16_t reserved;
    uint32_t x2apic_id;
    uint32_t flags;
    uint32_t acpi_id;

}__attribute__ ((packed));
typedef struct SystemAcpiMADT9 SystemAcpiMADT9;


extern uint8_t acpi_madt0_counter;
extern uint8_t acpi_madt1_counter;
extern uint8_t acpi_madt2_counter;
extern uint8_t acpi_madt3_counter;
extern uint8_t acpi_madt4_counter;
extern uint8_t acpi_madt5_counter;
extern uint8_t acpi_madt9_counter;


extern const SystemAcpiMADT0** AcpiMADT0Pointers;
extern const SystemAcpiMADT1** AcpiMADT1Pointers;
extern const SystemAcpiMADT2** AcpiMADT2Pointers;
extern const SystemAcpiMADT3** AcpiMADT3Pointers;
extern const SystemAcpiMADT4** AcpiMADT4Pointers;
extern const SystemAcpiMADT5** AcpiMADT5Pointers;
extern const SystemAcpiMADT9** AcpiMADT9Pointers;

extern uint8_t* madt_entries[0x10];

#ifdef __cplusplus
extern "C"{
#endif

const SystemAcpiSDT* const apic_sdt_find(void);
void madt_entries_get(SystemAcpiSDT* apic_entry);
uint8_t madt_checksum_check(SystemAcpiSDT* entry);

#ifdef __cplusplus
}
#endif