
;DOESNT WORK

[org 0x10000]
[bits 32]

jmp print_syscall


print_syscall:

mov eax, 2
int 0x80


times 512 - ($-$$) db 0x0