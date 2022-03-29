#pragma once

#include <stdint.h>
#include <stdbool.h>

#define VGA_TEXT_MEMORY 0xb8000

#define increment_array(arr,ctr,plus)\
    for(uint8_t i = 0; i < ctr; i++)\
    {\
        arr[i] += plus;\
    }

#define decrement_array(arr,ctr,minus)\
    for(uint8_t i = 0; i < ctr; i++)\
    {\
        arr[i] -= minus;\
    }



uint8_t keyboard_input;

bool app_exited = false;
