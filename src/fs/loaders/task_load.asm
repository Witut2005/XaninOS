[bits 32]

global task_load

task_load:
    mov esi, [esp + 4]

    ; push word cs
    ; push word ds
    ; push word ss

    ; movzx eax, cx
    ; mov cs, eax

    mov eax, 0x40
    mov ds, eax

    mov eax, 0x40
    mov ss, eax

    ; mov eax, 0x38
    ; mov cs, eax

    mov eax, esi
    jmp dword 0x38:0x10
    ; call dword [esi]
    ; pop word ss
    ; pop word ds
    ; pop word dx
    mov cs, dx
    ret