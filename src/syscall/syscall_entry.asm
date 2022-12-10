
extern syscall_handle
global syscall_entry

section .text

syscall_entry:
pushfd

push ebx
push edx
push ecx
push esi
push edi

call syscall_handle

pop edi
pop esi
pop ecx
pop edx
pop ebx
popfd

iretd