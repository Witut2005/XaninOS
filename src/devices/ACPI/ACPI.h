#pragma once

#include <stdint.h>
#include <stddef.h>


struct rsdp_descriptor1
{

    uint8_t signature[8];
    uint8_t checksum;
    uint8_t oemid[6];
    
    uint8_t revision;
    uint32_t rsdt_address;

} __attribute__ ((packed))*rsdp;

typedef struct rsdp_descriptor1 rsdp_descriptor1;

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
    uint32_t creator_id;;
    uint32_t creator_revision; 
    uint32_t lapic_address;
    uint32_t flags;
    uint8_t entry_type;
    uint8_t record_length;

}__attribute__((packed))*apic_sdt;

typedef struct apic_sdt_entry apic_sdt_entry;






