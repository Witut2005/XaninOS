

section .data
napis: db "ugabuga",0xa,0x0
nazwa: db "/ivt", 0x0

;stos do kitu
;przerwania EHHHH

section .data
tmp: dd 0x0

section .text

global _start

_start:
mov eax, 5
mov ebx, nazwa
mov ecx, 3
int 0x80

mov [tmp], eax

mov ebx, eax
mov eax, 0x4
mov ecx, napis 
mov edx, 9
int 0x80

mov eax, 6
mov ebx, [tmp]
int 0x80

mov eax, 4
mov ebx, 1
mov ecx, napis
mov edx, 9
int 0x80

mov eax, 100
int 0x80

mov eax, 1
int 0x80
