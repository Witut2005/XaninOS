
[org 0x10000]

jmp start

data: db "/piwko.txt",0x0

start:

mov eax, 8
mov ebx, data
int 0x80
ret
