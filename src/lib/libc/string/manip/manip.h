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
  char* strdup(const char* str);
  char* strcat(bool dest_first, char* dest, char* src);
  char* string_align_begin(char* const str, char filler, uint32_t count);
  char* string_align_end(char* const str, char filler, uint32_t count);

#ifdef __cplusplus
}
#endif
