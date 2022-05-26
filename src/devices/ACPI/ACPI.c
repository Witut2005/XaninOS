
#include <devices/ACPI/ACPI.h>


uint32_t acpi_rdsp_base;

rsdp_descriptor1* get_acpi_rsdp_address_base(void)
{
    char* acpi_string = (char*)0xE000;

    while(acpi_string < (char*)0xFFFFF)
    {    
        if(strncmp(acpi_string, "RSD PTR ", 5))
        {
            return (rsdp_descriptor1*)acpi_string;
        }
    
        acpi_string += 0x10;
    }
}


void acpi_print_rsdp(rsdp_descriptor1* rsdz)
{
    xprintf("RDSP data: %x\n", rsdp);
    
    

    xprintf("signature: ");
    for(int i = 0; i < 8; i++)
        xprintf("%c", rsdp->signature[i]);

    xprintf("\n");

    xprintf("checksum: 0x%x\n", rsdp->checksum);
    xprintf("OMID: %s\n", rsdp->oemid);
    xprintf("revision: 0x%x\n", rsdp->revision);
    xprintf("rsdt base: 0x%x\n", rsdp->rsdt_address);
}

uint32_t acpi_checksum_check(void)
{
    uint32_t sum = 0;
    const uint8_t* const field = (uint8_t*)rsdt;
    
    int i = 0;
    do 
    {
        sum += field[i];
        i++;
    }while(sum % 0x100 != 0);

    return i;


}


void acpi_print_sdt(sdt* x)
{
    xprintf("singature:       : ");

    for(int i = 0; i < 4; i++)
        xprintf("%c", x->signature[i]);
    
    xprintf("\n");

    xprintf("length           : 0x%x\n", x->length);
    xprintf("revision         : 0x%x\n", x->revision);
    xprintf("checksum         : 0x%x\n", x->checksum);

    xprintf("rsdt oemid       : ");
    for(int i = 0; i < 6; i++)
        xprintf("%c", x->oemid[i]);
    xprintf("\n");


    xprintf("rsdt oem table id: ");
        for(int i = 0; i < 8; i++)
            xprintf("%c", x->oem_table_id[i]);
    xprintf("\n");

    xprintf("oem_revision     : 0x%x\n", x->oem_revision);
    xprintf("creator_id       : 0x%x\n", x->creator_id);
    xprintf("creator_revision : 0x%x\n", x->creator_revision);
 
}















