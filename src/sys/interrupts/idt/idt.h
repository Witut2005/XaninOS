
#pragma once
#include <stdint.h>

typedef uint8_t interrupt_vector_t;
typedef void (*irq_handler)(void);
extern irq_handler interrupt_handlers[0x100];

#define INTERRUPT_REGISTER(interrupt_id, handler)\
{\
    interrupt_handlers[interrupt_id] = handler;\
    set_idt();\
}

#define INTERRUPT_UNREGISTER(interrupt_id)\
{\
    interrupt_handlers[interrupt_id] = NULL;\
    set_idt();\
}

#ifdef __cplusplus
extern "C" {
#endif
    extern void set_idt(void);
#ifdef __cplusplus
}
#endif
