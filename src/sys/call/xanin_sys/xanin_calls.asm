
[bits 32]

%define XANIN_DISK_READ_SYSCALL_ID 300
%define XANIN_DISK_WRITE_SYSCALL_ID 301

global __disk_read
__disk_read:
mov eax, XANIN_DISK_READ_SYSCALL_ID
; ecx, edx are already set
mov ebx, [esp + 4];buf
int 0x81
ret

global __disk_write
__disk_write:
mov eax, XANIN_DISK_WRITE_SYSCALL_ID
; ecx, edx are already set
mov ebx, [esp + 4]; buf
int 0x81
ret