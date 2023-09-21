
[bits 32]
global elf_jump_to_entry_point

elf_jump_to_entry_point:
mov ebx, [esp + 8] ;.init array address
mov edx, [esp + 4] ; entry point address
jmp edx