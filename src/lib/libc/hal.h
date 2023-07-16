
#pragma once

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

void outbIO(uint16_t port,uint8_t al);
void outwIO(uint16_t port,uint16_t ax);
void outdIO(uint16_t port,uint32_t eax);
void interrupt_disable(void);
void interrupt_enable(void);
uint8_t inbIO(uint16_t port);
uint16_t inwIO(uint16_t port);
uint32_t indIO(uint16_t port);
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void disable_cursor(void);
void update_cursor(uint32_t x, uint32_t y);
uint16_t get_cursor_position(void);
void eoi_send(void);
void io_wait(void);
void real_mode_enter(uint16_t segment, uint16_t offset, uint32_t return_address);
void real_mode_enter_no_return(uint16_t segment, uint16_t offset);
void rdmsr(uint32_t msr_id, uint32_t low, uint32_t high);
void wrmsr(uint32_t msr_id, uint32_t low, uint32_t high);
void pic_mask_set(uint16_t port, uint8_t value);
void pic_disable(void);
void sse_enable(void);
void pic_mode_disable(void);
// extern void __asm_registers_values_get(Registers* Reg);

#ifdef __cplusplus
}
#endif

static inline void sti(void)
{
    asm("sti");
}

static inline void cli(void)
{
    asm("cli");
}

static inline void stc(void)
{
    asm("stc");
}

static inline void clc(void)
{
    asm("clc");
}

// static inline void std(void)
// {
//     asm("std");
// }

static inline void cld(void)
{
    asm("cld");
}

static inline void cmc(void)
{
    asm("cmc");
}

static inline void cpu_halt(void)
{
    asm("cli");
    asm("hlt");
}

