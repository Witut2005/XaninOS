
#pragma once


#ifdef __cplusplus
extern "C" {
#endif

bool serial_port_initialize(uint16_t default_divisor);
void serial_port_byte_send(uint8_t val);
void serial_port_array_send(uint8_t* arr, uint32_t size);
void serial_port_string_send(const char* str);
void dbg_info(const char* msg);
void dbg_warning(const char* msg);
void dbg_error(const char* msg);

#ifdef __cplusplus
}
#endif