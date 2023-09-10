#pragma once

#include <stdint.h>
#include <lib/libc/stdiox.h>
#include <sys/terminal/frontend/frontend.h>
#include <sys/call/xanin_sys/calls/terminal/terminal.h>


#if __x86_64__

#define X86_POINTER uint64_t*
#define X86_POINTER_SIZE sizeof(uint8_t*)

#else

#define X86_POINTER uint32_t*
#define X86_POINTER_SIZE sizeof(uint8_t*)

#endif


#ifdef __cplusplus
extern "C" {
#endif

Xtb* __xtb_get(void);
void __xtb_init(uint32_t vga_width, uint32_t vga_height, uint16_t* vram);
void __xtb_scroll_up(Xtf* XtFrontend);
void __xtb_scroll_down(Xtf* XtFrontend);
void __xtb_flush(Xtf* XtFrontend);
void __xtb_flush_all(Xtf* XtFrontend);

static inline void xtb_is_currently_flushing_set(bool is_flushing)
{
    __xtb_get()->is_currently_flushing = is_flushing;
}

static inline bool xtb_is_currently_flushing_get(void)
{
    return __xtb_get()->is_currently_flushing;
}


static inline void xtb_disable_flushing(void)
{
    __xtb_get()->is_flushable = false;
}

static inline void xtb_enable_flushing(void)
{
    __xtb_get()->is_flushable = true;
}

static inline bool xtb_flushing_status_get(void)
{
    return __xtb_get()->is_flushable;
}

#ifdef __cplusplus
}
#endif
