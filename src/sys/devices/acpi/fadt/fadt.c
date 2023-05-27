
#include <stddef.h>
#include <lib/libc/string.h>
#include <sys/devices/acpi/fadt/fadt.h>
#include <sys/devices/acpi/rsdt/rsdt.h>

const SystemAcpiFADT* const acpi_fadt_find(void)
{
    const uint32_t* const sdt_pointers = (const uint32_t* const)(acpi_rsdt_get()->pointer_to_sdt);

    for(int i = 0; i < 10; i++)
    {
        if(bstrncmp((char*)sdt_pointers[i], "FACP", 4))
            return (const SystemAcpiFADT* const)sdt_pointers[i];
    }
    return NULL;
}
