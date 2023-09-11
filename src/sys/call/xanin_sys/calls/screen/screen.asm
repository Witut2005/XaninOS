
%include "./sys/call/xanin_sys/calls/macros.inc"
%include "./sys/call/xanin_sys/calls/screen/ids.inc"

global __sys_screen_init, __sys_letters_refresh, __sys_letters_refresh_add

__sys_screen_init: 
mov eax, XANIN_SCREEN_INIT
XANIN_INVOKE_SYSTEM_CALL
ret

__sys_letters_refresh: 
mov eax, XANIN_SCREEN_LETTERS_REFRESH
ESP_GET_NTH_ARGUMENT ecx, 1
XANIN_INVOKE_SYSTEM_CALL
ret

__sys_letters_refresh_add:
mov eax, XANIN_SCREEN_LETTERS_REFRESH_ADD
ESP_GET_NTH_ARGUMENT ecx, 1
ESP_GET_NTH_ARGUMENT edx, 2
XANIN_INVOKE_SYSTEM_CALL
ret

