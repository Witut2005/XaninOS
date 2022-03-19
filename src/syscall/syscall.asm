

global _syscall

extern xprintf

_syscall:

    cmp eax, 1
    je _xprintf

    
    mov eax,0xffffffff
    iretd
_xprintf:
    push esi
    call xprintf
    add esp, 0x4
_end:
iretd


