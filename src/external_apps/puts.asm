

section .data
napis: db "ugabuga",0xa,0x0
nazwa: db "/ivt", 0x0

section .text

global _start

_start:

mov eax, 5
mov ebx, nazwa
mov ecx, 3
int 0x80
; jmp $

push eax

mov ebx, eax
mov eax, 0x4
mov ecx, napis 
mov edx, 10
int 0x80
; jmp $

mov eax, 6
pop ebx
int 0x80
; jmp $

mov eax, 100
int 0x80

mov eax, 1
int 0x80
