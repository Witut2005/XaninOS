
[org 0x10800]
[bits 32]

jmp load

;ATA REGISTERS
ATA_MASTER equ 0x1,
ATA_SLAVE equ 0x0,
ATA_FIRST_BUS equ 0x1F0,
ATA_SECONDARY_BUS equ 0x170,
ATA_DATA_REGISTER equ 0x0,
ATA_ERROR_REGISTER equ 0x1,
ATA_FEATURES_REGISTER equ 0x1,
ATA_SECTOR_COUNT_REGISTER equ 0x2,
ATA_SECTOR_NUMBER_LOW equ 0x3,
ATA_SECTOR_NUMBER_MID equ 0x4,
ATA_SECTOR_NUMBER_HIGH equ 0x5,
ATA_DRIVE_REGISTER equ 0x6,
ATA_STATUS_REGISTER equ 0x7,
ATA_COMMAND_REGISTER equ 0x7,
ATA_CONTROL_REGISTER equ 0x206


;ATA COMMANDS
ATA_IDENTIFY equ 0xEC,
ATA_READ equ 0x20,
ATA_WRITE equ 0x30,
ATA_FLUSH equ 0xE7

disk_status: db 0x0

load:

mov al, 0xe
mov dx, ATA_FIRST_BUS + ATA_DRIVE_REGISTER
out dx, al

mov al, 0x0
mov dx, ATA_FIRST_BUS + ATA_ERROR_REGISTER
out dx, al

;HOW MANY SECTORS
mov al, 100
mov dx, ATA_FIRST_BUS + ATA_SECTOR_COUNT_REGISTER
out dx, al

mov al, 0x43
mov dx, ATA_FIRST_BUS + ATA_SECTOR_NUMBER_LOW
out dx, al

mov al, 0x0
mov dx, ATA_FIRST_BUS + ATA_SECTOR_NUMBER_MID
out dx, al

mov al, 0x0
mov dx, ATA_FIRST_BUS + ATA_SECTOR_NUMBER_HIGH
out dx, al

mov al, ATA_READ
mov dx, ATA_FIRST_BUS + ATA_COMMAND_REGISTER
out dx, al

mov dx, ATA_FIRST_BUS + ATA_STATUS_REGISTER
in al, dx
mov [disk_status], al

wait_a_little_bit:
mov dx, ATA_FIRST_BUS + ATA_STATUS_REGISTER
in al, dx
mov [disk_status], al
and al, 0x81
cmp al, 0x80
je wait_a_little_bit

mov al, 0x0
out 0x80, al

mov al, 0x0
out 0x80, al

mov al, 0x0
out 0x80, al

mov al, 0x0
out 0x80, al

mov ecx, 0x20200

mov edx, 101
disk_load_loop:
dec edx
cmp edx, 0
je end

mov ebx, 0
second_loop:

mov dx, ATA_FIRST_BUS + ATA_DATA_REGISTER
in ax, dx
mov word [ecx], ax
add ecx, 2
inc ebx
cmp ebx, 256
je disk_load_loop
jmp second_loop

end:

times 512 - ($-$$) db 0x0
