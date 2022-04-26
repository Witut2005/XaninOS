
;DOESNT WORK

[org 0x10000]
[bits 32]

jmp print_syscall

str: db "omg",0xa,0
str1: db "syscall are cool stuff",0xa,0x0

return_addr: resb 4

print_syscall:

;push dword [ebp + 4]
;ret

mov dword [return_addr], ebx


mov eax, 2
mov esi, ebx
int 0x80


;mov eax, 0
;mov esi, str1
;int 0x80

mov eax, 3
mov esi, str1
int 0x80

push dword [return_addr]
ret

times 512 - ($-$$) db 0x0
