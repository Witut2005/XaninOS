[org 0x10400]
[bits 16]

;%include "/home/witut/Desktop/xaninOS/programming_libcaries/xanin_asm.inc"

jmp reboot


DAP:
size_of_packet: db 0x10
reserved: db 0x0
how_many_sectors: dw 0x30
offset: dw 0x800
segment_num: dw 0x0
lba: dd 0x2 + 0x10
lba48: dd 0x0



reboot:

    ;WRITE SECTORS ON DISK 
    ;DISK NUMBER 
    mov ax, 0x0
    mov ds, ax 
    mov dl, [0x7c02]
    

    mov ax, 0x1000
    
    ;DS:SI
    mov ds, ax 
    mov si, 0x402
   
    ;FUNC NUM AND FLAGS
    mov ah, 0x43
    mov al, 0x0

    int 0x13

    lidt[IVT]

    mov cx, 0x0
    div cx

    IVT:
        dw 0x0
        dd 0x0

    times 512 - ($-$$) db 0xD6