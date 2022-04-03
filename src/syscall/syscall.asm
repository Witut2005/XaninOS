
[bits 32]

global _syscall

%macro input_on 0

mov al, 0x20
out 0x20, al
sti

%endmacro

extern xscanf
extern xprintf
extern putchar
extern strlen
extern screen_clear
extern zsk
extern shutdown
extern xin_get_start_sector

get_eip:
    mov eax, [esp]
    ret

_syscall:
    test eax, eax
    jz _xscanf

    cmp eax, 1
    je _xprintf

    cmp eax, 2
    je _putchar

    cmp eax, 3
    je _strlen

    cmp eax, 4
    je _screen_clear

    ;16-BIT APPS
    cmp eax, 50
    je _shutdown

    ;32-BIT APPS
    cmp eax, 100
    je _zsk

    ;XIN_FILE_SYSTEM
    cmp eax, 200
    je _xin_get_start_sector

    mov eax,0xffffffff
    iretd

_xscanf:
    ;MORE ARGS !!!
    push edi
    push esi

	mov al,0x20
	out 0x20,al ; SEND EOI TO PIC1
    
    sti

    call xscanf

    xscanf_end:

    pop eax
    pop eax

    jmp _end

_xprintf:
    push esi
    call xprintf
    pop eax
    jmp _end

_putchar:
    push esi
    call putchar
    pop eax
    jmp _end


_strlen:
    push esi
    call strlen
    jmp _end

_screen_clear:
    call screen_clear
    jmp _end

_shutdown:
    input_on
    call shutdown

_zsk:
    input_on
    push esi
    call zsk
    pop eax
    jmp _end


_xin_get_start_sector:
    push esi
    call xin_get_start_sector
    jmp $
    pop eax
    jmp _end

_end:
iretd



