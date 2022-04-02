
;DOESNT WORK

[org 0x10000]
[bits 32]

jmp print_syscall

xscanf123: db "abcd", 0x0
prints: db "omg you printed me",0xa,0

my_var: db 0x0
napis: db "%s",0x0
zsk_delay: db "100",0x0

print_syscall:



mov eax, 50
int 0x80


times 512 - ($-$$) db 0x0