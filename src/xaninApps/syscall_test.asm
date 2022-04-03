
;DOESNT WORK

[org 0x10000]
[bits 32]

jmp print_syscall

str: db "shutdown.bin",0


print_syscall:



mov eax, 200
mov esi, str
int 0x80


times 512 - ($-$$) db 0x0