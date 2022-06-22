
[org 0x400]
[bits 16]

;%include "/home/witut/Desktop/xaninOS/programming_libcaries/xanin_asm.inc"

shutdown:

    mov ax, 0xb800
    mov ds, ax

    mov word [0], 0x4141

    mov ax, 0x5307
    mov cx, 0x3
    mov bx, 0x1
    int 0x15

    jmp $

times 512 - ($-$$) db 0x0
