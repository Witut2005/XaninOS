#pragma once
#include <stdint.h>

#define LIBC_STR_BAD_IP_ADDRESS 0xFFFFFFFF

enum STRING_ERNNO_VALUES {
    LIBC_STR_IPV4_ERRNO = 0x1
};

#ifdef __cplusplus
extern "C" {
#endif

[[nodiscard]] uint32_t check_string_errors(uint32_t mask);
[[nodiscard]] uint32_t str2ipv4(char const* str);

#ifdef __cplusplus
}
#endif
