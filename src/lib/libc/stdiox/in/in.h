#pragma once

#include <lib/libc/colors.h>
#include <lib/libc/stdlibx.h>
#include <stdint.h>
#include <sys/call/xanin_sys/calls/stdio/stdio.h>
#include <sys/call/xanin_sys/calls/vga/vga.h>
#include <sys/terminal/frontend/frontend.h>

#ifdef __cplusplus
extern "C" {
#endif

  char getchar(void);
  char getscan(void);
  xchar getxchar(void);
  xchar inputg(void);
  void xscan_range(char* string_buffer, uint32_t how_many_chars);
  void xscanf_handler(KeyInfo key_info, void* args);
  void xscanf(char* fmt, ...);

#ifdef __cplusplus
}
#endif