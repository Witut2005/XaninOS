

[org 0x10000]

_reboot_external:
lidt [data]
mov edx, 0
div edx

data:
    dd 0x0

; mov eax, 88
; mov ebx, 0xfee1dead
; mov ecx, 672274793
; int 0x80