#pragma once

#include <sys/interrupts/idt/idt.h>

#ifdef __cplusplus
extern "C" {
#endif

    bool keyboard_init_cpp(interrupt_vector_t vector);
    void kbd_cpu_reset(void);

#ifdef __cplusplus
}
#endif