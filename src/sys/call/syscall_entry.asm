
extern syscall_handle
global syscall_entry

section .text

syscall_entry:

pushfd
pusha

call syscall_handle

popa 
popfd

iretd