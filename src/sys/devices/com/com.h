
#pragma once

#include "./labels.h"

#ifdef __cplusplus
extern "C" {
#endif

    bool serial_port_initialize(uint16_t default_divisor);
    void serial_port_byte_send(uint8_t val);
    void serial_port_array_send(uint8_t* arr, uint32_t size);
    void serial_port_string_send(const char* str);
    void dbg_success(const char* label, const char* msg);
    void dbg_info(const char* label, const char* msg);
    void dbg_warning(const char* label, const char* msg);
    void dbg_error(const char* label, const char* msg);
    void dbg_mark(const char* label);

#ifdef __cplusplus
}
#endif