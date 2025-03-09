
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif 

  int32_t strcmp(char const* a, char const* b);
  bool bstrcmp(char const* a, char const* b);
  bool bstrncmp(char const* a, char const* b, size_t string_size);

#ifdef __cplusplus
}
#endif 