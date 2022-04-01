
;DOESNT WORK

[org 0x10000]
[bits 32]

print_syscall:

nop
nop
mov edi, 0xb8000

nop
nop
mov word [edi], 0x4141

nop
nop
jmp $

napis: db "syscall test", 0xa,0

times 512 - ($-$$) db 0x0