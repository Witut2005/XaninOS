[bits 32]

global i8254x_interrupt_handler_entry
extern i8254x_interrupt_handler

%define APIC_SPURIOUS_INTERRUPT_VECTOR_REGISTER 0xFEE000F0
%define APIC_PRESENT (1 << 8)
%define APIC_EOI_REGISTER 0xFEE000B0

i8254x_interrupt_handler_entry:
    pushad
    call i8254x_interrupt_handler
    popad
    mov eax, [APIC_SPURIOUS_INTERRUPT_VECTOR_REGISTER]
    and eax, APIC_PRESENT
    cmp eax, APIC_PRESENT

    je apic_send_eoi
    jne pic_send_eoi

apic_send_eoi:
    mov dword eax, APIC_EOI_REGISTER
    mov dword [eax], 0x0
    iretd

pic_send_eoi:
    mov al, 0x20
    out 0x20, al
    iretd



