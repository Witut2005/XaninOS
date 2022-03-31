
;DOESNT WORK

[org 0x10000]

print_syscall:

mov eax, 1
mov esi, napis
int 0x80

jmp $

napis: db "syscall test", 0xa,0

times 512 - ($-$$) db 0x0