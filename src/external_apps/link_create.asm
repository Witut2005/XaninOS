
[bits 32]

section .data

section .text

global _start

_start:
mov eax, 8
mov ebx, [ebp + 12]
mov ecx, [ebp + 8]
jmp $
int 0x80

mov eax, 1
int 0x80
