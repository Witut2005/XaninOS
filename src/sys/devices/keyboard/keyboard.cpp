
#include "./keyboard.hpp"
#include "./scan_codes.h"
#include <sys/input/input.h>
#include <lib/libc/hal.h>
#include <sys/devices/com/com.h>
#include <sys/terminal/backend/backend.h>
#include <sys/input/input.hpp>

extern "C" void keyboard_handler_init(void);
// extern "C" int exit(void);

using namespace Device;

Keyboard& Keyboard::the(void)
{
    return s_instance;
}

bool Keyboard::init(interrupt_vector_t vector)
{
    reset();
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
    #warning "TO DO exit on CTRL + C";
    auto& input = InputManager::the();

    m_key_info.scan_code = read(ControllerPort::KeyboardEncoder);

    if (!m_special_key_pressed)
    {
        if (m_key_info.scan_code == KEYBOARD_SPECIAL_KEYS_PREFIX)
        {
            m_special_key_pressed = true;
            return;
        }
        else {
            m_special_key_pressed = false;
        }
    }

    key_state_update(m_special_key_pressed, m_key_info.scan_code);

    input.key_info_update(m_key_info);
    input.mapper_call(m_key_info.scan_code);

    input.handlers_call();

    //updates screen buffer on every key press
    if (stdio_mode_get() == STDIO_MODE_TERMINAL) {
        __xtb_flush_all(__vty_get());
    }

    // if (__input_is_ctrl_pressed() && m_key_info.keys_pressed[KBP_C])
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

#warning "TODO sus";
uint8_t Keyboard::read(ControllerPort reg)
{
    if (reg == ControllerPort::KeyboardEncoder) // wait for Buffer being not empty
        ((inbIO(ControllerPort::OnboardKeyboardController) & StatusRegisterMask::OutputBufferStatus) == BufferStatus::Empty);
    return inbIO(reg);
}

void Keyboard::leds_set(Keyboard::leds_mask_t mask)
{
    write(ControllerPort::KeyboardEncoder, Command::LEDset);
    write(ControllerPort::KeyboardEncoder, mask);
}


void Keyboard::key_state_update(bool is_special_key, uint8_t scan_code)
{
    bool* key_table = is_special_key ? m_key_info.special_keys_pressed : m_key_info.keys_pressed;
    if (!InputManager::the().is_break_code(scan_code)) {
        key_table[scan_code] = true;
    }
    else {
        key_table[m_key_info.scan_code - KEYBOARD_KEYS_BREAK_CODES_OFFSET] = false;
    }

    if (!is_special_key)
    {
        switch (scan_code)
        {
        case KBP_CAPSLOCK:
            m_key_info.functional_keys.caps = !m_key_info.functional_keys.caps;
            break;

        case KBP_LEFT_SHIFT:
        case KBP_RIGHT_SHIFT:
            m_key_info.functional_keys.shift = true;
            break;

        case KBP_LEFT_ALT:
            m_key_info.functional_keys.alt = true;
            break;

        case KBP_LEFT_CONTROL:
            m_key_info.functional_keys.ctrl = true;
            break;

            /////////////////////////

        case KBR_LEFT_SHIFT:
        case KBR_RIGHT_SHIFT:
            m_key_info.functional_keys.shift = false;
            break;

        case KBR_LEFT_ALT:
            m_key_info.functional_keys.alt = false;
            break;

        case KBR_LEFT_CONTROL:
            m_key_info.functional_keys.ctrl = false;
            break;
        }
    }

    else
    {
        switch (scan_code)
        {
        case KBSP_RIGHT_ALT:
            m_key_info.functional_keys.alt = true;
            break;

        case KBSP_RIGHT_CONTROL:
            m_key_info.functional_keys.ctrl = true;
            break;

            //////////////////////////////////////

        case KBSR_RIGHT_ALT:
            m_key_info.functional_keys.alt = false;
            break;

        case KBSR_RIGHT_CONTROL:
            m_key_info.functional_keys.ctrl = false;
            break;
        }
    }

    m_special_key_pressed = false;
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