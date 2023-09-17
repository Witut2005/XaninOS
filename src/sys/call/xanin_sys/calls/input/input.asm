
%include "./sys/call/xanin_sys/calls/macros.inc"
%include "./sys/call/xanin_sys/calls/input/ids.inc"

global __sys_getchar, __sys_getscan, __sys_inputg, __sys_getxchar, __sys_keyinfo_get

__sys_getchar:              ;char getchar(void)
mov eax, 201                ;getchar syscall id
XANIN_INVOKE_SYSTEM_CALL
ret                         ;eax holds return value

__sys_getscan:              ;char getscan(void)
mov eax, 202                ;getchar syscall id
XANIN_INVOKE_SYSTEM_CALL
ret                         ;eax holds return value

__sys_getxchar:
__sys_inputg:               ;xchar __sys_inputg(void)
mov eax, 203                ;inputg syscall id
ESP_GET_NTH_ARGUMENT ecx, 1
XANIN_INVOKE_SYSTEM_CALL
ret

__sys_keyinfo_get:          ;void __sys_keyinfo_get(key_info_t* ptr)
mov eax, 204            ;keyinfo_get syscall id
ESP_GET_NTH_ARGUMENT ecx, 1
XANIN_INVOKE_SYSTEM_CALL
ret