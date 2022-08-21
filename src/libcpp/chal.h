
#pragma once
#include <stdint.h>

extern "C"
{

void outbIO(uint16_t port,uint8_t al);
void outwIO(uint16_t port,uint16_t ax);
void outdIO(uint16_t port,uint32_t eax);
uint8_t inbIO(uint16_t port);
uint16_t inwIO(uint16_t port);
uint32_t indIO(uint16_t port);
void disable_cursor(void);
void eoi_send(void);
void io_wait(void);
void real_mode_enter(uint16_t segment, uint16_t offset, uint32_t return_address);
void rdmsr(uint32_t msr_id, uint32_t low, uint32_t high);
void wrmsr(uint32_t msr_id, uint32_t low, uint32_t high);

static inline void sti(void)
{
    asm("sti");
}

static inline void cli(void)
{
    asm("cli");
}

}