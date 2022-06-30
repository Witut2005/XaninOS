
[bits 32]

test:

mov eax, 'z' + 's' + 'k'
int 0x80


times 512 - ($-$$) db 0x0
