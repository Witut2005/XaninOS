#pragma once

#include <stdint.h>
#include <lib/libc/time.h>

enum LIBC_STRTOI_OPTIONS {
  STRTOI_BINARY = 2,
  STRTOI_OCTAL = 8,
  STRTOI_DECIMAL = 10,
  STRTOI_HEXADECIMAL = 16
};

#ifdef __cplusplus
extern "C" {
#endif

  char* time_to_string(bcd_time_t time, char* buf);
  char* date_to_string(bcd_date_t date, char* buf);
  char* int_to_decimal_string(bool _signed, int32_t value, char* buf);
  char* int_to_string(uint32_t value, char* buf, const uint8_t base);
  char* bcd_to_string(uint8_t x, char* buf);
  char* bcd_stream_to_string(uint8_t* value, uint32_t value_size, char* buf);
  void erase_spaces(char* str);
  [[nodiscard]] uint32_t atoi(char* str);
  [[nodiscard]] uint32_t strtoi(const char* str, uint8_t format);

#ifdef __cplusplus
}
#endif