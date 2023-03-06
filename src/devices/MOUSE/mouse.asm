
[bits 32]

global mouse_handler_init
extern mouse_handler

%define APIC_SPURIOUS_INTERRUPT_VECTOR_REGISTER 0xFEE000F0
%define APIC_PRESENT (1 << 8)
%define APIC_EOI_REGISTER 0xFEE000B0

mouse_handler_init:
	mov word [0xb8000], 0x4141
    pushfd
    pushad
    call mouse_handler 
    popad
    popfd
    mov eax, [APIC_SPURIOUS_INTERRUPT_VECTOR_REGISTER]
    and eax, APIC_PRESENT
    cmp eax, APIC_PRESENT

    je apic_send_eoi
    jne pic_send_eoi

apic_send_eoi: 
    mov dword eax, APIC_EOI_REGISTER
    mov dword [eax], 0x0
    iretd

pic_send_eoi : 
    mov al, 0x20
    out 0x20, al
    iretd
