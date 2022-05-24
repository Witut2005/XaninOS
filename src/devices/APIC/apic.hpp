
#include <stdint.h>
#include <devices/APIC/apic_registers.h>


class AdvancedProgrammableInterruptContoller
{

    public:
    void write(uint32_t address, uint32_t value);
    uint32_t read(uint32_t address);
    bool enable();
    bool disable();
    void set_spurious_vector_number(uint32_t vector);
    void lvt_set();

}Apic;


extern "C"
{

    void apic_write(uint32_t address, uint32_t value)
    {
        Apic.write(address, value);
    }

    bool apic_enable(void)
    {
        Apic.enable();
        return true;
    }

    bool apic_disable(void)
    {
        Apic.disable();
        return false;
    }

    uint32_t apic_read(uint32_t address)
    {
        return Apic.read(address);
    }

    void apic_set_spurious_vector_number(uint32_t vector_number)
    {
        Apic.set_spurious_vector_number(vector_number);
    }


    void apic_lvt_set(void)
    {
        Apic.lvt_set();
    }


}