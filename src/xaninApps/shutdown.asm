
[org 0x10000]
[bits 16]






shutdown:
mov ax, 0x4F02	; set VBE mode
mov bx, 0x11A	; VBE mode number; notice that bits 0-13 contain the mode number and bit 14 (LFB) is set and bit 15 (DM) is clear.
int 0x10			; call VBE BIOS



    jmp $

    mov ax, 0xb800
    mov ds,ax
    mov word [0x0], 0x4242
    mov ax, 0x5307
    mov cx, 0x3
    mov bx, 0x1
    int 0x15

    jmp $

times 512 - ($-$$) db 0x0