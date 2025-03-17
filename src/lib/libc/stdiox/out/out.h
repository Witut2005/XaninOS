#include <stdint.h>
typedef uint8_t color_t;

#ifdef __cplusplus
extern "C" {
#endif

  void putchar(char c);
  void stdio_init(void);
  void putchar_color(char c, color_t color);
  int vga_screen_buffer_clear(void);
  int screen_clear(void);
  void putc(char* str, uint32_t count);
  void putsc(char const* str, color_t color);
  void puts(char const* str);
  void puts_warning(char const* str);
  void puts_error(char const* str);
  void xprintf(char* fmt, ...);

#ifdef __cplusplus
}
#endif