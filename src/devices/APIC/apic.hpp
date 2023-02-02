
#include <stdint.h>
#include <devices/APIC/apic_registers.h>


class AdvancedProgrammableInterruptContoller
{

    public:
    static void write(uint32_t address, uint32_t value);     //APIC write
    static uint32_t read(uint32_t address);                  //APIC read
    static bool enable();                                    //enables APIC
    static bool disable();                                   //disables APIC
    static void set_spurious_vector_number(uint32_t vector); //set INT to be issued when delivering spurious interrupt
    static void lvt_timer_set();                             //specifies interrupt delivery when the APIC timer signals an interrupt

};


extern "C"
{

    void apic_write(uint32_t address, uint32_t value)
    {
        AdvancedProgrammableInterruptContoller::write(address, value);
    }

    bool apic_enable(void)
    {
        AdvancedProgrammableInterruptContoller::enable();
        AdvancedProgrammableInterruptContoller::set_spurious_vector_number(0xFF);
        return true;
    }

    bool apic_disable(void)
    {
        AdvancedProgrammableInterruptContoller::disable();
        return false;
    }

    uint32_t apic_read(uint32_t address)
    {
        return AdvancedProgrammableInterruptContoller::read(address);
    }

    void apic_set_spurious_vector_number(uint32_t vector_number)
    {
        AdvancedProgrammableInterruptContoller::set_spurious_vector_number(vector_number);
    }


    void apic_lvt_set(void)
    {
        AdvancedProgrammableInterruptContoller::lvt_timer_set();
    }


}
