

#include <stdint.h>
#include <lib/libc/hal.h> //c++ hal uses static ioport addreses
#include <sys/devices/com/com.hpp>

class SerialPort
{

    private:
        uint8_t com_id;
        bool dlab_state;
        bool chip_state;

    public:
        void dlab_enable(uint16_t divisor)
        {
            dlab_state = true;
            outbIO(COM1 + COM_LINE_CONTROL_REGISTER, DLAB_ENABLE);

            outbIO(COM1 + COM_DLAB0, divisor & 0xFF);
            outbIO(COM1 + COM_DLAB1, (divisor >> 8) & 0xFF);
        }
        
        void dlab_disable()
        {
            dlab_state = false;
            outbIO(COM1 + COM_LINE_CONTROL_REGISTER, 0x0);
        }

        void interrupt_enable(uint8_t interrupt_number)
        {
            outbIO(COM1 + COM_INTERRUPT_REGISTER, interrupt_number);
        }

        bool chip_state_get()
        {
            return chip_state;
        }

        void port_init(uint16_t divisor)
        {
            this->interrupt_enable(0x0);
            this->dlab_enable(0x00C0);
            outbIO(COM1 + COM_LINE_CONTROL_REGISTER, 0x3); //8 bits, no parity, one stop bit
            outbIO(COM1 + COM_FIFO, 0xC7); // https://stackoverflow.com/questions/21164092/role-of-fifo-buffer-for-com-port-in-windows

            outbIO(COM1 + COM_MODEM_CONTROL_REGISTER, 0xB);  //irq on
            outbIO(COM1 + COM_MODEM_CONTROL_REGISTER, 0x1F); //chip test


            outbIO(COM1 + COM_DATA_REGISTER, 0xAE);

            if(inbIO(COM1 + COM_DATA_REGISTER) == 0xAE) 
            {
                chip_state = false;
            }

            else
            {
                outbIO(COM1 + COM_MODEM_CONTROL_REGISTER, 0xF);
                chip_state = true;
            }


        }

}ComPort;

extern "C"
{

    void com_port_init(uint16_t divisor)
    {
        ComPort.port_init(divisor);
    }

    bool com_status(void)
    {
        return ComPort.chip_state_get();
    }

}
