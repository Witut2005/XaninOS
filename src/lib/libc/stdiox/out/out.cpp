#include <stdarg.h>
#include <lib/libc/string.h>
#include <lib/libc/time.h>
#include <sys/terminal/backend/backend.h>
#include "./out.h"
#include <sys/call/xanin_sys/calls/stdio/stdio.h>
#include <sys/call/xanin_sys/calls/vga/vga.h>
#include <lib/libc/colors.h>

void putchar(char c)
{
  __sys_xtf_character_put(__sys_vty_get(), c);
}

void stdio_init(void)
{
  xanin_syscall0(XANIN_SHELL_STDEIO_CREATE);
  // XinEntryCreateArgs args;
  // args.entryname = "stdout";
  // stdout = (XinEntry*)xanin_syscall2(XANIN_XIN_ENTRY_CREATE, (uint32_t)&args, XIN_STDOUT);

  // args.entryname = "/proc/stdin";
  // stdin = (XinEntry*)xanin_syscall2(XANIN_XIN_ENTRY_CREATE, (uint32_t)&args, XIN_STDIN);

  // args.entryname = "/proc/OUTPUT_COLOR_ERROR_SET";
  // stdin = (XinEntry*)xanin_syscall2(XANIN_XIN_ENTRY_CREATE, (uint32_t)&args, XIN_STDIN);
}

void putchar_color(char c, color_t color)
{
  __sys_xtf_cell_put(__sys_vty_get(), c, color);
}

int vga_screen_buffer_clear(void)
{
  uint16_t* screen_cleaner = (uint16_t*)__vga_buffer_segment_get();
  for (int i = 0; i < (VGA_WIDTH * VGA_HEIGHT); i++)
  {
    *screen_cleaner = '\0';
    screen_cleaner++;
  }
  return XANIN_OK;
}

int screen_clear(void)
{
  if (stdio_mode_get() == STDIO_MODE_TERMINAL)
    __sys_xtf_buffer_clear(__sys_vty_get());
  return XANIN_OK;
}

void putc(char* str, uint32_t count)
{
  asm("mov eax, 4;"
    "mov ebx, 1;"
    "mov ecx, %0;"
    "mov edx, %1;"
    "int 0x80;"
    :
  : "g"(str), "g"(count));
}

void putsc(char const* str, color_t color)
{
  Xtf* StdioFront = __sys_vty_get();

  for (int i = 0; i < strlen(str); i++)
    __sys_xtf_cell_put(StdioFront, str[i], color);
}

void puts(char const* str)
{
  Xtf* StdioVty = __sys_vty_get();

  while (*str != '\0')
    __sys_xtf_character_put(StdioVty, *(str++));
}

void puts_warning(char const* str)
{
  putsc("[Warning]", OUTPUT_COLOR_SET(black, yellow));
  __sys_xtf_character_put(__sys_vty_get(), ' ');
  puts(str);
}

void puts_error(char const* str)
{
  putsc("[Error]", OUTPUT_COLOR_SET(black, lred));
  __sys_xtf_character_put(__sys_vty_get(), ' ');
  puts(str);
}


void xprintf(char* fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  Xtf* vty = __sys_vty_get();

  struct {
    uint8_t background;
    uint8_t foreground;
  } font_color = { black, white };

  for (int i = 0; fmt[i] != '\0';)
  {
    if (fmt[i] == '%')
    {
      switch (fmt[i + 1])
      {

      case '\0': {
        break;
      }

      case 'z': {
        uint8_t received_color = (uint8_t)va_arg(args, int);
        font_color.background = (received_color & 0xf0) >> 4;
        font_color.foreground = received_color & 0x0f;
        i = i + 2;
        break;
      }

      default: {
        char format[3];
        strncpy(format, &fmt[i], 2);
        char* buf = (char*)calloc(XANIN_PMMNGR_BLOCK_SIZE * 4);

        xsprintf(buf, format, (char*)va_arg(args, uint32_t));

        for (int j = 0; buf[j] != '\0'; j++)
        {
          __sys_xtf_cell_put(vty, buf[j],
            OUTPUT_COLOR_SET(font_color.background, font_color.foreground));
        }

        free(buf);
        i = i + 2;
        break;
      }
      }
    }

    else
    {
      __sys_xtf_cell_put(vty, fmt[i],
        OUTPUT_COLOR_SET(font_color.background, font_color.foreground));
      i++;
    }
  }
  __sys_xtb_flush(vty);
}