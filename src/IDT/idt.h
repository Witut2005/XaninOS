
#pragma once
typedef void (*irq_handler)(void);
extern void set_idt(void);
extern void irq_register(uint32_t irq, irq_handler handler);
extern void interrupt_register(uint32_t interrupt_id, irq_handler handler);
