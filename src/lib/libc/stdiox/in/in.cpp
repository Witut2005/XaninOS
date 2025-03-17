
#include <lib/libc/memory.h>
#include <lib/libc/pair.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/stdiox_legacy.h>
#include <lib/libc/stdlibx.h>
#include <lib/libc/string.h>
#include <lib/libc/time.h>
#include <lib/screen/screen.h>
#include <lib/system/system.h>
#include <stdarg.h>
#include <sys/devices/com/com.h>
#include <sys/devices/keyboard/scan_codes.h>
#include <sys/terminal/backend/backend.h>

char getchar(void)
{
  return __sys_getchar();
}

char getscan(void)
{
  return __sys_getscan();
}

xchar getxchar(void)
{
  return __sys_getxchar();
}

xchar inputg(void)
{
  return __sys_inputg();
}

void xscan_range(char* string_buffer, uint32_t how_many_chars)
{

  stdio_mode_t stdio_current_mode = stdio_mode_get();
  xchar Input;

  if (stdio_current_mode != STDIO_MODE_TERMINAL)
    return;

  uint32_t buffer_index = 0;
  char* field_buffer = (char*)calloc(how_many_chars);

  while (1)
  {
    Input = __sys_inputg();

    if (Input.scan_code == ENTER)
    {
      puts("\n");
      return;
    }

    else if (Input.scan_code == BSPC)
    {
      if (!buffer_index)
        continue;

      string_buffer[--buffer_index] = '\0';
      stdio_legacy_cell_put('\0', OUTPUT_COLOR_SET(black, black), Screen.y,
        Screen.x);
      __sys_xtf_remove_last_cell(__sys_vty_get());
      __sys_xtb_flush(__sys_vty_get());
      Screen.x--;
    }

    else if (Input.character)
    {
      if (buffer_index == how_many_chars)
        continue;

      string_buffer[buffer_index] = Input.character;
      __sys_xtf_cell_put(__sys_vty_get(), Input.character,
        OUTPUT_COLOR_SET(black, white));
      __sys_xtb_flush(__sys_vty_get());
      buffer_index++;
      Input.character = 0;
    }
  }
  free(field_buffer);
}



struct ScanfArgumentPack
{
  char* fmt;
  va_list args;
  uint32_t index;
  char* buffer;
  bool blocade;
};

void xscanf_handler(KeyInfo key_info, void* args)
{
  auto vty = __sys_vty_get();
  ScanfArgumentPack* pack = (ScanfArgumentPack*)args;

  if (key_info.scan_code == KBP_BACKSPACE)
  {
    if (pack->index) {
      pack->index--;
    }
    else {
      return;
    }

    __sys_xtf_remove_last_cell(vty);
    __sys_xtb_flush(vty);

    pack->buffer[pack->index] = '\0';
  }

  else if (key_info.scan_code == KBSP_ARROW_UP)
  {
    __sys_xtb_scroll_up(vty);
  }

  else if (key_info.scan_code == KBSP_ARROW_DOWN)
  {
    __sys_xtb_scroll_down(vty);
  }

  else if (key_info.scan_code == KBSP_ARROW_LEFT)
  {
    __sys_xtf_cursor_dec(vty);
  }

  else if (key_info.scan_code == KBSP_ARROW_RIGHT)
  {
    __sys_xtf_cursor_inc(vty);
  }
  else if (key_info.character)
  {
    __sys_xtf_cell_put(vty, key_info.character, OUTPUT_COLOR_SET(black, white));
    pack->buffer[pack->index++] = key_info.character;
  }
  else if (key_info.scan_code == KBP_ENTER)
  {
    strcpy((char*)va_arg(pack->args, uint32_t), pack->buffer);
    __sys_xtf_cell_put(vty, '\n', NULL);
    pack->blocade = false;
  }
}

void xscanf(char* fmt, ...)
{
  if (stdio_mode_get() != STDIO_MODE_TERMINAL)
  {
    return;
  }

  va_list args;
  va_start(args, fmt);

  uint32_t index = 0;

  // char* field_buffer = (char*)calloc(XSCANF_FIELD_BUFFER_SIZE);
  // char* string_typed_buffer = (char*)calloc(XSCANF_STRING_TYPED_BUFFER_SIZE);
  char* buffer = (char*)calloc(XANIN_PMMNGR_BLOCK_SIZE * 4);

  Xtf* vty = __sys_vty_get();
  __sys_xtf_scrolling_on(vty);
  __sys_xtf_cursor_off(vty);

  ScanfArgumentPack pack = { fmt, args, 0, buffer, true };
  InputHandler handler = { xscanf_handler, { &pack, INPUT_USER } };
  auto handler_id = __sys_input_add_handler(&handler);

  while (pack.blocade);
  __sys_input_remove_handler(handler_id);
}