
#pragma once
typedef void (*irq_handler)(void);
extern irq_handler interrupt_handlers[0x100];

#ifdef __cplusplus
extern "C" {
#endif

extern void set_idt(void);
// extern void INTERRUPT_REGISTER(uint32_t irq, irq_handler handler);
// extern void INTERRUPT_REGISTER(uint32_t interrupt_id, irq_handler handler);

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
}
#endif
