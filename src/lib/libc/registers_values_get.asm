

section .data

section .text
global __asm_registers_values_get

__asm_registers_values_get:
push ebx
mov ebx, [ebp + 8]; arguments 

mov dword [ebx], eax
mov dword [ebx + 4], ecx
mov dword [ebx + 8], edx
pop dword [ebx + 12]
mov dword [ebx + 16], esp
mov dword [ebx + 20], ebp
mov dword [ebx + 24], esi
mov dword [ebx + 28], edi
mov dword ebx, [ebx+ 12]

mov esp, [ebp+4]
ret