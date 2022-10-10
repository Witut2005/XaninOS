/*
#include <libcpp/hal.h>
#include <libc/hal.h>
#include <libc/stdiox.h>

#define PS2_DATA_REGISTER     0x60
#define PS2_STATUS_REGISTER   0x64
#define PS2_COMMAND_REGISTER  0x64




enum class Ps2
{
    ps2_write = 0,
    ps2_read = 1
};

class MouseClass
{
    using namespace hal;

    private:
    Port8 PicMasterData = Port8(0x21);
    Port8 PicSlaveData = Port8(0xa1);
    Port8 Ps2Data = Port8(PS2_DATA_REGISTER);
    Port8 Ps2Status = Port8(PS2_STATUS_REGISTER);
    Port8 Ps2Command = Port8(PS2_COMMAND_REGISTER);
    uint8_t mouse_cycle = 0;
    uint8_t mouse_x;
    uint8_t mouse_y;

    int8_t mouse_bytes[3] = {0};

    public:

    void print()
    {
        xprintf("%x\n", PicMasterData.port);
        xprintf("%x\n", PicSlaveData.port);
        xprintf("%x\n", Ps2Data.port);
        xprintf("%x\n", Ps2Status.port);
        xprintf("%x\n", Ps2Command.port);
    
    }

    void wait(Ps2 operation)
    {

        uint32_t time = 100000;

        if(operation == Ps2::ps2_write)
        {
            while(time--)
                if(inbIO(0x64) & 0x2 == 0) return;
                //if(Ps2Status.read() & 0x2 == 0) return;
            
        }

        else
        {
            while(time--)
                if(inbIO(0x64) & 1)return;
        }


    }

    uint8_t read()
    {
        this->wait(Ps2::ps2_read);
        return inbIO(0x60);
        //return Ps2Data.read();
    }
        
    void write(uint8_t value)
    {
        this->wait(Ps2::ps2_write);
        outbIO(0x64, 0xD4);
        //Ps2Command.write(0xD4);
        this->wait(Ps2::ps2_write);
        outbIO(0x60, value);
        //Ps2Data.write(value);
    }

    void handle()
    {

        PicMasterData = Port8(0x21);
        PicSlaveData = Port8(0xa1);
        Ps2Data = Port8(PS2_DATA_REGISTER);
        Ps2Status = Port8(PS2_STATUS_REGISTER);
        Ps2Command = Port8(PS2_COMMAND_REGISTER);


        switch(mouse_cycle)
        {
            case 0:
            {
                mouse_bytes[mouse_cycle] = Ps2Data.read();
                mouse_cycle++;
                break;
            }
            
            case 1:
            {
                mouse_bytes[mouse_cycle] = Ps2Data.read();
                mouse_cycle++;
                break;
            }

            case 2:
            {
                mouse_bytes[mouse_cycle] = Ps2Data.read();
                mouse_x = mouse_bytes[1];
                mouse_y = mouse_bytes[2];
                mouse_cycle = 0;
                xprintf("Mouse x: %d\n", mouse_x);
                xprintf("Mouse y: %d\n", mouse_y);
                break;
            }
        }
    }

    void enable()
    {

        //https://wiki.osdev.org/Mouse_Input#0xD4_Byte.2C_Command_Byte.2C_Data_Byte  

        uint8_t status;
        PicMasterData = Port8(0x21);
        PicSlaveData = Port8(0xa1);
        Ps2Data = Port8(PS2_DATA_REGISTER);
        Ps2Status = Port8(PS2_STATUS_REGISTER);
        Ps2Command = Port8(PS2_COMMAND_REGISTER);


        //Enable second PS/2 port (only if 2 PS/2 ports supported)
        this->wait(Ps2::ps2_write);
        outbIO(0x64, 0xA8);
        //Ps2Command.write(0xA8);

        //Read PS/2 controller configuration byte
        this->wait(Ps2::ps2_write);
        outbIO(0x64, 0x20);
        //Ps2Command.write(0x20);

        //Second PS/2 port interrupt (1 = enabled, 0 = disabled, only if 2 PS/2 ports supported)
        this->wait(Ps2::ps2_read);
        status = (inbIO(0x60) | 2);//Ps2Data.read() | 2;

        //Write Data to PS/2 contoller configuration byte
        this->wait(Ps2::ps2_write);
        outbIO(0x64, 60);

        //Ps2Command.write(0x60);

        //Write Data to PS/2 contoller configuration byte
        this->wait(Ps2::ps2_write);
        outbIO(0x60, status);
        //Ps2Data.write(status);



        //mouse on
        this->write(0xF6);
        this->read();

        //mouse on
        this->write(0xF4);
        this->read();



    }


}Mouse;


extern "C" 
{

    void mouse_handler(void)
    {
        //Mouse.handle();
        inbIO(0x60);
        xprintf("o");
        eoi_send();
        outbIO(0xa1, 0x20);
     
    }    

    void mouse_enable(void)
    {
        Mouse.enable();
    }

}

*/