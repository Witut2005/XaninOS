#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

  char* toupper(char* str);
  char* tolower(char* str);
  char* strcpy(char* dest, char const* src);
  char* strncpy(char* dest, char const* src, size_t size); // this strncpy implemtation put '\0' at the end
  char* strrev(char* str);

#ifdef __cplusplus
}
#endif
