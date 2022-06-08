
#pragma once

#include <stddef.h>
#include <stdint.h>


extern void outbIO(uint16_t port, uint8_t al);
extern void outwIO(uint16_t port, uint16_t ax);
extern void outdIO(uint16_t port, uint32_t eax);

extern void interrupt_disable(void);
extern void interrupt_enable(void);

extern uint8_t inbIO(uint16_t port);
extern uint16_t inwIO(uint16_t port);
extern uint32_t indIO(uint16_t port);

extern void disable_cursor(void);
extern void update_cursor(void);
extern void eoi_send(void);
extern void io_wait(void);
extern void real_mode_enter(void);
extern void rdmsr(void);
extern void wrmsr(void);
extern void pic_mask_set(uint16_t port, uint8_t value);
extern void pic_disable(void);
extern void sse_enable(void);
extern void pic_mode_disable(void);