
#pragma once

#include <stdint.h>

typedef struct {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
}__attribute__((packed)) CPUIDResult;

struct EFlags
{
    union
    {
        uint32_t flags;
        struct
        {
            uint8_t cf : 1;
            uint8_t resv1 : 1;
            uint8_t pf : 1;
            uint8_t resv2 : 1;
            uint8_t af : 1;
            uint8_t resv3 : 1;
            uint8_t zf : 1;
            uint8_t sf : 1;
            uint8_t tf : 1;
            uint8_t intf : 1;
            uint8_t df : 1;
            uint8_t of : 1;
            uint8_t iopl : 2;
            uint8_t nt : 1;
            uint8_t md : 1;
            uint8_t rf : 1;
            uint8_t vm : 1;
            uint8_t ac : 1;
            uint8_t vif : 1;
            uint8_t vip : 1;
            uint8_t id : 1;
            uint8_t resv4 : 8;
            uint8_t none : 1;
            uint8_t ai : 1;
        };
    };
} __attribute__((packed));

typedef struct EFlags EFlags;

#define INTERRUPTS_OFF(flags_ptr) \
    eflags_get(flags_ptr);        \
    interrupt_disable();

#define INTERRUPTS_ON(flags_ptr) \
    if ((flags_ptr)->intf)       \
        interrupt_enable()

#ifdef __cplusplus
extern "C"
{
#endif

    void eflags_get(EFlags* ptr);
    void interrupt_disable(void);
    void interrupt_enable(void);
    void outbIO(uint16_t port, uint8_t al);
    void outwIO(uint16_t port, uint16_t ax);
    void outdIO(uint16_t port, uint32_t eax);
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

CPUIDResult cpuid(uint32_t leaf, uint32_t);