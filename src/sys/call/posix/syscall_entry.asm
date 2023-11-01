
extern syscall_handle
extern kernel_loop
global syscall_entry

section .text

syscall_entry:

pushfd
pusha

cmp eax, 1
jne normal

popa
popfd

; mov eax, kernel_loop
mov dword [esp], kernel_loop
iret

popa 
popfd

iretd

normal:
call syscall_handle

popa 
popfd

iretd