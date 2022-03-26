


idt_real_info:
    dw 0x03ff
    dw 0x0000
    dw 0x0000


xanin_saver:
    cli
    mov eax, 0x28
    mov ds, eax
    mov fs, eax
    mov gs, eax
    mov ss, eax
    mov es, eax

    ;lidt idt_real_info  
    
    mov eax, cr0
    xor eax, 0x1 
    mov cr0,eax

;    jmp word 0x20:0x7c02
    jmp word 0x0:0x7a05