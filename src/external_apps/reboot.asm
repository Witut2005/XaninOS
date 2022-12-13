

section .text

global _start

_start:
mov eax, 88
mov ebx, 0xfee1dead
mov ecx, 672274793
int 0x80