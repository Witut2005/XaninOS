
#pragma once

/* BIOS colors enum */
enum Colors{

    black = 0x0,
    blue = 0x1,
    green = 0x2,
    cyan = 0x3,
    red = 0x4,
    magenta = 0x5,
    brown = 0x6,
    lgray = 0x7,
    dgray = 0x8,
    lblue = 0x9,
    lgreen = 0xa,
    lcyan = 0xb,
    lred = 0xc,
    lmagenta = 0xd,
    yellow = 0xe,
    white = 0xf

};


#define white16 (uint16_t)('_' | (white)) 
#define white32 (uint32_t) (((' ' | (0xFF << 8)) << 16) | (' ' | (0xFF << 8 )))

