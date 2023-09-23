
[bits 32]
global elf_jump_to_entry_point

elf_jump_to_entry_point:

mov ebx, [esp + 16] ; sizeof section entry
mov edx, [esp + 12] ; sizeof section
mov ecx, [esp + 8]  ;.init array address
mov eax, [esp + 4]  ; entry point address
jmp eax