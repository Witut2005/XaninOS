
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif 
  bool is_digit(char c);
  bool is_in_char_range(char r1, char r2, char c);
  bool is_char(char c); // TODO is_printable_char
  uint32_t strlen(char const* str);
  int32_t strcmp(char const* a, char const* b);
  bool bstrcmp(char const* a, char const* b);
  bool bstrncmp(char const* a, char const* b, size_t string_size);

#ifdef __cplusplus
}
#endif 