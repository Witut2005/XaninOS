
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
  char* buffer;
  uint32_t buffer_index;
  bool blocade;
};

void xscanf_handler(KeyInfo key_info, void* args)
{
  auto vty = __sys_vty_get();
  ScanfArgumentPack* pack = (ScanfArgumentPack*)args;

  if (key_info.scan_code == KBP_BACKSPACE && pack->buffer_index)
  {
    __sys_xtf_remove_last_cell(vty);
    __sys_xtb_flush(vty);

    pack->buffer[--pack->buffer_index] = '\0';
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
    pack->buffer[pack->buffer_index++] = key_info.character;
  }
  else if (key_info.scan_code == KBP_ENTER)
  {
    uint32_t str_counter = 0;
    uint32_t counter = 0;

    const char* str = pack->fmt;

    char* field_buffer = (char*)calloc(2048);

    while (str[str_counter] != '\0')
    {

      memset(field_buffer, 0, 2048);

      if (str[str_counter] == '%')
      {
        str_counter++;
        switch (str[str_counter])
        {

        case 's': {
          char* string_pointer = va_arg(pack->args, char*);

          if (!strlen(pack->buffer))
            break;

          for (int i = 0; pack->buffer[counter] != '\0' && pack->buffer[counter] != ' ';
            i++, counter++)
          {
            field_buffer[i] = pack->buffer[counter];
          }


          for (int i = 0; field_buffer[i] != '\0' && field_buffer[i] != ' ';
            i++)
          {
            if ((field_buffer[i] > 127) || (field_buffer[i] < 0x20))
            {
              string_pointer[i] = '\0'; // invalid ASCII characters
              break;
            }

            string_pointer[i] = field_buffer[i];
          }

          counter++;
          break;
        }

        case 'd': {

          uint32_t* number = va_arg(pack->args, uint32_t*);

          for (int i = 0; pack->buffer[counter] != '\0' && pack->buffer[counter] != ' ';
            i++)
          {
            field_buffer[i] = pack->buffer[counter];
            counter++;
          }

          *number = strtoi(field_buffer, STRTOI_DECIMAL);

          if (field_buffer[0] == '-')
            *number = *number * -1;

          break;
        }

        case 'c': {

          char* number = va_arg(pack->args, char*);
          *number = pack->buffer[0];
          break;
        }

        case 'x': {

          uint32_t* number = va_arg(pack->args, uint32_t*);

          for (int i = 0; pack->buffer[counter] != '\0' && pack->buffer[counter] != ' ';
            i++)
          {
            field_buffer[i] = pack->buffer[counter];
            counter++;
          }

          *number = strtoi(field_buffer, STRTOI_HEXADECIMAL);

          if (field_buffer[0] == '-')
            *number = *number * -1;

          break;
        }

        case 'b': {

          uint32_t* number = va_arg(pack->args, uint32_t*);

          for (int i = 0; pack->buffer[counter] != '\0' && pack->buffer[counter] != ' ';
            i++)
          {
            field_buffer[i] = pack->buffer[counter];
            counter++;
          }

          *number = strtoi(field_buffer, STRTOI_BINARY);

          break;
        }
        }

        str_counter++;
      }

      else
        str_counter++;
    }

    memset(field_buffer, 0, 1000);
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

  char* buffer = (char*)calloc(XANIN_PMMNGR_BLOCK_SIZE * 4);

  Xtf* vty = __sys_vty_get();
  __sys_xtf_scrolling_on(vty);
  __sys_xtf_cursor_off(vty);

  ScanfArgumentPack pack = { .fmt = fmt, .args = args, .buffer = buffer, .buffer_index = 0,.blocade = true };

  InputHandler handler = { xscanf_handler, { &pack, INPUT_USER } };
  auto handler_id = __sys_input_add_handler(&handler);

  while (pack.blocade);
  __sys_input_remove_handler(handler_id);
}