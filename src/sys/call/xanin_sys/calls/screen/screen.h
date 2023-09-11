

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void __sys_screen_init(void);
void __sys_letters_refresh (uint16_t* cursor_current_positon) __attribute__((deprecated));
void __sys_letters_refresh_add (uint16_t* cursor_current_positon, char character_saved) __attribute__((deprecated));

#ifdef __cplusplus
}
#endif