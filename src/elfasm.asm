
section .data

str: db "elf loader works!!",0xa,0x0

section .text

global _start

_start:

mov esi, str
mov eax, 'p' + 's'
int 0x80
jmp $
ret

