
[bits 32]


mov ecx, 0x1ffff


mov dword ebx, [0x20002] 
mov eax, 0x500000

disk_loop:

mov edx, [eax]
mov dword [ebx], edx
inc eax

loop disk_loop

ret