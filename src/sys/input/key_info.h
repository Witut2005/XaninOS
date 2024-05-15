
#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    char character;
    uint8_t scan_code;
}xchar;

typedef struct {
    uint8_t scan_code;
    char character;

    bool keys_pressed[0x80];
    bool special_keys_pressed[0x80];

    bool is_caps;
    bool is_shift;
    bool is_ctrl;
}KeyInfo;
