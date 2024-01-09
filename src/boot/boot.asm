[org 0x7c00] ;there we start execution
[bits 16]; <-- 16bit real mode



jmp _loadSector

;12 + 2 + 16
XIN_FS_OFFSET: dd 0x1234 
XIN_PTRS_SIZE_IN_SECTORS: dd 0x1234
XIN_ENTRIES_SIZE_IN_SECTORS: dd 0x1234

BOOT_DISK_NUMBER: db 0x0

DAP:
size_of_packet: db 0x10
reserved: db 0x0
how_many_sectors: dw 0x35
offset: dw 0x0
segment_num: dw 0x2000
lba: dd 0x9a
lba48: dd 0x0

; VESA_CRTC_INFO_BLOCK:
; total_number_of_pixels_horizontally: dw 1000
; horizontal_sync_start: dw 0
; horizontal_sync_end: dw 999
; total_number_of_scan_lines: dw 80
; vertical_sync_start:  dw 0
; vertical_sync_end:  dw 80
; flags: db 0
; pixel_clock: dd 10
; refresh_rate: dw 10

_loadSector:

mov [BOOT_DISK_NUMBER], dl

;INITIALIZE SCREEN
; mov ax, 0x3
; int 0x10

;SETTING VIDEO MODE
; mov ax, 0x1111
; xor bl,bl
; int 10h

;mov ah, 0x1
;mov ch, 0x3f
;int 0x10

int13_read:

; D400
; A000
; 20000
; 37400
reading_disk:


;kernal_load
mov si, DAP
mov ah, 0x42
mov dl, [BOOT_DISK_NUMBER]
int 0x13

;kernel_1
mov word [lba], 0x9A + 0x35
mov word [offset], 0x35 * 0x200

mov si, DAP
mov ah, 0x42
mov dl, [BOOT_DISK_NUMBER]
int 0x13

;kernel_2
mov word [lba], 0x9A + 0x69 
mov word [offset], 0x6A * 0x200
mov word [how_many_sectors], 0x50

mov si, DAP
mov ah, 0x42
mov dl, [BOOT_DISK_NUMBER]
int 0x13

;kernel_3
mov word [lba], 0x9A + 0x69 + 0x50

mov word [offset], 0x0
mov word [segment_num], 0x1740
mov word [how_many_sectors], 0xD0

mov si, DAP
mov ah, 0x42
mov dl, [BOOT_DISK_NUMBER]
int 0x13

;xin_fs structure
;mov word [lba], 0x2 + 0x10
;mov word [offset], 0x800
;mov word [segment_num], 0x0
;mov word [how_many_sectors], 0x30

;mov si, DAP
;mov ah, 0x42
;mov dl, [BOOT_DISK_NUMBER]
;int 0x13

;program for returning to real mode
; mov word [lba], 0x1
; mov word [offset], 0x600
; mov word [segment_num], 0x0
; mov word [how_many_sectors], 0x1

; mov si, DAP
; mov ah, 0x42
; mov dl, [BOOT_DISK_NUMBER]
; int 0x13

;assembly applications
; mov word [lba], 0x2
; mov word [offset], 0x200
; mov word [segment_num], 0x1000
; mov word [how_many_sectors], 0x10

; mov si, DAP
; mov ah, 0x42
; mov dl, [BOOT_DISK_NUMBER]
; int 0x13

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


vesa_mode: dw 0x0
disk_num: dw 0x0
str: db "ERROR your computer doesnt support fully int13 xaninOS cant be loaded",0

times (0x10 - ( $ - $$ ) % 0x10 ) db 0x00

first_boot_signature: 
    sig: db "FIRST_BOOT",0x0

; xanin_information_block: 
;     signature: db "XANIN_INFO"
;     eax_r: dd 0x0
;     ecx_r: dd 0x0
;     edx_r: dd 0x0
;     ebx_r: dd 0x0
;     esp_r: dd 0x0
;     ebp_r: dd 0x0
;     esi_r: dd 0x0
;     edi_r: dd 0x0

;     ss_r: dw 0x0
;     cs_r: dw 0x0
;     ds_r: dw 0x0
;     es_r: dw 0x0
;     fs_r: dw 0x0
;     gs_r: dw 0x0

;     ip_r: dd 0x0

    


times 510 - ($-$$) db 0x0

dw 0xaa55

;vbe_info_structure:
;    vesa_attribute:                 dw 0
;    vesa_window_a:                  db 0
;    vesa_window_b:                  db 0
;    vesa_granularity:               dw 0
;    vesa_window_size:               dw 0 
;    vesa_segment_a:                 dw 0
;    vesa_segment_b:                 dw 0
;    vesa_win_func_ptr:              dd 0
;    vesa_pitch:                     dw 0
;    vesa_width:                     dw 0
;    vesa_w_char:                    db 0
;    vesa_y_char:                    db 0
;    vesa_planes:                    db 0
;    vesa_bpp:                       db 0
;    vesa_banks:                     db 0
;    vesa_memory_model:              db 0
;    vesa_bank_size:                 db 0
;    vesa_image_pages:               db 0
;    vesa_reserved0:                 db 0

;    vesa_red_mask:                  db 0
;    vesa_rred_position:             db 0
;    vesa_green_mask:                db 0 
;    vesa_green_position:            db 0
;    vesa_blue_mask:                 db 0
;    vesa_blue_position:             db 0
;    vesa_reserved_mask:             db 0
;    vesa_reserved_position:         db 0
;    vesa_direct_color_attributes:   db 0
;
;    vesa_framebuffer:               dd 0
;    vesa_off_screen_mem_off:        dd 0
;    vesa_off_screen_mem_size:       dw 0xffff
;    vesa_reserved1:                 resb 206
