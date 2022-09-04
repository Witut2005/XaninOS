
global xanin_cpu_backup_make

xanin_cpu_backup_make:
    push edi
    push esi

    ;get instruction pointer
    mov esi, [esp + 0x8]
    mov edi, 0x7C00 + 0x140 + 10
    mov dword [edi+44],esi
    pop dword esi

    ;general purpose registers
    mov dword [edi], eax
    mov dword [edi+4], ecx
    mov dword [edi+8], edx
    mov dword [edi+12], ebx
    mov dword [edi+16], esp
    mov dword [edi+20], ebp
    mov dword [edi+24], esi
    pop dword eax
    mov dword [edi+28], eax 

    ;segment registers
    mov eax, ss
    mov word [edi+32], ax
    mov eax, cs
    mov word [edi+34], ax
    mov eax, ds
    mov word [edi+36], ax
    mov eax, es
    mov word [edi+38], ax
    mov eax, fs
    mov word [edi+40], ax
    mov eax, gs
    mov word [edi+42], ax

    ret