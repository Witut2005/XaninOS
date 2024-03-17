#pragma once

#include <sys/interrupts/idt/idt.h>

#ifdef __cplusplus
extern "C" {
#endif

    bool keyboard_init(interrupt_vector_t vector);
    void keyboard_test(void);
    void keyboard_reset(void);
    void keyboard_cpu_reset(void);
    void keyboard_leds_set(uint8_t mask);
    void keyboard_handler(void);

#ifdef __cplusplus
}
#endif