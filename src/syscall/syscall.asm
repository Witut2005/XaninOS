
[bits 32]

global _syscall

extern xscanf
extern xprintf
extern putchar

_syscall:

    test eax, eax
    jz _xscanf

    cmp eax, 1
    je _xprintf

    cmp eax, 2
    je _putchar

    mov eax,0xffffffff
    iretd

_xscanf:
    push esi
    push edi
    
    call xscanf

    add esp, 0x8
    jmp _end

_xprintf:
    push esi
    call xprintf
    pop eax
    jmp _end

_putchar:
    push esi
    call putchar
    pop eax
    jmp _end

_end:
iretd


