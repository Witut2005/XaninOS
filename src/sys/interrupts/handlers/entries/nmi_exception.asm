


global nmi_interrupt_exception_entry 
extern nmi_interrupt_exception_handler
extern exit

section .data

section .text
nmi_interrupt_exception_entry:
    cli
    pushfd
    pusha
    call nmi_interrupt_exception_handler
    popa
    popfd
    sti
    call exit
