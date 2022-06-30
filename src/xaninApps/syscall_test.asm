
;DOESNT WORK

[org 0x10000]
[bits 32]

print_syscall:

;push dword [ebp + 4]
;ret

mov dword [return_addr], ebx

mov eax, 'p' + 's'
mov esi, napis
int 0x80

mov eax, 'f' + 'o'
mov esi, file_name
int 0x80


mov dword esi, [eax + 59]
and esi, 0xFF00
shr esi, 8

mov eax, 'p' + 'x'
mov esi, esi
int 0x80

mov eax, 'z' + 's' + 'k'
int 0x80

jmp $


push dword [return_addr]
ret

napis: db "xaninOS syscall",0xa,0
file_name: db "syscall_test.bin",0
return_addr: resb 4

times 512 - ($-$$) db 0x20
