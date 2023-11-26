
#pragma once

#include <lib/libc/hal.h>
#include <lib/libc/singal.h>
#include <lib/libc/stdlibx.h>
#include <lib/screen/screen.h>
#include <sys/devices/apic/apic_registers.h>
#include <sys/input/input.h>

#define KEYBOARD_DRIVER_KEY_REMAP(from, to) \
    if (KeyInfo.character == from)          \
    KeyInfo.character = to

#pragma once

#include <lib/libc/hal.h>
#include <lib/screen/screen.h>
#include <sys/devices/apic/apic_registers.h>
#include <sys/terminal/backend/backend.h>

extern int screenshot(void);

#define KEYBOARD_ENCODER 0x60
#define ONBOARD_KEY_CONTROLLER 0x64
#define KEYBOARD_STATUS_REG 0x64
#define KEYBOARD_DATA_REG 0x60

static bool SpecialKeyPressed;

static inline void keyboard_driver_clean_up(void)
{
    eoi_send();
    interrupt_enable();
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

    KeyInfo.is_pressed = true;
    __input_global_key_info_set(KeyInfo);

    if (keyboard_handle != NULL)
        keyboard_handle();

    __input_scan_code_mapper_call(KeyInfo.scan_code);

    __input_handle_observed_objects(&KeyInfo);

    // we need to set interrupts
    keyboard_driver_clean_up();

    __input_call_handlers(KeyInfo);

    if (KeyInfo.is_ctrl && KeyInfo.character == 'c')
        exit();
}
