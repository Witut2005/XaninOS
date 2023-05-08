
[bits 16]


shutdown:
    mov ax, 0x5307
    mov cx, 0x3
    mov bx, 0x1
    int 0x15

    jmp $

shutdown_str: db "xanin_shutdown",0xa,0x0

times 512 - ($-$$) db 0x0
