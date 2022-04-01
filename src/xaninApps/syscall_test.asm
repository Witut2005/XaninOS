
;DOESNT WORK

[org 0x10000]
[bits 32]

print_syscall:

mov eax, 0
mov edi, xscanf123
mov esi, napis

int 0x80

jmp $

xscanf123: db '%','s',0x0
napis: db "syscall test", 0xa,0

times 512 - ($-$$) db 0x0