#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

[[nodiscard]] char* char_find_from_end(char* str, uint32_t offset, char c);
[[nodiscard]] char* char_find_from_end(char* str, uint32_t offset, char c);
[[nodiscard]] char* substr_find(char* str, char const* substr);
[[nodiscard]] char* substr_last_find(char* str, char const* substr);

#ifdef __cplusplus
}
#endif
