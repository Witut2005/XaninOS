
;DOESNT WORK

[org 0x10000]
[bits 32]

jmp print_syscall


napis: "xaninOS syscall",0xa,0
return_addr: resb 4

print_syscall:

;push dword [ebp + 4]
;ret

mov dword [return_addr], ebx


mov eax, 1
mov edi, 2
mov esi, 3
mov edx, 4
mov ecx, 5
mov ebx, 6
mov esi, ebx
int 0x80


mov eax, 0
mov esi, napis
int 0x80

mov eax, 2
mov esi, ecx
int 0x80

jmp $


push dword [return_addr]
ret

times 512 - ($-$$) db 0x0
