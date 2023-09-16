
#pragma once

#include <stdint.h>
#include <stdbool.h>

struct xchar
{
    char character;
    uint8_t scan_code;
};

typedef struct xchar xchar;

struct key_info_t
{
    uint8_t scan_code;
    char character;

    bool is_shift;
    bool is_ctrl;
    bool is_caps;
    bool is_bspc;
    bool is_alt;

    bool is_left;
    bool is_right;
    bool is_up;
    bool is_down;
    
    bool is_pressed;
    bool is_hold;

};

typedef struct key_info_t key_info_t;