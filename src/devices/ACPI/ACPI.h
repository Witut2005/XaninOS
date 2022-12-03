#pragma once

#include <stdint.h>
#include <stddef.h>


struct rsdp_descriptor
{

    uint8_t signature[8];
    uint8_t checksum;
    uint8_t oemid[6];
    
    uint8_t revision;
    uint32_t rsdt_address;

} __attribute__ ((packed))*rsdp;

typedef struct rsdp_descriptor rsdp_descriptor;

struct acpi_rsdt
{
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oemid[6];
    char oem_table_id[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
    uint32_t pointer_to_sdt[0x10];
} __attribute__ ((packed));

typedef struct acpi_rsdt acpi_rsdt;

acpi_rsdt* rsdt;

struct sdt
{
    
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oemid[6];
    char oem_table_id[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;

} __attribute__ ((packed));

typedef struct sdt sdt;

struct apic_sdt_entry
{

    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;

    char oemid[6];
    char oem_table_id[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision; 

    //uint8_t entry_type;
    //uint8_t record_length;

}__attribute__((packed))*apic_sdt;
typedef struct apic_sdt_entry apic_sdt_entry;

uint8_t* madt_entries[0x10];

struct madt_entry_type0
{
    uint8_t entry_type;
    uint8_t length;

    uint8_t acpi_processor_id;
    uint8_t apic_id;
    uint32_t flags;

}__attribute__((packed));
typedef struct madt_entry_type0 madt_entry_type0;

struct madt_entry_type1
{
    uint8_t entry_type;
    uint8_t length;

    uint8_t io_apic_id;
    uint8_t reserved;
    uint32_t io_apic_base;
    uint32_t global_system_int_table;
}__attribute__ ((packed));
typedef struct madt_entry_type1 madt_entry_type1;


struct madt_entry_type2
{
    uint8_t entry_type;
    uint8_t length;

    uint8_t bus_source;
    uint8_t irq_source;
    uint32_t global_system_int_table;
    uint16_t flags;
}__attribute__ ((packed));
typedef struct madt_entry_type2 madt_entry_type2;


struct madt_entry_type3
{
    uint8_t entry_type;
    uint8_t length;

    uint8_t nmi_source;
    uint8_t reserved;
    uint16_t flags;
    uint32_t global_system_int;
}__attribute__ ((packed));
typedef struct madt_entry_type3 madt_entry_type3;


struct madt_entry_type4
{
    uint8_t entry_type;
    uint8_t length;

    uint8_t acpi_processor_id;
    uint16_t flags;
    uint8_t lint;
}__attribute__ ((packed));
typedef struct madt_entry_type4 madt_entry_type4;

struct madt_entry_type5
{
    uint8_t entry_type;
    uint8_t length;

    uint8_t reserved;

    uint32_t lapic_address_low;
    uint32_t lapic_address_high; //64bit physical address

}__attribute__ ((packed));
typedef struct madt_entry_type5 madt_entry_type5;

struct madt_entry_type9
{
    uint8_t entry_type;
    uint8_t length;

    uint16_t reserved;
    uint32_t x2apic_id;
    uint32_t flags;
    uint32_t acpi_id;

}__attribute__ ((packed));
typedef struct madt_entry_type9 madt_entry_type9;


extern uint8_t madt_entry_type0_counter;
extern uint8_t madt_entry_type1_counter;
extern uint8_t madt_entry_type2_counter;
extern uint8_t madt_entry_type3_counter;
extern uint8_t madt_entry_type4_counter;
extern uint8_t madt_entry_type5_counter;
extern uint8_t madt_entry_type9_counter;


extern const madt_entry_type0** madt_entry_type0_ptr;
extern const madt_entry_type1** madt_entry_type1_ptr;
extern const madt_entry_type2** madt_entry_type2_ptr;
extern const madt_entry_type3** madt_entry_type3_ptr;
extern const madt_entry_type4** madt_entry_type4_ptr;
extern const madt_entry_type5** madt_entry_type5_ptr;
extern const madt_entry_type9** madt_entry_type9_ptr;


extern uint8_t* used_irqs;

