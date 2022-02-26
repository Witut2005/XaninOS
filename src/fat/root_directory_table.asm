
root_directory_table:

drive_entry:
drive_name: db "XANIN_OS"
drive_ext: db "   "
drive_atr: db 0x8
drive_case: db 0x0
creation_time_mili_sec: db 0x0
creation_time: dw 0x0
creation_date: dw 0x0
last_access: dw 0x0
reserved: dw 0x0
last_modification_time: dw 0x0
last_modification_date: dw 0x0
starting_cluster: dw 0x0
file_size: dd 0x0

times 2048 - ($-$$) db 0x0
