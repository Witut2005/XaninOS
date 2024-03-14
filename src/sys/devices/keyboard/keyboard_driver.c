
#pragma once

#include <lib/libc/hal.h>
#include <lib/libc/singal.h>
#include <lib/libc/stdlibx.h>
#include <lib/screen/screen.h>
#include <sys/devices/apic/apic_registers.h>
#include <sys/input/input.h>
#include <sys/devices/com/com.h>
#include <sys/interrupts/idt/idt.h>

#define KEYBOARD_DRIVER_KEY_REMAP(from, to) \
    if (KeyInfo.character == from)          \
    KeyInfo.character = to

#define KEYBOARD_ENCODER 0x60
#define ONBOARD_KEY_CONTROLLER 0x64
#define KEYBOARD_STATUS_REG 0x64
#define KEYBOARD_DATA_REG 0x60

#define KEYBOARD_TEST_FAILURE 0xfc
#define KEYBOARD_TEST_SUCCESS 0x55
#define KEYBOARD_DISABLE 0xAD
#define KEYBOARD_ENABLE 0xAE

extern int screenshot(void);
extern void keyboard_handler_init(void);

static bool SpecialKeyPressed;

static inline void keyboard_driver_clean_up(void)
{
    eoi_send();
    interrupt_enable();
}


uint8_t keyboard_self_test(void)
{
    outbIO(0x64, 0xaa);
    return inbIO(0x60);
}

void keyboard_reset(void)
{
    outbIO(0x64, KEYBOARD_DISABLE); // KEYBOARD OFF

    for (int i = 0; i < 10; i++)
        io_wait();

    outbIO(0x64, KEYBOARD_ENABLE); // KEYBOARD ON
}

uint8_t keyboard_init(uint8_t vector)
{

    uint8_t keyboard_status = keyboard_self_test();

    if (keyboard_status == KEYBOARD_TEST_FAILURE)
    {
        dbg_error(DEBUG_LABEL_KERNEL_DEVICE, "keyboard self test failed. Halting execution");
        cpu_halt();
    }

    INTERRUPT_REGISTER(vector, keyboard_handler_init);
    dbg_info(DEBUG_LABEL_IRQ, "Keyboard device installed");
    return keyboard_status;
}

void keyboard_driver(void)
{
    interrupt_disable();
    static key_info_t KeyInfo;
    KeyInfo.scan_code = inbIO(KEYBOARD_DATA_REG);
    // xprintf("%x ", KeyInfo.scan_code);

    if (!SpecialKeyPressed)
    {
        if (KeyInfo.scan_code == KEYBOARD_SPECIAL_KEYS_PREFIX)
        {
            SpecialKeyPressed = true;
            keyboard_driver_clean_up();
            return;
        }
        else
            SpecialKeyPressed = false;
    }

    // xprintf("%d ", SpecialKeyPressed);
    if (SpecialKeyPressed)
    {
        if (!is_break_code(KeyInfo.scan_code))
            KeyInfo.special_keys_pressed[KeyInfo.scan_code] = true;
        else
            KeyInfo.special_keys_pressed[KeyInfo.scan_code - KEYBOARD_KEYS_BREAK_CODES_OFFSET] = false;

        SpecialKeyPressed = false;
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

    // we need to set interrupts
    keyboard_driver_clean_up();

    __input_call_handlers(KeyInfo);

    if (__input_is_ctrl_pressed() && KeyInfo.keys_pressed[KBP_C])
        exit();
}
