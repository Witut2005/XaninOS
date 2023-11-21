
%include "./sys/call/xanin_sys/calls/macros.inc"
%include "./sys/call/xanin_sys/calls/input/ids.inc"

global __sys_getchar, __sys_getscan, __sys_inputg, __sys_getxchar, __sys_keyinfo_get
global __sys_input_character_mapper_set, __sys_input_character_mapper_call
global __sys_is_normal_key_pressed, __sys_is_special_key_pressed
global __sys_input_add_object_to_observe, __sys_input_remove_object_from_observe, __sys_input_handle_observed_objects
global __sys_input_add_handler, __sys_input_remove_handler, __sys_input_remove_user_handlers, __sys_input_call_handlers

global __sys_input_prtsc_handler_set

__sys_getchar:              ;char getchar(void)
mov eax, XANIN_GETCHAR                ;getchar syscall id
XANIN_INVOKE_SYSTEM_CALL
ret                         ;eax holds return value

__sys_getscan:              ;char getscan(void)
mov eax, XANIN_GETSCAN                ;getchar syscall id
XANIN_INVOKE_SYSTEM_CALL
ret                         ;eax holds return value

__sys_getxchar:
__sys_inputg:               ;xchar __sys_inputg(void)
mov eax, XANIN_INPUTG                ;inputg syscall id
ESP_GET_NTH_ARGUMENT ecx, 1
XANIN_INVOKE_SYSTEM_CALL
ret

__sys_keyinfo_get:          ;void __sys_keyinfo_get(key_info_t* ptr)
mov eax, XANIN_KEYINFO_GET  ;keyinfo_get syscall id
ESP_GET_NTH_ARGUMENT ecx, 1
XANIN_INVOKE_SYSTEM_CALL
ret

__sys_is_normal_key_pressed:;void __sys_keyinfo_get(key_info_t* ptr)
mov eax, XANIN_IS_NORMAL_KEY_PRESSED;keyinfo_get syscall id
ESP_GET_NTH_ARGUMENT ecx, 1
XANIN_INVOKE_SYSTEM_CALL
ret

__sys_is_special_key_pressed:;void __sys_keyinfo_get(key_info_t* ptr)
mov eax, XANIN_IS_SPECIAL_KEY_PRESSED;keyinfo_get syscall id
ESP_GET_NTH_ARGUMENT ecx, 1
XANIN_INVOKE_SYSTEM_CALL
ret

__sys_input_character_mapper_set:
mov eax, XANIN_INPUT_CHARACTER_MAPPER_SET;keyinfo_get syscall id
ESP_GET_NTH_ARGUMENT ecx, 1
XANIN_INVOKE_SYSTEM_CALL
ret

__sys_input_character_mapper_call:
mov eax, XANIN_INPUT_CHARACTER_MAPPER_CALL;keyinfo_get syscall id
ESP_GET_NTH_ARGUMENT ecx, 1
XANIN_INVOKE_SYSTEM_CALL
ret

__sys_input_add_object_to_observe:
mov eax, XANIN_INPUT_ADD_OBJECT_TO_OBSERVE
ESP_GET_NTH_ARGUMENT ecx, 1
XANIN_INVOKE_SYSTEM_CALL
ret

__sys_input_remove_object_from_observe:
mov eax, XANIN_INPUT_REMOVE_OBJECT_FROM_OBSERVE
ESP_GET_NTH_ARGUMENT ecx, 1
XANIN_INVOKE_SYSTEM_CALL
ret

__sys_input_handle_observed_objects:
mov eax, XANIN_INPUT_HANDLE_OBSERVED_OBJECTS
ESP_GET_NTH_ARGUMENT ecx, 1
XANIN_INVOKE_SYSTEM_CALL
ret

__sys_input_add_handler:
ESP_GET_NTH_ARGUMENT ecx, 1
mov eax, XANIN_INPUT_ADD_HANDLER
XANIN_INVOKE_SYSTEM_CALL
ret

__sys_input_remove_handler:
ESP_GET_NTH_ARGUMENT ecx, 1
mov eax, XANIN_INPUT_REMOVE_HANDLER
XANIN_INVOKE_SYSTEM_CALL
ret

__sys_input_remove_user_handlers:
mov eax, XANIN_INPUT_REMOVE_USER_HANDLERS
XANIN_INVOKE_SYSTEM_CALL
ret

__sys_input_call_handlers:
mov eax, XANIN_INPUT_CALL_HANDLERS
ESP_GET_NTH_ARGUMENT ecx, 1
XANIN_INVOKE_SYSTEM_CALL
ret

global __sys_input_prtsc_handler_set
mov eax, XANIN_INPUT_PRTSC_HANDLER_SET
ESP_GET_NTH_ARGUMENT ecx, 1
XANIN_INVOKE_SYSTEM_CALL
ret



