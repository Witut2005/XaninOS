
[org 0x7c00] ;there we start execution
[bits 16]; <-- 16bit real mode



jmp _loadSector


BOOT_DISK_NUMBER: db 0x0

DAP:
size_of_packet: db 0x10
reserved: db 0x0
how_many_sectors: dw 0x35
offset: dw 0x0
segment_num: dw 0x2000
lba: dd 0xD + 1 + 0x10
lba48: dd 0x0

_loadSector:
mov [BOOT_DISK_NUMBER], dl


;INITIALIZE SCREEN
mov ax, 0x3
int 0x10

;SETTING VIDEO MODE
mov ax, 0x1111
xor bl,bl
int 10h


int13_read:


mov ax, 0x0
mov ds, ax
mov bx, 0x35
mov cx, 0x3

reading_disk:

;kernal_load
mov si, DAP
mov ah, 0x42
mov dl, [BOOT_DISK_NUMBER]
int 0x13

;kernel_1
mov word [lba], 0xD + 1 + 0x35 + 0x10
mov word [offset], 0x35 * 0x200

mov si, DAP
mov ah, 0x42
mov dl, [BOOT_DISK_NUMBER]
int 0x13

;kernel_2
mov word [lba], 0xD + 1 + 0x6A + 0x10
mov word [offset], 0x6A * 0x200

mov si, DAP
mov ah, 0x42
mov dl, [BOOT_DISK_NUMBER]
int 0x13

;xin_fs structure
mov word [lba], 0x2 + 0x10
mov word [offset], 0x800
mov word [segment_num], 0x0
mov word [how_many_sectors], 0xD

mov si, DAP
mov ah, 0x42
mov dl, [BOOT_DISK_NUMBER]
int 0x13

;program for returning to real mode
mov word [lba], 0x1
mov word [offset], 0x600
mov word [segment_num], 0x0
mov word [how_many_sectors], 0x1

mov si, DAP
mov ah, 0x42
mov dl, [BOOT_DISK_NUMBER]
int 0x13

;assembly applications
mov word [lba], 0x2
mov word [offset], 0x200
mov word [segment_num], 0x1000
mov word [how_many_sectors], 0x10

mov si, DAP
mov ah, 0x42
mov dl, [BOOT_DISK_NUMBER]
int 0x13



read_ok: 

mov ah,0xe
mov si,str

print_msg:
lodsb
cmp al,0
je jmp_ker_load
int 0x10
jmp print_msg

jmp_ker_load:

    mov ax, 0xA000
    mov ds,ax
    mov word [0x0], 0x4242

jmp word 0x2000:0x0000

idt_real:
dw 0x3ff
dd 0x0


vesa_mode: dw 0x0
disk_num: dw 0x0
str: db "ERROR your computer doesnt support fully int13 xaninOS cant be loaded",0

times 510 - ($-$$) db 0x0

dw 0xaa55
