
[org 0x600]
[bits 32]


fast_real_mode_switch:

    cli
    mov al, 0x8
    out 0x21, al

    mov al, 0x70
    out 0xa1, al

    [bits 32]
    jmp word 0x20:prot16

    [bits 16]
    prot16:

    jmp $

    cli
    lidt[IVT]

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
    ; jmp $
    mov sp, 0x2000

    mov ax, 0x0
    mov ds, ax
    mov ss, ax
    mov gs, ax
    mov fs, ax
    mov es, ax


    ; sti

    ; pushf
    ; push 0x0
    ; push 0x400

    ; pushf
    ; push bx
    ; push dx
    
    mov ax, 0xb800
    mov cs, ax
    mov ss, ax
    mov ds, ax
    mov es, ax

    mov bx, 0x0
    mov word [bx], 0x4141

    jmp $
    
    iret

    IVT:
    dw 0x03FF
    dd 0x0

    fswitch_str: db "xanin_fast_real_mode_enter",0x0

times 512 - ($-$$) db 0x0
