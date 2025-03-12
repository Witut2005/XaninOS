
#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
[[nodiscard]] bool is_digit(char c);
[[nodiscard]] bool is_in_char_range(char r1, char r2, char c);
[[nodiscard]] bool is_char(char c); // TODO is_printable_char
[[nodiscard]] uint32_t strlen(char const* str);
[[nodiscard]] int32_t strcmp(char const* a, char const* b);
[[nodiscard]] bool bstrcmp(char const* a, char const* b);
[[nodiscard]] bool bstrncmp(char const* a, char const* b, size_t string_size);
[[nodiscard]] uint32_t number_of_lines_get(char const* str);
[[nodiscard]] uint32_t size_of_biggest_line_get(char const* str);
#ifdef __cplusplus
}
#endif
