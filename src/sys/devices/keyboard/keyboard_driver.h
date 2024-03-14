
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

uint8_t keyboard_self_test(void);
void keyboard_reset(void);
uint8_t keyboard_init(uint8_t vector);
void keyboard_driver(void);

#ifdef __cplusplus
}
#endif