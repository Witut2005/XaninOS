#pragma once

#include <stdint.h>
#include <stdbool.h>

#define VGA_TEXT_MEMORY 0xb8000

#define incArr(arr,ctr,plus)\
    for(uint8_t i = 0; i < ctr; i++)\
    {\
        arr[i] += plus;\
    }

#define decArr(arr,ctr,minus)\
    for(uint8_t i = 0; i < ctr; i++)\
    {\
        arr[i] -= minus;\
    }


uint16_t* petrisCursor = (uint16_t*)VGA_TEXT_MEMORY;
static uint8_t x;
static uint8_t y;

uint8_t input;

bool tetrisON = false;

bool exitApp = false;

uint32_t cpu_mhz = 0x0;