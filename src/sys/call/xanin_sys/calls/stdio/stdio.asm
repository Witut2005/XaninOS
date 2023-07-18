

[bits 32]

%define XANIN_STDIO_MODE_SET 2000
%define XANIN_STDIO_MODE_GET 2001

section .data
global stdio_current_mode

section .text

global stdio_mode_set
stdio_mode_set:
mov eax, XANIN_STDIO_MODE_SET
; ecx already set
int 0x81 
ret


global stdio_mode_get
stdio_mode_get:
mov eax, XANIN_STDIO_MODE_GET
int 0x81
ret