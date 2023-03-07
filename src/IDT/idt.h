
#pragma once
typedef void (*irq_handler)(void);
extern irq_handler interrupt_handlers[0x100];

#ifdef __cplusplus
extern "C" {
#endif

extern void set_idt(void);
// extern void interrupt_register(uint32_t irq, irq_handler handler);
// extern void interrupt_register(uint32_t interrupt_id, irq_handler handler);

#define interrupt_register(interrupt_id, handler)\
{\
    interrupt_handlers[interrupt_id] = handler;\
    set_idt();\
}

#ifdef __cplusplus
}
#endif
