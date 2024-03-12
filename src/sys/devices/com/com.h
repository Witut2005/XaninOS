
#pragma once


#ifdef __cplusplus
extern "C" {
#endif

bool serial_port_initialize(void);
bool serial_port_byte_send(uint8_t val);
void serial_port_array_send(uint8_t* arr, uint32_t size);
void serial_port_string_send(char* str);

#ifdef __cplusplus
}
#endif