
[org 0x20000]

[bits 16]
return:

    mov ax, 0x2000
    mov ds, ax
    mov es, ax

    mov ax, 0x1f00
    mov ss, ax
    xor sp, sp

    ;lgdt[_GDT_ADDR] ;LGDT ALWAYS BEFORE ENTERING 32-BIT MODE

    mov eax, cr0
    or eax, 0x1
    mov cr0,eax


    jmp dword 0x8:0x425

[bits 32]
_bits32:
    jmp $
    mov edi, 0x7C00 + 0x140 + 10

    ;general purpose registers
    mov dword eax, [edi]
    mov dword ecx, [edi+4]
    mov dword edx, [edi+8]
    mov dword ebx, [edi+12]
    mov dword esp, [edi+16]
    mov dword ebp, [edi+20]
    mov dword esi, [edi+24]
    push dword eax

    ;segment registers
    mov eax, ss
    mov word [edi+32], ax
    mov eax, cs
    mov word [edi+34], ax
    mov eax, ds
    mov word [edi+36], ax
    mov eax, es
    mov word [edi+38], ax
    mov eax, fs
    mov word [edi+40], ax
    mov eax, gs
    mov word [edi+42], ax

    mov eax, 0x7C00 + 0x140 + 10
    mov dword edi, [eax] 
    pop eax

    push dword [0x7C00 + 0x140 + 10 + 44]
    jmp $
    ret




_GDT_ADDR:
    dw (_GDT_END - _GDT) - 1 ; size of GDT
    dd _GDT                  ; GDT beginning

_GDT:


    ;0x0
    ;null segment
    dd 0x0
    dd 0x0

    ;0x8
    ;code segment
    dd 0x0000ffff 
    db 0x0
    db 10011110b
    db 11001111b
    db 0x0

    ;0x10
    ;data segment
    dd 0x0000ffff 
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

    ;0x18
    ;stack segment
    dw 0
    dw 0xffff   
    db 0
    db 10010110b
    db 01000000b
    db 0xff

    ;0x20
    ;16bit code segment
    dd 0x0000ffff 
    db 0x0
    db 10011000b
    db 00001111b
    db 0x0
    
    ;0x28
    ;16bit data segment
    dd 0x0000ffff 
    db 0x0
    db 10010010b
    db 00001111b
    db 0x0



_GDT_END:



times 512 - ($-$$) db 0x0

CODE_SEGMENT equ 0x8    ;0x0 + null_seg
DATA_SEGMENT equ 0x10   ;0x0 + null_seg + code_seg
STACK_SEGMENT equ 0x18  ;0x0 + null_seg + code_seg + data_seg