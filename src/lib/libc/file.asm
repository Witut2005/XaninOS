
%define XANIN_FOPEN_SYSCALL_ID 10
%define XANIN_FREAD_SYSCALL_ID 11
%define XANIN_FWRITE_SYSCALL_ID 12
%define XANIN_FCLOSE_SYSCALL_ID 13

%define XANIN_OPEN_SYSCALL_ID 14
%define XANIN_READ_SYSCALL_ID 15
%define XANIN_WRITE_SYSCALL_ID 16
%define XANIN_CLOSE_SYSCALL_ID 17



global fopen
fopen:
mov eax, XANIN_FOPEN_SYSCALL_ID
; ecx, edx already set properly 
int 0x81
ret

;------------------------

global fread
fread:
mov eax, XANIN_FREAD_SYSCALL_ID
; ecx, edx already set properly 
mov ebx, [esp + 4] ; counter
int 0x81
ret

;------------------------

global fwrite
fwrite:
mov eax, XANIN_FWRITE_SYSCALL_ID
; ecx, edx already set properly 
mov ebx, [esp + 4] ; counter
int 0x81
ret

;------------------------

global fclose
fclose:
mov eax, XANIN_FCLOSE_SYSCALL_ID
; ecx, edx already set properly 
int 0x81
ret

;------------------------

global open
mov eax, XANIN_OPEN_SYSCALL_ID
; ecx, edx already set properly 
int 0x81
ret

;------------------------

global read
mov eax, XANIN_READ_SYSCALL_ID
; ecx, edx already set properly 
mov ebx, [esp + 4]; counter
int 0x81
ret

;------------------------

global write
mov eax, XANIN_OPEN_SYSCALL_ID
; ecx, edx already set properly 
mov ebx, [esp + 4]; counter
int 0x81
ret

;------------------------

global close
mov eax, XANIN_OPEN_SYSCALL_ID
; ecx, edx already set properly 
int 0x81
ret