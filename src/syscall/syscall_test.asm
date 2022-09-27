


[bits 32]

test:

mov eax, 4
mov ebx, 1
mov ecx, napis
mov edx, 10
int 0x80
jmp $
ret

napis: db "xaninOS posix syscall", 0xa,0x0


times 512 - ($-$$) db 0x0
