

section .data
napis: db "ugabuga",0xa,0x0
nazwa: db "/folder", 0x0
filename: db "/folder/aha.txt", 0x0
new_filename: db "/folder/from.lnk", 0x0

;stos do kitu
;przerwania EHHHH

section .data
tmp: dd 0x0

section .text

global _write_external

_write_external:

mov eax, 39
mov ebx, nazwa
int 0x80

mov eax, 8
mov ebx, filename
int 0x80

mov eax, 5
mov ebx, filename
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

mov eax, 9
mov ebx, filename
mov ecx, new_filename
int 0x80

mov eax, 1
int 0x80
