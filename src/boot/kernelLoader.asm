
[org 0x20000]
[bits 16]

jmp enter_32

elf_load_address: dd 0x90909090

enter_32:


mov ax, 0x2000
mov ds, ax
mov es, ax

mov ax, 0x1f00
mov ss, ax
xor sp, sp

lgdt[_GDT_ADDR] ;LGDT ALWAYS BEFORE ENTERING 32-BIT MODE

mov eax, cr0
or eax, 0x1
mov cr0,eax

jmp dword CODE_SEGMENT:(_bits32)


[bits 32]

_bits32:

mov eax, DATA_SEGMENT
mov es, eax
mov ds, eax
mov fs, eax
mov ss, eax
mov gs, eax

mov dword esp,0x01000000

mov eax, 0xb8000 ;VRAM 
mov dword [eax],0x0F410F41;print something


A20on:

in al, 0x92
or al, 2
out 0x92, al

;http://www.brokenthorn.com/Resources/OSDevPic.html

picConf:

cli

;ICW 1
mov al,0x11 
out PIC1_COMMAND,al
out PIC2_COMMAND,al

;ICW 2

mov al,0x20
out PIC1_DATA,al ;ten PIC obsługuje IRQ 0-7

mov al,0x28
out PIC2_DATA,al ;ten PIC obsługuje IRQ 8-15

;ICW 3

mov al,0x4 ; bit pos = IRQ Line <-- VERY IMPORTANT
out PIC1_DATA,al 

mov al,0x2 ; bit value = IRQ Line <-- VERY IMPORTANT
out PIC2_DATA,al

;ICW 4

mov al,0x1
out PIC1_DATA,al
out PIC2_DATA,al


;masking IRQs
mov al,0xff
out 0x21,al
out 0xA1,al



;stack test 
push 0x11111111
push 0x22222222
push 0x33333333


;https://www.youtube.com/watch?v=EbTNacDhqbA <-- GYNVAEL OSDEV VIDEO #3

loader:

mov word si, [kernel + 0x12]  ;machine type
cmp si, 0x3
je machine_good

cli
hlt

machine_good:
mov esi, [kernel + 0x18]
mov dword [elf_load_address], esi
mov esi, [kernel + 0x1C]  ;phofff point to header table
add esi, kernel           ; offset from begin + begin of kernel

movzx ecx, word [kernel + 0x2C] ; pnum contains number of entries in header table

cld 

  ; first p_vaddr.
xor ebp,ebp ; First PT_LOAD p_vaddr

.ph_loop:
mov eax, [esi + 0]
cmp eax, 1  ; If it's not PT_LOAD, ignore.
jne .next

push dword [esi + 0x4]    ; p_offset GIT
push dword [esi + 0x8]    ; p_vaddr GIT
push dword [esi + 0x10]   ; p_filesz
push dword [esi + 0x14]   ; p_memsz

test ebp,ebp
jnz .skip
mov ebp, [esp + 0x8]
.skip:

; Backup
mov ebx, esi
mov edx, ecx

; Zero memory
mov edi, [esp + 0x8]
mov ecx, [esp]
xor al, al
rep stosb

; Copy segment

mov esi,[esp + 0xc]
lea esi,[kernel + esi]
mov edi, [esp + 0x8]
mov ecx, [esp + 0x4]
rep movsb

; Restore
mov ecx, edx
mov esi, ebx
.next:
add esi, 0x20
loop .ph_loop

mov edi, ebp
mov dword eax, [kernel + 0x18]

jmp eax

jmp $ 

times (32 - ( $ - $$ ) % 32 ) db 0x00

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



times(512 - ( $ - $$ ) % 512) db 0x0


kernel:

;macros
    CODE_SEGMENT equ 0x8    ;0x0 + null_seg
    DATA_SEGMENT equ 0x10   ;0x0 + null_seg + code_seg
    STACK_SEGMENT equ 0x18  ;0x0 + null_seg + code_seg + data_seg

    PIC1_COMMAND equ 0x20
    PIC1_DATA equ 0x21

    PIC2_COMMAND equ 0xa0
    PIC2_DATA equ 0xa1

    KEYBOARD_ENCODER equ 0x60
    ONBOARD_KEY_CONTROLLER equ 0x64


