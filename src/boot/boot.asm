
[org 0x7c00] ;there we start execution
[bits 16]; <-- 16bit real mode



jmp _loadSector
nop

;/------------------------------------------/
;|this filed is not used by ustar           |
;|this filed is created to handle easier FS |
;/------------------------------------------/


bios_parameter_block:
oem_id: dq "00000000"
bytes_per_sector: dw 0x200
sectors_per_cluster: db 0x1
reserved_sectors: dw 0x0
fats_number: db 0x2
root_directories_number: db 0x1
total_sectors: dw 0xFFFF
sectors_per_track: dw 80
number_of_heads: dw 1
hidden_sectors: dd 0x0
large_sector_count: dd 0x0

extended_boot_record:
dq 0x0
dq 0x0
dq 0x0
dw 0x0

USTAR_START:
db "OFF"
dd 0x0000

USTAR_FILE_ENTRIES:
db "ENTR"
db 0x0

BOOT_DISK_NUMBER:
db 0x0

DAP:
size_of_packer: db 0x10
reserved: db 0x0
how_many_sectors: dw 0x35
offset: dw 0x0
segment_num: dw 0x2000
lba: dd 0xD
lba48: dd 0x0

_loadSector:
mov [BOOT_DISK_NUMBER], dl

mov ax, 0x3
int 0x10

mov ax,1111h
xor bl,bl
int 10h


int13_read:


mov ax, 0x0
mov ds, ax
mov bx, 0x35
mov cx, 0x3

reading_disk:

mov si, DAP
mov ah, 0x42
mov dl, [BOOT_DISK_NUMBER]
int 0x13

mov word [lba], 0xD + 0x35
mov word [offset], 0x35 * 0x200

mov si, DAP
mov ah, 0x42
mov dl, [BOOT_DISK_NUMBER]
int 0x13

mov word [lba], 0x2
mov word [offset], 0x800
mov word [segment_num], 0x0
mov word [how_many_sectors], 0xD

mov si, DAP
mov ah, 0x42
mov dl, [BOOT_DISK_NUMBER]
int 0x13

read_ok: 

;mov ah,0x0
;mov ax,0x2000
;mov es,ax

;mov ah,0x2
;mov al,0x40
;mov ch,0x0
;mov cl,0x2 ; we dont want to copy first sector
;mov dh,0x0
;mov bx,0x0
;int 0x13
;dec bp
;cmp bp,0
;jne int13_read

mov ah,0xe
mov si,str

print_msg:
lodsb
cmp al,0
je jmp_ker_load
int 0x10
jmp print_msg

jmp_ker_load:
jmp word 0x2000:0x0000


disk_num: dw 0x0
str: db "ERROR your computer doesnt support fully int13 xaninOS cant be loaded",0

times 510 - ($-$$) db 0x0

dw 0xaa55
