
[org 0x10200]
[bits 16]

;%include "/home/witut/Desktop/xaninOS/programming_libaries/xanin_asm.inc"

jmp shutdown


DAP:
size_of_packet: db 0x10
reserved: db 0x0
how_many_sectors: dw 0xD
offset: dw 0x800
segment_num: dw 0x0
lba: dd 0x2 + 0x10
lba48: dd 0x0


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
