[org 0x600]


xanin_saver:

    mov al, 0x8
    out 0x21, al

    mov al, 0x70
    out 0xa1, al

    [bits 32]
    jmp word 0x20:prot16

    [bits 16]
    prot16:

    cli

    mov eax, 0x28
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax
    mov ss, eax

    mov eax, cr0
    and eax, 0x7FFFFFFE 
    mov cr0,eax
    
    jmp word 0x0:real_mode

    real_mode:
    mov sp, 0x2000

    mov ax, 0x0
    mov ds, ax
    mov ss, ax
    mov gs, ax
    mov fs, ax
    mov es, ax

    lidt[LVT]

    sti

    pushf
    push 0x0
    push 0x2000

    pushf
    push bx
    push dx
    
    iret

    LVT:
    dw 0x03FF
    dd 0x0

    times 512 - ($-$$) db 0x0   