

section .data

section .text
global __asm_registers_values_get

__asm_registers_values_get:

push ebp
mov ebp, esp

push ebx

mov ebx, [ebp + 8]; arguments 

mov dword [ebx], eax
mov dword [ebx + 4], ecx
mov dword [ebx + 8], edx
pop dword [ebx + 12]
mov dword [ebx + 16], esp
mov eax, [ebp]
mov dword [ebx + 20], eax; ebp after return
mov dword [ebx + 24], esi
mov dword [ebx + 28], edi

push eax
mov eax, [esp + 8]
mov dword [ebx + 32], eax
pop eax
push eax
mov eax, cs
mov word [ebx + 36], ax

mov eax, ds
mov word [ebx + 38], ax

mov eax, es
mov word [ebx + 40], ax

mov eax, fs
mov word [ebx + 42], ax

mov eax, gs
mov word [ebx + 44], ax

mov eax, ss
mov word [ebx + 46], ax

pop eax
mov ebx, [ebx + 16]

mov esp, ebp
pop ebp
ret 4; maybe retn