

section .data
napis: db "ugabuga",0xa,0x0

section .text

global _start

_start:

mov eax, 0x4
mov ebx, napis
mov ecx, 0x9
int 0x80
ret
