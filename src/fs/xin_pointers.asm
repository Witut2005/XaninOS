

xin_fs_pointers:


;pointers: db 0x1,0x1,0xFF,0xFF, 

times 0x1000 - ($-$$) db 0x0 ; 4096
; times 0x100000 - ($-$$) db 0x0 ; 1048576
