
#pragma once

#include <lib/system/system.h>
#include <sys/input/key_info.h>
#include <sys/input/types.h>

static inline char __sys_getchar(void) { return (char)xanin_syscall0(XANIN_GETCHAR); }
static inline char __sys_getscan(void) { return (uint8_t)xanin_syscall0(XANIN_GETSCAN); }

static inline xchar __sys_inputg(void)
{
    uint32_t result = xanin_syscall0(XANIN_INPUTG);
    return *(xchar*)&result;
}

static inline xchar __sys_getxchar(void) { return __sys_inputg(); }
static inline void __sys_keyinfo_get(KeyInfo* key_info) { xanin_syscall1(XANIN_KEYINFO_GET, (uint32_t)key_info); }
static inline bool __sys_is_normal_key_pressed(uint8_t scan_code) { return (bool)xanin_syscall0(XANIN_IS_NORMAL_KEY_PRESSED); }
static inline bool __sys_is_special_key_pressed(uint8_t scan_code) { return (bool)xanin_syscall0(XANIN_IS_SPECIAL_KEY_PRESSED); }
static inline void __sys_input_character_mapper_set(char (*mapper)(uint8_t)) { xanin_syscall1(XANIN_INPUT_CHARACTER_MAPPER_SET, (uint32_t)mapper); }
static inline void __sys_input_prtsc_handler_set(input_scan_code_mapper_handler_t handler) { xanin_syscall1(XANIN_INPUT_PRTSC_HANDLER_SET, (uint32_t)handler); }

static inline bool __sys_input_add_handler(InputHandler const* handler) { return (bool)xanin_syscall1(XANIN_INPUT_ADD_HANDLER, (uint32_t)handler); }
static inline bool __sys_input_remove_handler(int id) { return (bool)xanin_syscall1(XANIN_INPUT_REMOVE_HANDLER, id); }
static inline void __sys_input_remove_user_handlers(void) { xanin_syscall0(XANIN_INPUT_REMOVE_USER_HANDLERS); }
