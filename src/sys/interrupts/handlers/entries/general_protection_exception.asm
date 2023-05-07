
global general_protection_exception_entry 
extern general_protection_exception_handler
extern exit

section .data

section .text
general_protection_exception_entry:
    cli
    pushfd
    pusha
    call general_protection_exception_handler
    popa
    popfd
    sti
    call exit

