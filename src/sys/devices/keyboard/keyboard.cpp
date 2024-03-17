
#include "./keyboard.hpp"
#include "./scan_codes.h"
#include <sys/input/input.h>
#include <lib/libc/hal.h>
#include <sys/devices/com/com.h>

extern "C" void keyboard_handler_init(void);
extern "C" int exit(void);

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
    // interrupt_disable();
    dbg_info(DEBUG_LABEL_KERNEL_DEVICE, "Keyboard handler");
    static key_info_t KeyInfo;
    KeyInfo.scan_code = read(ControllerPort::KeyboardEncoder);
    // xprintf("%x ", KeyInfo.scan_code);

    if (!m_special_key_pressed)
    {
        if (KeyInfo.scan_code == KEYBOARD_SPECIAL_KEYS_PREFIX)
        {
            m_special_key_pressed = true;
            return;
        }
        else
            m_special_key_pressed = false;
    }

    if (m_special_key_pressed)
    {
        if (!is_break_code(KeyInfo.scan_code))
            KeyInfo.special_keys_pressed[KeyInfo.scan_code] = true;
        else
            KeyInfo.special_keys_pressed[KeyInfo.scan_code - KEYBOARD_KEYS_BREAK_CODES_OFFSET] = false;

        m_special_key_pressed = false;
    }

    else
    {
        if (!is_break_code(KeyInfo.scan_code))
            KeyInfo.keys_pressed[KeyInfo.scan_code] = true;
        else
            KeyInfo.keys_pressed[KeyInfo.scan_code - KEYBOARD_KEYS_BREAK_CODES_OFFSET] = false;
    }

    if (KeyInfo.scan_code == KBP_CAPSLOCK)
        KeyInfo.is_caps = ~KeyInfo.is_caps;

    __input_global_key_info_set(KeyInfo);
    __input_scan_code_mapper_call(KeyInfo.scan_code);
    __input_handle_observed_objects(&KeyInfo);

    __input_call_handlers(KeyInfo);

    for (int i = 0; i < 20;i += 2);

    // if (__input_is_ctrl_pressed() && KeyInfo.keys_pressed[KBP_C])
    //     exit();
}

void Keyboard::cpu_reset(void)
{
    write(ControllerPort::OnboardKeyboardController, Command::CPUSystemReset);
}

void Keyboard::write(ControllerPort reg, uint8_t data)
{
    while ((inbIO(ControllerPort::OnboardKeyboardController) & StatusRegisterMask::InputBufferStatus) == BufferStatus::Full);
    outbIO(reg, data);
}

uint8_t Keyboard::read(ControllerPort reg)
{
    while ((inbIO(ControllerPort::OnboardKeyboardController) & StatusRegisterMask::OutputBufferStatus) == BufferStatus::Empty);
    inbIO(reg);
}

void Keyboard::leds_set(Keyboard::leds_mask_t mask)
{
    write(ControllerPort::KeyboardEncoder, Command::LEDset);
    write(ControllerPort::KeyboardEncoder, mask);
}

Keyboard Keyboard::s_instance;

extern "C"
{
    bool keyboard_init(interrupt_vector_t vector) {
        return Keyboard::the().init(vector);
    }

    void keyboard_test(void)
    {
        Keyboard::the().test();
    }

    void keyboard_reset(void)
    {
        Keyboard::the().reset();
    }

    void keyboard_cpu_reset(void)
    {
        Keyboard::the().cpu_reset();
    }

    void keyboard_leds_set(uint8_t mask)
    {
        Keyboard::the().leds_set(mask);
    }

    void keyboard_handler(void)
    {
        Keyboard::the().handle();
    }

}