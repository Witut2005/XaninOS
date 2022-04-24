
[org 0x10200]
[bits 16]

;jmp shutdown


;DAP:
;size_of_packet: db 0x10
;reserved: db 0x0
;how_many_sectors: dw 0x10
;offset: dw 0x800
;segment_num: dw 0x0
;lba: dd 0x4
;lba48: dd 0x0



shutdown:

    ;WRITE SECTORS ON DISK 
    ;DISK NUMBER 
    ;mov ax, 0x0
    ;mov ds, ax 
    ;mov dl, [0x7c02]
    

    ;mov ax, 0x1000
    
    ;DS:SI
    ;mov ds, ax 
    ;mov si, 0x202
   
    ;FUNC NUM AND FLAGS
    ;mov ah, 0x43
    ;mov al, 0x0

    ;int 0x13



    ;DRIVE NUMBER  
    ;mov ax, 0x0
    ;mov ds, ax
    ;mov dl, [0x7c02]
    
    ;mov word [0x800], 0xFFFF
   
    ;DS:SI
    ;mov ax, 0x1000
    ;mov ds, ax 
    ;mov si, 0x202

    ;FUNC NUMBER
    ;mov ah, 0x42 
    ;int 0x13

    ;jmp $
    
    ;mov ax, 0x0
    ;mov ds, ax
        
    ;jmp $

    mov ax, 0x5307
    mov cx, 0x3
    mov bx, 0x1
    int 0x15

    jmp $

times 512 - ($-$$) db 0x0
