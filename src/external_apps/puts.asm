

section .data
napis: db "ugabuga",0xa,0x0

section .text

global _start

_start:

mov eax, 0x1
mov ebx, napis
int 0x80
