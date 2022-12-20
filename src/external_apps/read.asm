

section .data
tmp: dd 0x0
filename: db "/folder/aha.txt",0x0
buffer: resb 50

section .text

global _start

_start:

mov eax, 5
mov ebx, filename
mov ecx, 3
int 0x80

cmp eax, 0xFFFFFFFF
je end

mov [tmp], eax

mov eax, 19
mov ebx, [tmp]
mov ecx, 1
int 0x80

mov ebx, [tmp] 
mov eax, 3
mov ecx, buffer
mov edx, 8
int 0x80

mov eax, 6
mov ebx, [tmp]
int 0x80

mov eax, 4
mov ebx, 1
mov ecx, buffer
mov edx, 9
int 0x80

mov eax, 100
int 0x80

end:

mov eax, 1
int 0x80