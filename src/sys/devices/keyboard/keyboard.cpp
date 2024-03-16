
#include "./keyboard.hpp"
#include <lib/libc/hal.h>
#include <sys/devices/com/com.h>

extern "C" void keyboard_handler_init(void);

using namespace Device;

Keyboard& Keyboard::the(void)
{
    return s_instance;
}

bool Keyboard::init(interrupt_vector_t vector)
{
    bool ok = test();
    if (ok)
    {
        INTERRUPT_REGISTER(vector, keyboard_handler_init);
        dbg_info(DEBUG_LABEL_IRQ, "Keyboard device installed :))");
    }

    else {
        dbg_error(DEBUG_LABEL_KERNEL_DEVICE, "keyboard self test failed. Halting execution");
        cpu_halt();

    }
    return ok;
}

void Keyboard::reset(void)
{
    write(ControllerPort::OnboardKeyboardController, Command::Reset);
    //ENABLE keyboard 
}

bool Keyboard::test(void)
{
    write(ControllerPort::OnboardKeyboardController, Command::SelfTest);
    return read(ControllerPort::KeyboardEncoder) == SelfTestReturnStatus::Success ? true : false;
}

void Keyboard::handle(void)
{

}

void Keyboard::cpu_reset(void)
{
    write(ControllerPort::OnboardKeyboardController, Command::CPUSystemReset);
}

void Keyboard::write(ControllerPort reg, enum Command command)
{
    outbIO(reg, command);
}

uint8_t Keyboard::read(ControllerPort reg)
{
    inbIO(reg);
}

void Keyboard::leds_set(Keyboard::leds_mask_t mask) {

}

Keyboard Keyboard::s_instance;

extern "C"
{
    bool keyboard_init_cpp(interrupt_vector_t vector) {
        return Keyboard::the().init(vector);
    }

    void kbd_cpu_reset(void)
    {
        Keyboard::the().cpu_reset();
    }
}