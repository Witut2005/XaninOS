
#pragma once

#include <stdint.h>

struct terminal_t
{
    uint16_t x;
    uint16_t y;
    uint8_t* buffer;
};

#ifndef __cplusplus
typedef struct terminal_t terminal_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

terminal_t* terminal_create(void);
void terminal_destroy(terminal_t* terminal, terminal_t* new_terminal);
void terminal_set(terminal_t* previous, terminal_t* terminal);

#ifdef __cplusplus
}
#endif
