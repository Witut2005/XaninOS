
#pragma once

#include <stdint.h>
#include <stddef.h>

extern void assert_func(bool condition, char* file_name, uint32_t line);

#define assert(condition) assert_func(condition, __FILE__, __LINE__)
