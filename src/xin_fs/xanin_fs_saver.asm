extern xanin_saver_c

global xanin_saver

xanin_saver:


    mov al, 0x8
    out 0x21, al

    mov al, 0x70
    out 0xa1, al

    call xanin_saver_c

    ;jmp 0x20:prot16

    ;prot16:

    mov eax, cr0
    xor eax, 0x1 
    mov cr0,eax
    
    mov ax, 0x0
    jmp word 0x0:0x7c02



