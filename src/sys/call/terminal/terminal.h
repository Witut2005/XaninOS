
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void vty_set(Xtf* XtFrontend) __attribute__((fastcall));
Xtf* vty_get(void);
void xtb_init(uint32_t vga_width, uint32_t vga_height, uint16_t* vram)__attribute__((fastcall));

#ifdef __cplusplus
}
#endif