
#include <devices/ACPI/ACPI.h>


char* get_acpi_address_base(void)
{

    char* acpi_string = (char*)0xE000;

    while(acpi_string > (char*)0xFFFFF)
    {    
        if(cmpstr(acpi_string, "RSD PTR "))
        {
            return acpi_string;
        }
    
        acpi_string += 0x10;
    
    }


}



















